#ifndef SIMULATION_H
#define SIMULATION_H

class Simulation
{
	public:
		std::vector<Road> roads;
		std::vector<Car> cars;
		int nb_cars;
		double time;
		int generation;
		sf::Font font;
		sf::Text generation_text;
		sf::Text nb_cars_text;
		int car_shown;
		std::vector<std::vector<sf::CircleShape>> neurons;
		std::vector<std::vector<sf::RectangleShape>> lines;

		Simulation(sf::RenderWindow& window, int car_amount, int road_amount);

		bool is_finish();
		void restart();
		void update(sf::RenderWindow& window);
		void draw(sf::RenderWindow& window);
};

#endif
