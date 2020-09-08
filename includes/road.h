#ifndef ROAD_H
#define ROAD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <functional>
#include <array>

#define START_POS { sf::Vector2f(100, 100), sf::Vector2f(100, 200)};
#define FINISH_POS { sf::Vector2f(1000, 1000), sf::Vector2f(1000, 1100) };
#define FINISH_CURSOR_TRIGGER 60

enum RoadState
{
	DRAWING,
	LEARNING
};

class Road
{
	public:
		int nb_cars;
		RoadState state;
		// les vecteurs contiennent les coordonnées des points des murs, array[0] = mur_1, array[1] = mur_2
		std::array<std::vector<sf::Vector2f>, 2> wall_points;
		std::array<sf::Vector2f, 2> start = START_POS;
		std::array<sf::Vector2f, 2> finish = FINISH_POS;

		Road(int nb_cars = 100, std::string filename = "");

		void update(sf::RenderWindow& window);
		void draw(sf::RenderWindow& window);
		void restart();
		bool parse_points(std::ifstream& file);

	private:
		int wall = 0;

		void update_drawing(sf::RenderWindow& window);
		void update_learning(sf::RenderWindow& window);
};

#endif
