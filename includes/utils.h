#ifndef UTILS_H
#define UTILS_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "car.h"
#include "network.h"
#include "road.h"
#include <cmath>
#include <iostream>
#include <cmath>
#include <array>
#include <SFML/Graphics.hpp>

extern int screen_width;

#define PI 3.14159265359
#define RESIZE ((1. / 1920.) * screen_width)
#define TIME_STEP .4

#define NETWORK_STRUCTURE {4, 1}
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
