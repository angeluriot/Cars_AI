#include "neuron.h"
#include "utils.h"
#include "level.h"
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

int main()
{
	int width;
	int height;

	if (sf::VideoMode::getDesktopMode().width > (16. / 9.) * sf::VideoMode::getDesktopMode().height)
	{
		height = (sf::VideoMode::getDesktopMode().height * 3) / 4;
		width = (height * 16) / 9;
	}

	else if (sf::VideoMode::getDesktopMode().width < (16. / 9.) * sf::VideoMode::getDesktopMode().height)
	{
		width = (sf::VideoMode::getDesktopMode().width * 3) / 4;
		height = (width * 9) / 16;
	}

	else
	{
		width = (sf::VideoMode::getDesktopMode().width * 3) / 4;
		height = (sf::VideoMode::getDesktopMode().height * 3) / 4;
	}

	screen_width = width;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(width, height), "Flappy bird AI", sf::Style::Close | sf::Style::Titlebar, settings);

	sf::Image icon;
	icon.loadFromFile("dependencies/resources/icon.png");

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	sf::Keyboard keyboard;
	sf::Clock clock_draw;
	sf::Clock clock_update;

	Level level(1000);

	sf::sleep(sf::seconds(3));

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

		if (clock_update.getElapsedTime() > sf::milliseconds(5. * !keyboard.isKeyPressed(sf::Keyboard::Space)))
		{
			level.update(0);
			clock_update.restart();
		}

		if (clock_draw.getElapsedTime() > sf::milliseconds(5.))
		{
			window.clear(sf::Color(0, 180, 255));
			level.draw(window);
			window.display();
			clock_draw.restart();
		}

		if (level.nb_birds == 0 or level.score == 100)
			level.restart();
	}

	return 0;
}