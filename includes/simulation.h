#ifndef SIMULATION_H
#define SIMULATION_H

class Simulation
{
	public:
		std::vector<Road> roads;
		std::vector<Car> cars;

		Simulation(sf::RenderWindow& window, int car_amount, int road_amount);

		void update(sf::RenderWindow& window);
		void draw(sf::RenderWindow& window);
};

#endif
