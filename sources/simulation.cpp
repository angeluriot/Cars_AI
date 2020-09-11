#include "utils.h"

Simulation::Simulation(sf::RenderWindow& window, int car_amount, int road_amount)
{
	(void)road_amount;
	// Une seule route à dessiner pour le moment
	roads.push_back(Road(window));

	for (int i = 0; i < car_amount; i++)
		cars.push_back(Car(roads[0]));
}

void Simulation::update(sf::RenderWindow& window)
{
	for (auto& r : roads)
		r.update(window);
	for (auto& c : cars)
		c.update(roads[0]);
}

void Simulation::draw(sf::RenderWindow& window)
{
	for (auto& c : cars)
		c.draw(window);
	for (auto& r : roads)
		r.draw(window);
}
