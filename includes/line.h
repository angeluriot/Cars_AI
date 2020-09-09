#ifndef LINE_H
#define LINE_H

#include <SFML/Graphics.hpp>

class Line
{
	public:

		sf::RectangleShape rectangle;
		sf::CircleShape left;
		sf::CircleShape right;

		Line();
		Line(const Line& line);
		Line(const Vector& point_1, const Vector& point_2, const double& width, const sf::Color& color);
		Line(const double& x_1, const double& y_1, const double& x_2, const double& y_2, const double& width, const sf::Color& color);
		Line(const sf::Vector2i& point_1, const sf::Vector2i& point_2, const double& width, const sf::Color& color);
		Line(const sf::Vector2f& point_1, const sf::Vector2f& point_2, const double& width, const sf::Color& color);

		void operator=(const Line& line);

		void set_point1(const Vector& point_1);
		void set_point1(const double& x, const double& y);
		void set_point1(const sf::Vector2i& point_1);
		void set_point1(const sf::Vector2f& point_1);

		void set_point2(const Vector& point_2);
		void set_point2(const double& x, const double& y);
		void set_point2(const sf::Vector2i& point_2);
		void set_point2(const sf::Vector2f& point_2);

		Vector get_point1() const;
		Vector get_point2() const;

		void set_width(const double& width);
		double get_width() const;

		void set_color(const sf::Color& color);
		sf::Color get_color() const;

		double get_length();

		void draw(sf::RenderWindow& window);
};

#endif
