#ifndef UTILS_H
#define UTILS_H
#include <SFML/Graphics.hpp>

class Bird;

extern int screen_width;

#define PI 3.14159265359
#define RESIZE ((1. / 1920.) * screen_width)
#define TIME_STEP 0.4
#define GRAVITY (5. * RESIZE)

#define BIRD_POSITION_X (300. * RESIZE)
#define BIRD_POSITION_Y (540. * RESIZE)
#define BIRD_SIZE_X (102. * RESIZE)
#define BIRD_SIZE_Y (72. * RESIZE)
#define JUMP_SPEED (-30. * RESIZE)

#define PIPE_SIZE_X (150. * RESIZE)
#define PIPE_SIZE_Y (1500. * RESIZE)
#define PIPE_GAP_X (300. * RESIZE)
#define PIPE_GAP_Y (200. * RESIZE)
#define PIPE_SPEED (10. * RESIZE)
#define PIPE_ADJUSTMENT (10. * RESIZE)
#define PIPE_RAND_LIMIT (100. * RESIZE)

#define NETWORK_STRUCTURE {4, 1}
#define NETWORK_POSITION_X ((1920. - 280.) * RESIZE)
#define NETWORK_POSITION_Y ((1080. - 280.) * RESIZE)
#define NETWORK_SIZE_X (200. * RESIZE)
#define NETWORK_SIZE_Y (200. * RESIZE)
#define NEURON_RADIUS (20. * RESIZE)
#define LINE_MAX (20. * RESIZE)

double sigmoid(const double& number);
double random();
double normalize(const double& number, const double& min, const double& max);
sf::RectangleShape create_line(const double& position_1_x, const double& position_1_y, const double& position_2_x, const double& position_2_y);
sf::CircleShape create_neuron(const double& position_x, const double& position_y);
void draw_network(std::vector<std::vector<sf::CircleShape>>& neurons, std::vector<std::vector<sf::RectangleShape>>& lines, const std::vector<Bird>& birds, int bird_shown);

#endif