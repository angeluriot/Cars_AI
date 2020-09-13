#ifndef UTILS_H
#define UTILS_H
#include <SFML/Graphics.hpp>
#include "vector.h"
#include "line.h"
#include "network.h"
#include "road.h"
#include "car.h"
#include "simulation.h"
#include <cmath>
#include <iostream>
#include <cmath>
#include <array>
#include <SFML/Graphics.hpp>
#include <sstream>

extern int screen_width;

#define PI 3.14159265359
#define RESIZE ((1. / 1920.) * screen_width)
#define TIME_STEP (1./ 60.)

#define START_POINT_1 sf::Vector2i(100. * RESIZE, 100. * RESIZE)
#define START_POINT_2 sf::Vector2i(100. * RESIZE, 200. * RESIZE)

#define FINISH_POINT_1 sf::Vector2i((1920. - 100.) * RESIZE, (1080. - 200.) * RESIZE)
#define FINISH_POINT_2 sf::Vector2i((1920. - 100.) * RESIZE, (1080. - 100.) * RESIZE)

#define START_POS { START_POINT_1, START_POINT_2};
#define FINISH_POS { FINISH_POINT_1, FINISH_POINT_2 };
#define FINISH_CURSOR_TRIGGER 100
#define TIME_MIN 30.
#define TIME_MAX 1000.

#define CAR_LENGTH 80.
#define CAR_WIDTH 40.
#define CAR_COLOR sf::Color(50, 50, 200)
#define DEAD_CAR_COLOR sf::Color(150, 150, 255)
#define MAX_SPEED 50.
#define MAX_ROTATION 3.

#define LASER_WIDTH 3
#define LASER_COLOR sf::Color(255, 150, 150)
#define LASER_MAX 100000.
#define VIEW_MAX (CAR_LENGTH * 5.)

#define NETWORK_STRUCTURE { 5, 4, 3, 2 }
#define NETWORK_POSITION_X (2250. * RESIZE)
#define NETWORK_POSITION_Y (-450. * RESIZE)
#define NETWORK_SIZE_X (500. * RESIZE)
#define NETWORK_SIZE_Y (300. * RESIZE)
#define NEURON_RADIUS (30. * RESIZE)
#define LINE_MAX (30. * RESIZE)

double sigmoid(const double& number);
double random_factor();
double normalize(const double& number, const double& min, const double& max);
sf::RectangleShape create_line(const double& position_1_x, const double& position_1_y, const double& position_2_x, const double& position_2_y);
sf::CircleShape create_neuron(const double& position_x, const double& position_y);
void draw_network(std::vector<std::vector<sf::CircleShape>>& neurons, std::vector<std::vector<sf::RectangleShape>>& lines, const std::vector<Car>& cars, int car_shown, sf::RenderWindow& window);
double distanceSq(const sf::Vector2i& a, const sf::Vector2i& b);
double distanceSq(const Vector& a, const Vector& b);
bool intersection(const Vector& point_1, const Vector& point_2, const Vector& point_3, const Vector& point_4, Vector& intersection);
double distance_to_line(const Vector& point, const Vector& point_1, const Vector& point_2);
double to_deg(const double& angle);
double to_rad(const double& angle);

#endif
