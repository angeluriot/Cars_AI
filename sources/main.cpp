#include "utils.h"

/*
** Boucle principale du programme
*/
void start_loop(sf::RenderWindow& window)
{
	sf::Keyboard keyboard;
	sf::Clock clock_draw;
	sf::Clock clock_update;

	sf::View view = window.getView();
	view.zoom(2);
	window.setView(view);

	Simulation simulation(window, 100, 1);

	while (window.isOpen())
	{
		sf::Event sf_event;

		while (window.pollEvent(sf_event))
		{
			switch (sf_event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
			}
		}

		if (clock_update.getElapsedTime() > sf::milliseconds(5. * keyboard.isKeyPressed(sf::Keyboard::Space)))
		{
			simulation.update(window);
			clock_update.restart();
		}

		if (clock_draw.getElapsedTime() > sf::milliseconds(5.))
		{
			window.clear(sf::Color::White);
			simulation.draw(window);
			window.display();
			clock_draw.restart();
		}

		if (simulation.is_finish())
			simulation.restart();
	}
}

/*
** Retourne un tableau avec { width, height }
*/
std::array<unsigned int, 2> get_resolution()
{
	if (sf::VideoMode::getDesktopMode().width > (16. / 9.) * sf::VideoMode::getDesktopMode().height)
	{
		unsigned int height = sf::VideoMode::getDesktopMode().height * 3 / 4;
		return { height * 16 / 9, height };
	}

	if (sf::VideoMode::getDesktopMode().width < (16. / 9.) * sf::VideoMode::getDesktopMode().height)
	{
		unsigned int width = (sf::VideoMode::getDesktopMode().width * 3) / 4;
		return { width, width * 9 / 16 };
	}

	return { sf::VideoMode::getDesktopMode().width * 3 / 4, sf::VideoMode::getDesktopMode().height * 3 / 4 };
}

void create_window(sf::RenderWindow& window)
{
	auto res = get_resolution();
	screen_width = res[0];

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	window.create(sf::VideoMode(res[0], res[1]), "Car AI", sf::Style::Close | sf::Style::Titlebar, settings);
	sf::Image icon;
	icon.loadFromFile("dependencies/resources/icon.png");

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

int main()
{
	sf::RenderWindow window;
	create_window(window);

	screen_width = window.getSize().x;

	start_loop(window);
	return 0;
}
