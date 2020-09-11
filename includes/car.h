#ifndef CAR_H
#define CAR_H
#include <array>

class Car
{
	public :

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

		Car(const Road& road);
		Car(const Car& car);

		void operator=(const Car& car);

		void update_corners();
		void update_sprite();
		void update_lasers(const Road& road);
		void update_alive(const Road& road);
		void update_finish(const Road& road);
		std::vector<double> look();
		std::vector<double> think(const std::vector<double>& view);
		void move(const std::vector<double>& thought);
		void update(const Road& road);
		double get_score();
		void recreate_from(const Car& car);
		void draw(sf::RenderWindow& window);
};

#endif
