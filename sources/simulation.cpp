#include "utils.h"

Simulation::Simulation(sf::RenderWindow& window, int car_amount, int road_amount)
{
	(void)road_amount;
	// Une seule route à dessiner pour le moment
	roads.push_back(Road(window));

	time = 0.;

	for (int i = 0; i < car_amount; i++)
		cars.push_back(Car(roads[0]));
}

bool Simulation::is_finish()
{
	return (time > 300.);
}

void Simulation::restart()
{
	time = 0.;

	std::sort(cars.begin(), cars.end(), [](Car car_1, Car car_2) -> bool { return (car_1.get_score() < car_2.get_score()); });

	for (int i = 0; i < cars.size() / 2.; i++)
		cars[i].recreate_from(cars[i * 2]);

	for (Car& car : cars)
		car.recreate();
}

void Simulation::update(sf::RenderWindow& window)
{
	time += TIME_STEP;
	for (auto& r : roads)
		r.update(window);
	for (auto& c : cars)
		c.update();
}

void Simulation::draw(sf::RenderWindow& window)
{
	for (auto& c : cars)
		c.draw(window);
	for (auto& r : roads)
		r.draw(window);
}
