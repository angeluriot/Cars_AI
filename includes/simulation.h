#ifndef SIMULATION_H
#define SIMULATION_H

class Simulation
{
	public:
		std::vector<Road> roads;
		std::vector<Car> cars;
		double time;

		Simulation(sf::RenderWindow& window, int car_amount, int road_amount);

		bool is_finish();
		void restart();
		void update(sf::RenderWindow& window);
		void draw(sf::RenderWindow& window);
};

#endif
