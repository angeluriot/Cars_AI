#include "utils.h"

Car::Car(Road& road)
{
	this->road = &road;
	brain = Network(NETWORK_STRUCTURE);
	position = Vector(road.start[0].x, road.start[0].y + ((road.start[1].y - road.start[0].y) / 2.) + random_factor() * ((road.start[1].y - road.start[0].y) / 4.));
	speed = 0.;
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

	if (road.state == LEARNING)
		update_lasers();

	alive = true;
	finish = false;
}

Car::Car(const Car& car)
{
	*this = car;
}

void Car::operator=(const Car& car)
{
	road = car.road;
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
		position + Vector((-CAR_LENGTH / 2.) * cos(-rotation) + (-CAR_WIDTH / 2.) * sin(-rotation), (-CAR_LENGTH / 2.) * -sin(-rotation) + (-CAR_WIDTH / 2.) * cos(-rotation)),
		position + Vector((-CAR_LENGTH / 2.) * cos(-rotation) + ( CAR_WIDTH / 2.) * sin(-rotation), (-CAR_LENGTH / 2.) * -sin(-rotation) + ( CAR_WIDTH / 2.) * cos(-rotation)),
		position + Vector(( CAR_LENGTH / 2.) * cos(-rotation) + (-CAR_WIDTH / 2.) * sin(-rotation), ( CAR_LENGTH / 2.) * -sin(-rotation) + (-CAR_WIDTH / 2.) * cos(-rotation)),
		position + Vector(( CAR_LENGTH / 2.) * cos(-rotation) + ( CAR_WIDTH / 2.) * sin(-rotation), ( CAR_LENGTH / 2.) * -sin(-rotation) + ( CAR_WIDTH / 2.) * cos(-rotation))
	};
}

void Car::update_sprite()
{
	sprite.setPosition(to_vector2f(position));
	sprite.setRotation(to_deg(rotation));
}

void Car::update_lasers()
{
	lasers = {
		Vector_polar(LASER_MAX, rotation + PI / 2.),
		Vector_polar(LASER_MAX, rotation + PI / 4.),
		Vector_polar(LASER_MAX, rotation),
		Vector_polar(LASER_MAX, rotation - PI / 4.),
		Vector_polar(LASER_MAX, rotation - PI / 2.)
	};

	for (int i = 0; i < lasers.size(); i++)
	{
		Vector laser_end;
		Vector min_laser_end = position + lasers[i];

		for (int k = 0; k < road->wall_points.size(); k++)
		{
			if (intersection(position, position + lasers[i], road->start[k], road->wall_points[k][0], laser_end) && get_distance(position, laser_end) < get_distance(position, min_laser_end))
				min_laser_end = laser_end;

			for (int j = 0; j < road->wall_points[k].size() - 1; j++)
				if (intersection(position, position + lasers[i], road->wall_points[k][j], road->wall_points[k][j + 1], laser_end) && get_distance(position, laser_end) < get_distance(position, min_laser_end))
					min_laser_end = laser_end;

			if (intersection(position, position + lasers[i], road->wall_points[k].back(), road->finish[k], laser_end) && get_distance(position, laser_end) < get_distance(position, min_laser_end))
				min_laser_end = laser_end;
		}

		lasers_sprites[i].set_point1(position);
		lasers_sprites[i].set_point2(min_laser_end);
	}
}

void Car::update_alive()
{
	for (auto& corner : corners)
	{
		for (int k = 0; k < road->wall_points.size(); k++)
		{
			if (distance_to_line(corner, road->start[k], road->wall_points[k][0]) < speed * TIME_STEP)
			{
				alive = false;
				break;
			}

			for (int j = 0; j < road->wall_points[k].size() - 1; j++)
				if (distance_to_line(corner, road->wall_points[k][j], road->wall_points[k][j + 1]) < speed * TIME_STEP)
				{
					alive = false;
					break;
				}

			if (!alive)
				break;

			if (distance_to_line(corner, road->wall_points[k].back(), road->finish[k]) < speed * TIME_STEP)
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

	if (time > TIME_MIN)
	{
		for (auto& corner : corners)
		{
			if (corner.x < road->start[0].x)
			{
				alive = false;
				sprite.setFillColor(DEAD_CAR_COLOR);
				break;
			}
		}
	}
}

void Car::update_finish()
{
	for (auto& corner : corners)
		if (distance_to_line(corner, road->finish[0], road->finish[1]) < speed * TIME_STEP)
		{
			finish = true;
			break;
		}

	if (finish)
		sprite.setFillColor(sf::Color::Red);
}

std::vector<double> Car::look()
{
	std::vector<double> view;

	for (int i = 0; i < lasers_sprites.size(); i++)
		view.push_back(normalize(lasers_sprites[i].get_length(), CAR_WIDTH, VIEW_MAX));

	return view;
}

std::vector<double> Car::think(const std::vector<double>& view)
{
	brain.update_outputs(view);

	return brain.get_outputs();
}

void Car::move(const std::vector<double>& thought)
{
	speed = thought[0] * MAX_SPEED;
	rotation += ((thought[1] * 2) - 1.) * MAX_ROTATION * TIME_STEP;

	position += Vector_polar(speed * TIME_STEP, rotation);
}

void Car::update()
{
	if (alive && !finish && road->state == LEARNING)
	{
		move(think(look()));

		time += TIME_STEP;
		distance += speed * TIME_STEP;

		update_corners();
		update_lasers();
		update_sprite();

		sprite.setFillColor(CAR_COLOR);

		update_alive();
		update_finish();
	}
}

double Car::get_score()
{
	//return (finish ? normalize(time, 0., 30.) / 2. + 0.5 : normalize(distance, 0., 2000.) / 2.);

	#define START (road->start[0] + ((road->start[1] - road->start[0]) / 2.))
	#define FINISH (road->finish[0] + ((road->finish[1] - road->finish[0]) / 2.))

	return (finish ? normalize(time, TIME_MIN, TIME_MAX) / 2. + 0.5 : normalize(get_distance(START, FINISH) - get_distance(position, FINISH), 0., get_distance(START, FINISH)) / 2.);
}

void Car::recreate()
{
	Network temp = brain;
	*this = Car(*road);
	brain = temp;
}

void Car::recreate_from(const Car& car)
{
	double score = get_score();
	brain = car.brain;
	brain.mutate(score);
}

void Car::draw_car(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Car::draw_lasers(sf::RenderWindow& window)
{
	if (alive && !finish)
		for (auto& laser_sprite : lasers_sprites)
			laser_sprite.draw(window);
}