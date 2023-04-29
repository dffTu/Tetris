#pragma once

#include <SFML/Graphics.hpp>

const int TILE_SIZE = 40;
const int INDENT = 40;
const int INFO_WIDTH = 150;
const int TOP_HEIGHT = 50;
const int SPACE_BETWEEN_TEXT = 20;
const int TILES_WIDTH_COUNT = 10;
const int TILES_HEIGHT_COUNT = 20;
const int SCREEN_WIDTH = TILES_WIDTH_COUNT * TILE_SIZE + INDENT * 2 + INFO_WIDTH;
const int SCREEN_HEIGHT = TILES_HEIGHT_COUNT * TILE_SIZE + INDENT + TOP_HEIGHT;
const double LINE_THICKNESS = 3;
const double SPEED_ADDING = 0.1;
const double NEXT_FIGURE_SCALE = 0.75;

const sf::Color COLORS[6] = { 
	sf::Color::Red, 
	sf::Color::Green, 
	sf::Color::Blue, 
	sf::Color::Yellow, 
	sf::Color::Cyan,
	sf::Color::Magenta
};