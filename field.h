#pragma once

#include <SFML/Graphics.hpp>
#include "config.h"
#include "geometric_primitives.h"
#include "figure.h"

class Field {
	sf::RenderWindow& window;

	size_t score;
	bool non_empty_cell[TILES_HEIGHT_COUNT][TILES_WIDTH_COUNT];
	sf::Color cell_color[TILES_HEIGHT_COUNT][TILES_WIDTH_COUNT];
	Figure current_figure;
	Figure next_figure;
	double timer;
	int current_y, current_x;
	double game_speed;

	bool game_running;
	bool game_started;

public:
	Field(sf::RenderWindow& win) : window(win), current_figure(), next_figure() {
		game_running = true;
		game_speed = 1;
		current_y = 0;
		current_x = (TILES_WIDTH_COUNT - 4) / 2;
		timer = 0;
		score = 0;
		for (size_t y = 0; y < TILES_HEIGHT_COUNT; ++y) {
			for (size_t x = 0; x < TILES_WIDTH_COUNT; ++x) {
				non_empty_cell[y][x] = false;
			}
		}
		next_figure.setScale(NEXT_FIGURE_SCALE);
	}

	void drawField() {
		Line line;
		line.setColor(sf::Color(183, 183, 183));
		line.setThickness(LINE_THICKNESS);
		for (size_t i = 0; i < TILES_HEIGHT_COUNT; ++i) {
			line.setPosition(INDENT, i * TILE_SIZE + TOP_HEIGHT, SCREEN_WIDTH - INDENT - INFO_WIDTH, i * TILE_SIZE + TOP_HEIGHT);
			line.draw(window);
		}
		for (size_t i = 1; i < TILES_WIDTH_COUNT; ++i) {
			line.setPosition(INDENT + i * TILE_SIZE, SCREEN_HEIGHT - INDENT, INDENT + i * TILE_SIZE, TOP_HEIGHT);
			line.draw(window);
		}
		line.setColor(sf::Color::Black);

		line.setPosition(
			INDENT - LINE_THICKNESS,
			TOP_HEIGHT - LINE_THICKNESS,
			INDENT - LINE_THICKNESS,
			SCREEN_HEIGHT - INDENT);
		line.draw(window);

		line.setPosition(
			SCREEN_WIDTH - INDENT - INFO_WIDTH - LINE_THICKNESS,
			TOP_HEIGHT - LINE_THICKNESS,
			SCREEN_WIDTH - INDENT - INFO_WIDTH - LINE_THICKNESS,
			SCREEN_HEIGHT - INDENT);
		line.draw(window);

		line.setPosition(
			INDENT, 
			SCREEN_HEIGHT - INDENT, 
			SCREEN_WIDTH - INDENT - INFO_WIDTH, 
			SCREEN_HEIGHT - INDENT);
		line.draw(window);
	}

	void drawCells() {
		sf::RectangleShape cell;
		cell.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
		for (size_t y = 0; y < TILES_HEIGHT_COUNT; ++y) {
			for (size_t x = 0; x < TILES_WIDTH_COUNT; ++x) {
				if (non_empty_cell[y][x]) {
					cell.setPosition(sf::Vector2f(x * TILE_SIZE + INDENT, y * TILE_SIZE + TOP_HEIGHT));
					cell.setFillColor(cell_color[y][x]);
					window.draw(cell);
				}
			}
		}
	}

	void drawCurrentFigure() {
		current_figure.setPosition(INDENT + current_x * TILE_SIZE, TOP_HEIGHT + current_y * TILE_SIZE);
		current_figure.draw(window);
	}

	void drawNextFigure() {
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(INFO_WIDTH, INFO_WIDTH));
		rect.setPosition(sf::Vector2f(SCREEN_WIDTH - INFO_WIDTH - INDENT / 2, TOP_HEIGHT + INDENT));
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(LINE_THICKNESS);
		window.draw(rect);

		size_t left_cnt = next_figure.countLeftEmptyLines();
		size_t right_cnt = next_figure.countRightEmptyLines();
		size_t up_cnt = next_figure.countUpEmptyLines();
		size_t down_cnt = next_figure.countDownEmptyLines();

