#ifndef ROAD_H
#define ROAD_H

class Road
{
	public:
		int nb_cars;

		Road(int nb_cars);

		void update();
		void draw(sf::RenderWindow& window);
		void restart();
};

#endif
