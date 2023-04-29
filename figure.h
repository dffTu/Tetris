#pragma once

#include <SFML/Graphics.hpp>
#include "config.h"
#include "geometric_primitives.h"

class Figure {
	sf::Color color;
	double x, y;
	double scale_factor;

public:
	bool cell[4][4];

	Figure() {
		std::srand(std::time(0));
		int color_ind = std::rand() % (sizeof(COLORS) / sizeof(COLORS[0]));
		color = COLORS[color_ind];
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				cell[i][j] = false;
			}
		}
		int type = std::rand() % 7;						
		if (type == 0) {							// Square figure
			cell[1][1] = cell[1][2] = cell[2][1] = cell[2][2] = true;
		} else if (type == 1) {							// T figure
			cell[2][0] = cell[1][1] = cell[2][1] = cell[2][2] = true;
		} else if (type == 2) {							// Z figure
			cell[2][0] = cell[2][1] = cell[1][1] = cell[1][2] = true;
		} else if (type == 3) {							// S figure
			cell[1][0] = cell[2][1] = cell[1][1] = cell[2][2] = true;
		} else if (type == 4) {							// L figure
			cell[3][1] = cell[2][1] = cell[1][1] = cell[3][2] = true;
		} else if (type == 5) {							// L-backwards figure
			cell[3][1] = cell[2][1] = cell[1][1] = cell[1][2] = true;
		} else if (type == 6) {							// I figure
			cell[0][1] = cell[1][1] = cell[2][1] = cell[3][1] = true;
		}
		int rotations_count = std::rand() % 4;
		while (rotations_count > 0) {
			rotate();
			--rotations_count;
		}
		scale_factor = 1;
	}

	sf::Color getColor() {
		return color;
	}

	size_t countLeftEmptyLines() {
		size_t result = 0;
		for (int x = 0; x < 4; ++x) {
			for (int y = 0; y < 4; ++y) {
				if (cell[y][x]) {
					return result;
				}
			}
			result++;
		}
		return result;
	}

	size_t countRightEmptyLines() {
		size_t result = 0;
		for (int x = 3; x >= 0; --x) {
			for (int y = 0; y < 4; ++y) {
				if (cell[y][x]) {
					return result;
				}
			}
			result++;
		}
		return result;
	}

	size_t countUpEmptyLines() {
		size_t result = 0;
		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				if (cell[y][x]) {
					return result;
				}
			}
			result++;
		}
		return result;
	}

	size_t countDownEmptyLines() {
		size_t result = 0;
		for (int y = 3; y >= 0; --y) {
			for (int x = 0; x < 4; ++x) {
				if (cell[y][x]) {
					return result;
				}
			}
			result++;
		}
		return result;
	}

	void setPosition(double new_x, double new_y) {
		x = new_x;
		y = new_y;
	}

	void setScale(double new_scale_factor) {
		scale_factor = new_scale_factor;
	}

	void rotate() {
		bool cell_tmp[4][4];
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				cell_tmp[i][j] = cell[3 - j][i];
			}
		}
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				cell[i][j] = cell_tmp[i][j];
			}
		}
	}

	void draw(sf::RenderWindow& window) {
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f((TILE_SIZE - LINE_THICKNESS) * scale_factor, (TILE_SIZE - LINE_THICKNESS) * scale_factor));
		rect.setFillColor(color);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(LINE_THICKNESS * scale_factor);
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (!cell[i][j]) {
					continue;
				}
				rect.setPosition(sf::Vector2f(x + j * TILE_SIZE * scale_factor, y + i * TILE_SIZE * scale_factor));
				window.draw(rect);
			}
		}
	}
};
