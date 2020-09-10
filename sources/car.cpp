#include "utils.h"

Car::Car(const Road& road)
{
	brain = Network(NETWORK_STRUCTURE);
	position = Vector(SPAWN_POSITION + random_factor() * SPAWN_AREA);
	speed = Vector(0., 0.);
	rotation = 0.;
	time = 0.;
	distance = 0.;

	update_corners();

	sprite.setFillColor(CAR_COLOR);
	sprite.setSize(sf::Vector2f(CAR_LENGTH, CAR_WIDTH));
	sprite.setOrigin(CAR_LENGTH / 2., CAR_WIDTH / 2.);
	sprite.setPosition(to_vector2f(position));

	lasers_sprites = {
		Line(position, position + Vector(1., 1.), LASER_WIDTH, LASER_COLOR),
		Line(position, position + Vector(1., 1.), LASER_WIDTH, LASER_COLOR),
		Line(position, position + Vector(1., 1.), LASER_WIDTH, LASER_COLOR),
		Line(position, position + Vector(1., 1.), LASER_WIDTH, LASER_COLOR),
		Line(position, position + Vector(1., 1.), LASER_WIDTH, LASER_COLOR),
	};

	update_lasers(road);

	alive = true;
	finish = false;
}

Car::Car(const Car& car)
{
	*this = car;
}

void Car::operator=(const Car& car)
{
	brain = car.brain;
	position = car.position;
	speed = car.speed;
	rotation = car.rotation;
	time = car.time;
	distance = car.distance;
	corners = car.corners;
	sprite = car.sprite;
	lasers = car.lasers;
	lasers_sprites = car.lasers_sprites;
	alive = car.alive;
	finish = car.finish;
}

void Car::update_corners()
{
	corners = {
		position + Vector((-CAR_LENGTH / 2.) * cos(rotation) + (-CAR_WIDTH / 2.) * sin(rotation), (-CAR_LENGTH / 2.) * -sin(rotation) + (-CAR_WIDTH / 2.) * cos(rotation)),
		position + Vector((-CAR_LENGTH / 2.) * cos(rotation) + ( CAR_WIDTH / 2.) * sin(rotation), (-CAR_LENGTH / 2.) * -sin(rotation) + ( CAR_WIDTH / 2.) * cos(rotation)),
		position + Vector(( CAR_LENGTH / 2.) * cos(rotation) + (-CAR_WIDTH / 2.) * sin(rotation), ( CAR_LENGTH / 2.) * -sin(rotation) + (-CAR_WIDTH / 2.) * cos(rotation)),
		position + Vector(( CAR_LENGTH / 2.) * cos(rotation) + ( CAR_WIDTH / 2.) * sin(rotation), ( CAR_LENGTH / 2.) * -sin(rotation) + ( CAR_WIDTH / 2.) * cos(rotation))
	};
}

void Car::update_sprite()
{
	sprite.setPosition(to_vector2f(position));
	sprite.setRotation(rotation * (180. / PI));
}

void Car::update_lasers(const Road& road)
{
	lasers = {
		Vector_polar(10000., rotation + PI / 2.),
		Vector_polar(10000., rotation + PI / 4.),
		Vector_polar(10000., rotation),
		Vector_polar(10000., rotation - PI / 4.),
		Vector_polar(10000., rotation - PI / 2.)
	};

	for (int i = 0; i < lasers.size(); i++)
	{
		Vector laser_end;
		Vector min_laser_end = lasers[i];

		for (int k = 0; k < road.wall_points.size(); k++)
		{
			if (intersection(position, position + lasers[i], road.start[k], road.wall_points[k][0], laser_end) && laser_end.get_norm() < min_laser_end.get_norm())
				min_laser_end = laser_end;

			for (int j = 0; j < road.wall_points[k].size() - 1; j++)
				if (intersection(position, position + lasers[i], road.wall_points[k][j], road.wall_points[k][j + 1], laser_end) && laser_end.get_norm() < min_laser_end.get_norm())
					min_laser_end = laser_end;

			if (intersection(position, position + lasers[i], road.wall_points[k].back(), road.finish[k], laser_end) && laser_end.get_norm() < min_laser_end.get_norm())
				min_laser_end = laser_end;
		}

		lasers_sprites[i].set_point2(min_laser_end);
	}
}

void Car::update_alive(const Road& road)
{
	for (auto& corner : corners)
	{
		for (int k = 0; k < road.wall_points.size(); k++)
		{
			if (distance_to_line(corner, road.start[k], road.wall_points[k][0]) < MAX_MOVE)
			{
				finish = true;
				break;
			}

			for (int j = 0; j < road.wall_points[k].size() - 1; j++)
				if (distance_to_line(corner, road.wall_points[k][j], road.wall_points[k][j + 1]) < MAX_MOVE)
				{
					alive = false;
					break;
				}

			if (!alive)
				break;

			if (distance_to_line(corner, road.wall_points[k].back(), road.finish[k]) < MAX_MOVE)
			{
				alive = false;
				break;
			}
		}

		if (!alive)
			break;
	}

	if (!alive)
		sprite.setFillColor(DEAD_CAR_COLOR);
}

void Car::update_finish(const Road& road)
{
	for (auto& corner : corners)
		if (distance_to_line(corner, road.finish[0], road.finish[1]) < MAX_MOVE)
		{
			finish = true;
			break;
		}
}

std::vector<double> Car::look()
{
	std::vector<double> view;

	for (int i = 0; i < lasers_sprites.size(); i++)
		view.push_back(normalize(lasers_sprites[i].get_length(), 0, VIEW_MAX));

	return view;
}

std::vector<double> Car::think(const std::vector<double>& view)
{
	brain.update_outputs(view);
}

void Car::move(const std::vector<double>& thought)
{
	speed.set_norm(speed.get_norm() + brain.get_output(0) * MAX_BOOST);
	speed.set_norm(speed.get_norm() > MAX_SPEED ? MAX_SPEED : speed.get_norm());

	rotation += brain.get_output(1) * TURN_RADIUS;
	speed.set_angle(speed.get_angle() + brain.get_output(1) * TURN_RADIUS);
}

void Car::update(const Road& road)
{
	if (alive && !finish)
	{
		move(think(look()));

		position += speed * TIME_STEP;
		time += TIME_STEP;
		distance += speed.get_norm() * TIME_STEP;

		update_corners();
		update_lasers(road);
		update_sprite();
		update_alive(road);
		update_finish(road);
	}
}

double Car::get_score()
{
	return (finish ? normalize(time, 0., 30.) / 2. + 0.5 : normalize(distance, 0., 2000.) / 2.);
}

void Car::recreate_from(const Car& car)
{
	double score = get_score();
	*this = car;

	brain.mutate(score);
}