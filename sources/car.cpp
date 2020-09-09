#include "utils.h"

Car::Car()
{
	brain = Network(NETWORK_STRUCTURE);
	score = 0.;
	position = Vector(SPAWN_POSITION + random_factor() * SPAWN_AREA);
	rotation = 0;
	update_corners();
	sprite;
	alive;
}

Car::Car(const Car& car)
{

}

void Car::operator=(const Car& car)
{

}

void Car::update_corners()
{
	corners = { position + Vector((-CAR_LENGTH / 2.) * cos(rotation) + (-CAR_WIDTH / 2.) * sin(rotation), (-CAR_LENGTH / 2.) * -sin(rotation) + (-CAR_WIDTH / 2.) * cos(rotation)),
				position + Vector((-CAR_LENGTH / 2.) * cos(rotation) + ( CAR_WIDTH / 2.) * sin(rotation), (-CAR_LENGTH / 2.) * -sin(rotation) + ( CAR_WIDTH / 2.) * cos(rotation)),
				position + Vector(( CAR_LENGTH / 2.) * cos(rotation) + (-CAR_WIDTH / 2.) * sin(rotation), ( CAR_LENGTH / 2.) * -sin(rotation) + (-CAR_WIDTH / 2.) * cos(rotation)),
				position + Vector(( CAR_LENGTH / 2.) * cos(rotation) + ( CAR_WIDTH / 2.) * sin(rotation), ( CAR_LENGTH / 2.) * -sin(rotation) + ( CAR_WIDTH / 2.) * cos(rotation)) };
}

std::array<double, 2> Car::think()
{
	return std::array<double, 2>();
}

void Car::update()
{
}

void Car::recreate_from(const Car& car)
{
}