		double x_l, y_u;
		x_l = (INFO_WIDTH - (4 + left_cnt - right_cnt) * NEXT_FIGURE_SCALE * TILE_SIZE) / 2.;
		y_u = (INFO_WIDTH - (4 + up_cnt - down_cnt) * NEXT_FIGURE_SCALE * TILE_SIZE) / 2.;
		next_figure.setPosition(
			SCREEN_WIDTH - INFO_WIDTH - INDENT / 2. + x_l, 
			TOP_HEIGHT + INDENT + y_u);
		next_figure.draw(window);
	}

	void drawScore() {
		sf::Text text;
		sf::Font font;
		font.loadFromFile("Fonts/arialmt.ttf");

		text.setFont(font);
		text.setFillColor(sf::Color::Black);

		text.setString(L"Очки:");
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
		text.setPosition(
			SCREEN_WIDTH - INFO_WIDTH / 2 - INDENT / 2, 
			TOP_HEIGHT + INDENT + INFO_WIDTH + INDENT
		);
		window.draw(text);
		
		text.setString(std::to_string(score));
		sf::FloatRect scoreRect = text.getLocalBounds();
		text.setOrigin(scoreRect.left + scoreRect.width / 2, scoreRect.top + scoreRect.height / 2);
		text.setPosition(
			SCREEN_WIDTH - INFO_WIDTH / 2 - INDENT / 2,
			TOP_HEIGHT + INDENT + INFO_WIDTH + INDENT + SPACE_BETWEEN_TEXT + textRect.height
		);
		window.draw(text);
	}

	void checkForFilledLines() {
		size_t filled_lines_count = 0;
		for (int y = TILES_HEIGHT_COUNT - 1; y >= 0; --y) {
			bool is_filled_line = true;
			for (int x = 0; x < TILES_WIDTH_COUNT; ++x) {
				if (!non_empty_cell[y][x]) {
					is_filled_line = false;
					break;
				}
			}
			if (is_filled_line) {		// Удаляем все заполненные строки и сдвигаем строки выше вниз
				filled_lines_count++;
				for (int i = y; i >= 1; --i) {
					for (int j = 0; j < TILES_WIDTH_COUNT; ++j) {
						non_empty_cell[i][j] = non_empty_cell[i - 1][j];
						cell_color[i][j] = cell_color[i - 1][j];
					}
				}
				for (int j = 0; j < TILES_WIDTH_COUNT; ++j) {
					non_empty_cell[0][j] = false;
				}
				++y;
			}
		}
		score += filled_lines_count * filled_lines_count * 100;
		if (filled_lines_count > 0) {
			game_speed += SPEED_ADDING;
		}
	}

	void addFigureToCells() {
		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				if (!current_figure.cell[y][x]) {
					continue;
				}
				non_empty_cell[y + current_y][x + current_x] = true;
				cell_color[y + current_y][x + current_x] = current_figure.getColor();
			}
		}
		checkForFilledLines();
		current_figure = next_figure;
		current_figure.setScale(1);
		current_y = 0;
		current_x = (TILES_WIDTH_COUNT - 4) / 2;
		next_figure = Figure();
		next_figure.setScale(NEXT_FIGURE_SCALE);
		if (figureIntersects()) {
			game_running = false;
			return;
		}
	}

	bool figureIntersects() {
		for (int y = 0; y < 4; ++y) {
			for (int x = 0; x < 4; ++x) {
				if (!current_figure.cell[y][x]) {
					continue;
				}
				if (current_y + y < 0 || current_y + y >= TILES_HEIGHT_COUNT || 
					current_x + x < 0 || current_x + x >= TILES_WIDTH_COUNT ||
					non_empty_cell[current_y + y][current_x + x]) {
					return true;
				}
			}
		}
		return false;
	}

	void updateFigure(double time, sf::Event& event) {
		if (event.type == sf::Event::KeyPressed && game_running) {
			if (event.key.code == sf::Keyboard::Up) {
				current_figure.rotate();
				if (figureIntersects()) {
					for (int i = 0; i < 3; ++i) {
						current_figure.rotate();
					}
				}
			} else if (event.key.code == sf::Keyboard::Left) {
				current_x--;
				if (figureIntersects()) {
					current_x++;
				}
			} else if (event.key.code == sf::Keyboard::Right) {
				current_x++;
				if (figureIntersects()) {
					current_x--;
				}
			} else if (event.key.code == sf::Keyboard::Down) {
				current_y++;
				if (figureIntersects()) {
					current_y--;
					addFigureToCells();
					timer = 0;
				}
			}
		}
	}
	
	void draw(double time) {
		drawCells();
		drawField();
		drawScore();
		if (game_running) {
			timer += time;
			if (timer > 1. / game_speed) {
				current_y++;				// Опускаем фигуру на 1 вниз
				if (figureIntersects()) {
					current_y--;
					addFigureToCells();		// Если фигура пересекается, зафиксировать её
				}
				timer = 0;
			}
			drawCurrentFigure();
		}
		drawNextFigure();
	}
};
