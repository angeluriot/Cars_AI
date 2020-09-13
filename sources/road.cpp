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
	file.open(filename);

	if (!file)
		return ;

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

bool Road::mouse_cross_line(sf::RenderWindow& window)
{
	Vector back = Vector(wall_points[this->wall].empty() ? start[this->wall] : wall_points[this->wall].back());
	Vector mouse = Vector(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	Vector dummy;

	if (sf::Mouse::getPosition(window).x > window.getSize().x || sf::Mouse::getPosition(window).y > window.getSize().y || sf::Mouse::getPosition(window).x < 0 || sf::Mouse::getPosition(window).y < 0)
		return true;
	for (int i = 0; i < 2; i++)
	{
		sf::Vector2f s = start[i];
		for (auto& wall : wall_points[i])
		{
			if (wall != back && intersection(Vector(s), Vector(wall), back, mouse, dummy))
				return true;
			s = wall;
		}
	}
	Vector back_wall0 = Vector(wall_points[0].empty() ? start[0] : wall_points[0].back());
	if (this->wall == 1 && intersection(Vector(finish[0]), back_wall0, back, mouse, dummy) && distanceSq(mouse, finish[this->wall]) >= FINISH_CURSOR_TRIGGER)
		return true;
	if (intersection(Vector(start[0]), Vector(start[1]), back, mouse, dummy))
		return true;
	if (intersection(Vector(finish[0]), Vector(finish[1]), back, mouse, dummy))
		return true;
	return false;
}

void Road::update_drawing(sf::RenderWindow& window)
{
	static bool pressed = true;
	sf::Mouse mouse;
	sf::Vector2f mousePos = window.mapPixelToCoords(mouse.getPosition(window));

	if (!pressed && mouse.isButtonPressed(sf::Mouse::Left))
	{
		pressed = true;
		if (mouse_cross_line(window))
			return ;
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
		sf::Color color = !mouse_cross_line(window) ? sf::Color(230, 230, 230) : sf::Color(255, 180, 180);
		Line(wall_points[this->wall].back(), mouseCoords, 5, color).draw(window);
	}
	else if (this->state == DRAWING)
	{
		mouseCoords = distanceSq(sf::Mouse::getPosition(window), window.mapCoordsToPixel(finish[this->wall])) < FINISH_CURSOR_TRIGGER ? finish[this->wall] : mouseCoords;
		sf::Color color = !mouse_cross_line(window) ? sf::Color(230, 230, 230) : sf::Color(255, 180, 180);
		Line(start[this->wall], mouseCoords, 5, color).draw(window);
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
