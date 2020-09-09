#ifndef UTILS_H
#define UTILS_H
#include <SFML/Graphics.hpp>
#include "vector.h"
#include "line.h"
#include "network.h"
#include "car.h"
#include "road.h"
#include <cmath>
#include <iostream>
#include <cmath>
#include <array>
#include <SFML/Graphics.hpp>

extern int screen_width;

#define PI 3.14159265359
#define RESIZE ((1. / 1920.) * screen_width)
#define TIME_STEP (1./ 60.)

#define START_POINT_1 sf::Vector2f(100, 100)
#define START_POINT_2 sf::Vector2f(100, 200)

#define FINISH_POINT_1 sf::Vector2f(1000, 1000)
#define FINISH_POINT_2 sf::Vector2f(1000, 1100)

#define START_POS { START_POINT_1, START_POINT_2};
#define FINISH_POS { FINISH_POINT_1, FINISH_POINT_2 };
#define FINISH_CURSOR_TRIGGER 60

#define SPAWN_AREA ((Vector(START_POINT_2) - Vector(START_POINT_1)) / 2.)
#define SPAWN_POSITION (Vector(START_POINT_1) + SPAWN_AREA)

#define CAR_LENGTH 20.
#define CAR_WIDTH 10.
#define CAR_COLOR sf::Color(50, 50, 200)
#define TURN_RADIUS 0.05

#define NETWORK_STRUCTURE { 5, 4, 3, 2 }
#define NETWORK_POSITION_X ((1920. - 280.) * RESIZE)
#define NETWORK_POSITION_Y ((1080. - 280.) * RESIZE)
#define NETWORK_SIZE_X (200. * RESIZE)
#define NETWORK_SIZE_Y (200. * RESIZE)
#define NEURON_RADIUS (20. * RESIZE)
#define LINE_MAX (20. * RESIZE)

double sigmoid(const double& number);
double random_factor();
double normalize(const double& number, const double& min, const double& max);
sf::RectangleShape create_line(const double& position_1_x, const double& position_1_y, const double& position_2_x, const double& position_2_y);
sf::CircleShape create_neuron(const double& position_x, const double& position_y);
void draw_network(std::vector<std::vector<sf::CircleShape>>& neurons, std::vector<std::vector<sf::RectangleShape>>& lines, const std::vector<Car>& birds, int bird_shown);
double distanceSq(const sf::Vector2f& a, const sf::Vector2f& b);

#endif
