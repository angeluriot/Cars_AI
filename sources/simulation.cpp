#include "utils.h"

Simulation::Simulation(sf::RenderWindow& window, int car_amount, int road_amount)
{
	(void)road_amount;
	// Une seule route à dessiner pour le moment
	roads.push_back(Road(window));

	time = 0.;

	for (int i = 0; i < car_amount; i++)
		cars.push_back(Car(roads[0]));

	neurons.clear();
	lines.clear();
	font.loadFromFile("dependencies/resources/GoogleSans-Bold.ttf");
	car_shown = 0;
	generation = 0;
	nb_cars = car_amount;

	nb_cars_text.setFont(font);
	nb_cars_text.setFillColor(sf::Color::Black);
	nb_cars_text.setCharacterSize(70. * RESIZE);
	nb_cars_text.setString("Nb cars : " + std::to_string(car_amount));
	nb_cars_text.setPosition(-850. * RESIZE, 1450. * RESIZE);

	generation_text = nb_cars_text;
	generation_text.setCharacterSize(70. * RESIZE);
	generation_text.setString("Gen : " + std::to_string(generation));
	generation_text.setPosition(-850. * RESIZE, 1350. * RESIZE);

	std::vector<int> layers = NETWORK_STRUCTURE;

	double position_y;
	double position_y_2;

	for (int i = 0; i < layers.size(); i++)
	{
		neurons.push_back({});
		lines.push_back({});

		for (int j = 0; j < layers[i]; j++)
		{
			if (layers[i] > 1)
				position_y = NETWORK_POSITION_Y + (NETWORK_SIZE_Y / ((double)layers[i] - 1)) * j;

			else
				position_y = NETWORK_POSITION_Y + NETWORK_SIZE_Y / 2.;

			neurons[i].push_back(create_neuron(NETWORK_POSITION_X + (NETWORK_SIZE_X / ((double)layers.size() - 1)) * i, position_y));

			if (i != layers.size() - 1)
			{
				for (int k = 0; k < layers[i + 1]; k++)
				{
					if (layers[i + 1] > 1)
						position_y_2 = NETWORK_POSITION_Y + (NETWORK_SIZE_Y / ((double)layers[i + 1] - 1)) * k;

					else
						position_y_2 = NETWORK_POSITION_Y + NETWORK_SIZE_Y / 2.;

					lines[i].push_back(create_line(NETWORK_POSITION_X + (NETWORK_SIZE_X / (double)(layers.size() - 1)) * i, position_y,
						NETWORK_POSITION_X + (NETWORK_SIZE_X / (double)(layers.size() - 1)) * (i + 1), position_y_2));
				}
			}
		}
	}
}

bool Simulation::is_finish()
{
	if (time > TIME_MAX)
		return true;

	for (Car& car : cars)
		if (car.alive)
			return false;

	return true;
}

void Simulation::restart()
{
	time = 0.;
	generation++;
	nb_cars = cars.size();

	std::sort(cars.begin(), cars.end(), [](Car car_1, Car car_2) -> bool { return (car_1.get_score() < car_2.get_score()); });

	for (int i = 0; i < cars.size() / 2.; i++)
		cars[i].recreate_from(cars[i * 2]);

	for (Car& car : cars)
		car.recreate();
}

void Simulation::update(sf::RenderWindow& window)
{
	for (auto& r : roads)
		r.update(window);

	if (roads[0].state == LEARNING)
	{
		time += TIME_STEP;

		double max_score = 0.;
		nb_cars = 0;

		for (int i = 0; i < cars.size(); i++)
		{
			cars[i].update();

			if (cars[i].alive && !cars[i].finish)
			{
				nb_cars++;

				if (cars[i].get_score() > max_score)
				{
					car_shown = i;
					max_score = cars[i].get_score();
				}
			}
		}

		cars[car_shown].sprite.setFillColor(sf::Color::Green);

		nb_cars_text.setString("Nb cars : " + std::to_string(nb_cars));
		generation_text.setString("Gen : " + std::to_string(generation));
	}
}

void Simulation::draw(sf::RenderWindow& window)
{
	if (roads[0].state == LEARNING)
	{
		for (auto& c : cars)
			c.draw_lasers(window);

		for (auto& c : cars)
			c.draw_car(window);
	}

	for (auto& r : roads)
		r.draw(window);

	if (roads[0].state == LEARNING)
	{
		window.draw(nb_cars_text);
		window.draw(generation_text);

		draw_network(neurons, lines, cars, car_shown, window);
	}
}
