#include "road.h"

/*
**	Si il y a un fichier, les points sont parsés depuis ce dernier
*/
Road::Road(int nb_cars, std::string filename)
{
	this->nb_cars = nb_cars;
	this->state = DRAWING;

	std::ifstream file;
	file.open(filename);

	if (!file.is_open())
		return ;

	this->state = LEARNING;
	if (!parse_points(file))
	{
		// Gestion d'erreur
		std::exit(1);
	}
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
		if (!std::getline(file, line))
		{
			for (auto wall : wall_points)
				wall.clear();
			return false;
		}
		for (int i = 0; line[i]; i++)
		{
			std::array<int, 2> coords;
			for (int j = 0; j < coords.size(); j++)
			{
				if (!std::isdigit(line[i]))
				{
					for (auto wall : wall_points)
						wall.clear();
					return false;
				}
				coords[j] = std::stoi(line.substr(i, line.size()));
				i += line.substr(i, line.size()).size() + 1;
			}
			wall_points[wall].push_back(sf::Vector2i(coords[0], coords[1]));
		}
	}
	return true;
}

void Road::update_drawing()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (distanceSq(sf::Mouse::getPosition(), finish[wall]) < FINISH_CURSOR_TRIGGER)
			wall++;
		else
			wall_points[wall].push_back(sf::Mouse::getPosition());
	}

	if (wall == 2)
		state = LEARNING;
}

void Road::update_learning()
{

}

/*
**	Lance la fonction d'update correspondant à state
*/
void Road::update()
{
	std::map<RoadState, void (Road::*)(void)> funcs { {DRAWING, &Road::update_drawing}, {LEARNING, &Road::update_learning} };
	auto pair = funcs.find(state);
	std::invoke(pair->second, *this);
}

void Road::draw(sf::RenderWindow& window)
{

}

void Road::restart()
{

}
