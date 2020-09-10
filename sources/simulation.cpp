#include "utils.h"

Simulation::Simulation(sf::RenderWindow& window, int car_amount, int road_amount)
{
	(void)road_amount;
	cars.assign(car_amount, Car());
	// Une seule route à dessiner pour le moment
	roads.push_back(Road(window));
}

void Simulation::update(sf::RenderWindow& window)
{
	for (auto& r : roads)
		r.update(window);
	for (auto& c : cars)
		c.update();
}

void Simulation::draw(sf::RenderWindow& window)
{
	for (auto& r : roads)
		r.draw(window);
	/*for (auto& c : cars)
		c.update();*/
}
