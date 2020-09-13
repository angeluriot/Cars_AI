#include "utils.h"

/*
**	Si il y a un fichier, les points sont parsés depuis ce dernier
*/
Road::Road(sf::RenderWindow& window, int nb_cars, std::string filename)
{
	this->nb_cars = nb_cars;
	this->wall = 0;
	this->state = DRAWING;
	this->start = { window.mapPixelToCoords(START_POINT_1), window.mapPixelToCoords(START_POINT_2) };
	this->finish = { window.mapPixelToCoords(FINISH_POINT_1), window.mapPixelToCoords(FINISH_POINT_2) };

	std::ifstream file;
	if(file.open(filename)){
		return;
	}

	

	this->state = LEARNING;
	this->wall = 2;
	parse_points(file);
}

/*
**	Parse les points d'un fichier au format :
**	point1X point1Y point2X point2Y ...
**	point1X point1Y point2X point2Y
**	où la première ligne représente les points du mur 1 et la 2e ceux du mur 2
*/
bool Road::parse_points(std::ifstream& file)
{
	std::string line;

	for (int wall = 0; !file.eof() && wall < 2; wall++)
	{
		std::getline(file, line);

		int i = 0;
		while (i < line.size() && line[i] != '\n')
		{
			std::array<float, 2> coords;

			for (int j = 0; j < coords.size(); j++)
			{
				coords[j] = std::stof(line.substr(i));
				while (line[i] != ' ' && line[i])
					i++;
				if (line[i] == ' ')
					i++;
			}

			wall_points[wall].push_back(sf::Vector2f(coords[0], coords[1]));
		}
	}
	file.close();
	return true;
}

void Road::update_drawing(sf::RenderWindow& window)
{
	static bool pressed = true;
	sf::Mouse mouse;

	if (!pressed && mouse.isButtonPressed(sf::Mouse::Left))
	{
		pressed = true;
		if (distanceSq(mouse.getPosition(window), window.mapCoordsToPixel(finish[wall])) < FINISH_CURSOR_TRIGGER)
			wall++;
		else
			wall_points[wall].push_back(window.mapPixelToCoords(mouse.getPosition(window)));
	}
	else if (pressed && !mouse.isButtonPressed(sf::Mouse::Left))
		pressed = false;

	if (wall == 2)
		state = LEARNING;
}

void Road::update_learning(sf::RenderWindow& window)
{

}

/*
**	Lance la fonction d'update correspondant à state
*/
void Road::update(sf::RenderWindow& window)
{
	std::map<RoadState, void (Road::*)(sf::RenderWindow&)> funcs { {DRAWING, &Road::update_drawing}, {LEARNING, &Road::update_learning} };
	auto pair = funcs.find(state);
	std::invoke(pair->second, *this, window);
}

void Road::draw(sf::RenderWindow& window)
{
	sf::Vector2f mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	// Ligne du départ au 1er point
	for (int i = 0; i < 2; i++)
		if (!wall_points[i].empty())
			Line(start[i], wall_points[i][0], 5, sf::Color(150, 150, 150)).draw(window);

	// Lignes entre les points
	for (auto wall : wall_points)
		for (int i = 1; i < wall.size(); i++)
			Line(wall[i - 1], wall[i], 5, sf::Color(150, 150, 150)).draw(window);

	// Ligne du dernier point à l'arrivée, ou du départ à l'arrivée si il n'y a pas de point intermédiaire
	for (int i = 0; i < this->wall; i++)
		if (!wall_points[i].empty())
			Line(finish[i], wall_points[i].back(), 5, sf::Color(150, 150, 150)).draw(window);
		else
			Line(start[i], finish[i], 5, sf::Color(150, 150, 150)).draw(window);

	// Trait du dernier point au curseur (ou à l'arrivée si on est dans la range)
	if (this->state == DRAWING && !wall_points[this->wall].empty())
	{
		mouseCoords = distanceSq(sf::Mouse::getPosition(window), window.mapCoordsToPixel(finish[this->wall])) < FINISH_CURSOR_TRIGGER ? finish[this->wall] : mouseCoords;
		Line(wall_points[this->wall].back(), mouseCoords, 5, sf::Color(230, 230, 230)).draw(window);
	}
	else if (this->state == DRAWING)
	{
		mouseCoords = distanceSq(sf::Mouse::getPosition(window), window.mapCoordsToPixel(finish[this->wall])) < FINISH_CURSOR_TRIGGER ? finish[this->wall] : mouseCoords;
		Line(start[this->wall], mouseCoords, 5, sf::Color(230, 230, 230)).draw(window);
	}

	// Lignes de départ et d'arrivée
	Line(start[0], start[1], 5, sf::Color(0, 180, 0)).draw(window);
	Line(finish[0], finish[1], 5, sf::Color(180, 0, 0)).draw(window);
}

void Road::restart()
{

}

bool Road::export_race(std::string filename)
{
	std::fstream file;
	file.open(filename, std::fstream::in | std::fstream::out);

	if (file)
		return false;
	file.open(filename, std::fstream::out | std::fstream::trunc);
	for (auto wall : wall_points)
	{
		for (int i = 0; i < wall.size(); i++)
			file << wall[i].x << ' ' << wall[i].y << (i == wall.size() - 1 ? "" : " ");
		file << '\n';
	}
	file.close();
	return true;
}
