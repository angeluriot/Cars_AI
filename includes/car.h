#ifndef CAR_H
#define CAR_H
#include <array>

class Car
{
	public :

		Road* road;
		Network brain;
		Vector position;
		double speed;
		double rotation;
		double time;
		double distance;
		std::array<Vector, 4> corners;
		sf::RectangleShape sprite;
		std::array<Vector, 5> lasers;
		std::array<Line, 5> lasers_sprites;
		bool alive;
		bool finish;

		Car(Road& road);
		Car(const Car& car);

		void operator=(const Car& car);

		void update_corners();
		void update_sprite();
		void update_lasers();
		void update_alive();
		void update_finish();
		std::vector<double> look();
		std::vector<double> think(const std::vector<double>& view);
		void move(const std::vector<double>& thought);
		void update();
		double get_score();
		void recreate();
		void recreate_from(const Car& car);
		void draw_car(sf::RenderWindow& window);
		void draw_lasers(sf::RenderWindow& window);
};

#endif
