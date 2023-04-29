#include <SFML/Graphics.hpp>
#include <iostream>
#include "config.h"
#include "field.h"

int main() {

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Tetris by dffTu", sf::Style::Default);

	Field field(window);
	sf::Clock clock;
	while (window.isOpen()) {
		double time = clock.getElapsedTime().asMicroseconds() / 1e6;
		clock.restart();
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else {
				field.updateFigure(time, event);
			}
		}
		window.clear(sf::Color::White);
		field.draw(time);
		window.display();
	}
	return 0;
}