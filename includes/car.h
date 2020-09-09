#ifndef CAR_H
#define CAR_H
#include <array>

class Car
{
	public :

		Network brain;
		double score;
		Vector position;
		double rotation;
		std::array<Vector, 4> corners;
		sf::RectangleShape sprite;
		bool alive;

		Car();
		Car(const Car& car);

		void operator=(const Car& car);

		void update_corners();
		std::array<double, 2> think();
		void update();
		void recreate_from(const Car& car);
};

#endif
