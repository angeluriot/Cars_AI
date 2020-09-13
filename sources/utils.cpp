#include "utils.h"

int screen_width;

double sigmoid(const double& number)
{
	return 1. / (1. + exp(-number));
}

double random_factor()
{
	return (((double)rand() / (double)RAND_MAX) * 2.) - 1.;
}

double normalize(const double& number, const double& min, const double& max)
{
	return sigmoid(((number - min) / (max - min)) * 8. - 4.);
}

sf::CircleShape create_neuron(const double& position_x, const double& position_y)
{
	sf::CircleShape neuron;

	neuron.setFillColor(sf::Color::Black);
	neuron.setRadius(NEURON_RADIUS);
	neuron.setOrigin(NEURON_RADIUS, NEURON_RADIUS);
	neuron.setPosition(position_x, position_y);

	return neuron;
}

sf::RectangleShape create_line(const double& position_1_x, const double& position_1_y, const double& position_2_x, const double& position_2_y)
{
	sf::RectangleShape line;
	double angle = atan2(position_2_y - position_1_y, position_2_x - position_1_x);

	line.setFillColor(sf::Color::Black);
	line.setSize(sf::Vector2f((position_2_x - position_1_x) / cos(angle), LINE_MAX));
	line.setOrigin(0., LINE_MAX / 2.);
	line.setPosition(position_1_x, position_1_y);
	line.setRotation(to_deg(angle));

	return line;
}

void draw_network(std::vector<std::vector<sf::CircleShape>>& neurons, std::vector<std::vector<sf::RectangleShape>>& lines, const std::vector<Car>& cars, int car_shown, sf::RenderWindow& window)
{
	for (int i = 1; i < cars[car_shown].brain.layers.size(); i++)
	{
		for (int j = 0; j < cars[car_shown].brain.layers[i].size(); j++)
		{
			if (cars[car_shown].brain.layers[i][j].add_value > 0.)
				neurons[i][j].setFillColor(sf::Color::Blue);

			else
				neurons[i][j].setFillColor(sf::Color::Red);

			for (int k = 0; k < cars[car_shown].brain.layers[i][j].weights.size(); k++)
			{
				if (cars[car_shown].brain.layers[i][j].weights[k] > 0.)
				{
					lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].setFillColor(sf::Color::Blue);
					lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].setSize(sf::Vector2f(lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].getSize().x,
						cars[car_shown].brain.layers[i][j].weights[k] * LINE_MAX));
					lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].setOrigin(lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].getOrigin().x,
						lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].getSize().y / 2.);
				}

				else
				{
					lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].setFillColor(sf::Color::Red);
					lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].setSize(sf::Vector2f(lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].getSize().x,
						cars[car_shown].brain.layers[i][j].weights[k] * LINE_MAX * -1));
					lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].setOrigin(lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].getOrigin().x,
						lines[i - 1][k * cars[car_shown].brain.layers[i].size() + j].getSize().y / 2.);
				}
			}
		}
	}

	for (auto& line : lines)
		for (auto& l : line)
			window.draw(l);

	for (auto& neuron : neurons)
		for (auto& n : neuron)
			window.draw(n);
}

double distanceSq(const sf::Vector2i& a, const sf::Vector2i& b)
{
	double distX = a.x - b.x;
	double distY = a.y - b.y;
	return (distX * distX + distY * distY);
}

double distanceSq(const Vector& a, const Vector& b)
{
	double distX = a.x - b.x;
	double distY = a.y - b.y;
	return (distX * distX + distY * distY);
}

bool intersection(const Vector& point_1, const Vector& point_2, const Vector& point_3, const Vector& point_4, Vector& intersection)
{
	if (point_1 == point_2 || point_3 == point_4)
		return false;

	if (point_1.x != point_2.x && point_3.x != point_4.x)
	{
		double a_1 = (point_2.y - point_1.y) / (point_2.x - point_1.x);
		double a_2 = (point_4.y - point_3.y) / (point_4.x - point_3.x);

		if (a_1 == a_2)
			return false;

		double b_1 = point_1.y - a_1 * point_1.x;
		double b_2 = point_3.y - a_2 * point_3.x;

		intersection.x = (b_2 - b_1) / (a_1 - a_2);
		intersection.y = a_1 * intersection.x + b_1;

		if (std::min(point_1.x, point_2.x) < intersection.x && std::max(point_1.x, point_2.x) > intersection.x && std::min(point_3.x, point_4.x) < intersection.x && std::max(point_3.x, point_4.x) > intersection.x)
			return true;
	}

	else if (point_1.x != point_2.x)
	{
		double a_1 = (point_2.y - point_1.y) / (point_2.x - point_1.x);
		double b_1 = point_1.y - a_1 * point_1.x;

		intersection.x = point_3.x;
		intersection.y = a_1 * intersection.x + b_1;

		if (std::min(point_1.x, point_2.x) < intersection.x && std::max(point_1.x, point_2.x) > intersection.x && std::min(point_3.y, point_4.y) < intersection.y && std::max(point_3.y, point_4.y) > intersection.y)
			return true;
	}

	else if (point_3.x != point_4.x)
	{
		double a_2 = (point_4.y - point_3.y) / (point_4.x - point_3.x);
		double b_2 = point_3.y - a_2 * point_3.x;

		intersection.x = point_1.x;
		intersection.y = a_2 * intersection.x + b_2;

		if (std::min(point_1.y, point_2.y) < intersection.y && std::max(point_1.y, point_2.y) > intersection.y && std::min(point_3.x, point_4.x) < intersection.x && std::max(point_3.x, point_4.x) > intersection.x)
			return true;
	}

	return false;
}

double distance_to_line(const Vector& point, const Vector& point_1, const Vector& point_2)
{
	double distance = abs((point_2.y - point_1.y) * point.x - (point_2.x - point_1.x) * point.y + point_2.x * point_1.y - point_2.y * point_1.x) / sqrt(pow(point_2.y - point_1.y, 2) + pow(point_2.x - point_1.x, 2));

	if (point_1.x != point_2.x)
	{
		if (std::min(point_1.x, point_2.x) > point.x || std::max(point_1.x, point_2.x) < point.x )
			return std::min(get_distance(point, point_1), get_distance(point, point_2));
	}

	else if(point_1.y != point_2.y)
	{
		if (std::min(point_1.y, point_2.y) > point.y || std::max(point_1.y, point_2.y) < point.y)
			return std::min(get_distance(point, point_1), get_distance(point, point_2));
	}

	else
		return std::min(get_distance(point, point_1), get_distance(point, point_2));

	return distance;
}

double to_deg(const double& angle)
{
	return angle * (180. / PI);
}

double to_rad(const double& angle)
{
	return angle * (PI / 180.);
}