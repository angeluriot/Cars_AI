#include "utils.h"

Line::Line()
{
}

Line::Line(const Line& line)
{
	*this = line;
}

Line::Line(const Vector& point_1, const Vector& point_2, const double& width, const sf::Color& color)
{
	rectangle.setFillColor(color);
	rectangle.setSize(sf::Vector2f(get_distance(point_1, point_2), width));
	rectangle.setOrigin(0., width / 2.);
	rectangle.setPosition(point_1.x, point_1.y);
	rectangle.setRotation(to_deg(get_angle(point_1, point_2)));

	left.setFillColor(color);
	left.setRadius(width / 2.);
	left.setOrigin(width / 2., width / 2.);
	left.setPosition(point_1.x, point_1.y);

	right.setFillColor(color);
	right.setRadius(width / 2.);
	right.setOrigin(width / 2., width / 2.);
	right.setPosition(point_2.x, point_2.y);
}

Line::Line(const double& x_1, const double& y_1, const double& x_2, const double& y_2, const double& width, const sf::Color& color)
{
	*this = Line(Vector(x_1, y_1), Vector(x_2, y_2), width, color);
}

Line::Line(const sf::Vector2i& point_1, const sf::Vector2i& point_2, const double& width, const sf::Color& color)
{
	*this = Line(Vector(point_1.x, point_1.y), Vector(point_2.x, point_2.y), width, color);
}

Line::Line(const sf::Vector2f& point_1, const sf::Vector2f& point_2, const double& width, const sf::Color& color)
{
	*this = Line(Vector(point_1.x, point_1.y), Vector(point_2.x, point_2.y), width, color);
}

void Line::operator=(const Line& line)
{
	rectangle = line.rectangle;
	left = line.left;
	right = line.right;
}

void Line::set_point1(const Vector& point_1)
{
	rectangle.setSize(sf::Vector2f(get_distance(point_1, get_point2()), get_width()));
	rectangle.setPosition(point_1.x, point_1.y);
	rectangle.setRotation(to_deg(get_angle(point_1, get_point2())));

	left.setPosition(point_1.x, point_1.y);
}

void Line::set_point1(const double& x, const double& y)
{
	set_point1(Vector(x, y));
}

void Line::set_point1(const sf::Vector2i& point_1)
{
	set_point1(Vector(point_1.x, point_1.y));
}

void Line::set_point1(const sf::Vector2f& point_1)
{
	set_point1(Vector(point_1.x, point_1.y));
}

void Line::set_point2(const Vector& point_2)
{
	rectangle.setSize(sf::Vector2f(get_distance(get_point1(), point_2), get_width()));
	rectangle.setRotation(to_deg(get_angle(get_point1(), point_2)));

	right.setPosition(point_2.x, point_2.y);
}

void Line::set_point2(const double& x, const double& y)
{
	set_point2(Vector(x, y));
}

void Line::set_point2(const sf::Vector2i& point_2)
{
	set_point2(Vector(point_2.x, point_2.y));
}

void Line::set_point2(const sf::Vector2f& point_2)
{
	set_point2(Vector(point_2.x, point_2.y));
}

Vector Line::get_point1() const
{
	return Vector(left.getPosition().x, left.getPosition().y);
}

Vector Line::get_point2() const
{
	return Vector(right.getPosition().x, right.getPosition().y);
}

void Line::set_width(const double& width)
{
	rectangle.setSize(sf::Vector2f(rectangle.getSize().x, width));
	left.setRadius(width / 2.);
	right.setRadius(width / 2.);
}

double Line::get_width() const
{
	return rectangle.getSize().y;
}

void Line::set_color(const sf::Color& color)
{
	rectangle.setFillColor(color);
	left.setFillColor(color);
	right.setFillColor(color);
}

sf::Color Line::get_color() const
{
	return rectangle.getFillColor();
}

double Line::get_length()
{
	return rectangle.getSize().x;
}

void Line::draw(sf::RenderWindow& window)
{
	window.draw(rectangle);
	window.draw(left);
	window.draw(right);
}
