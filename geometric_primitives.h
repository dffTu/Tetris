#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

const double PI = 3.141592653589793238463;

class Line {
private:
	sf::RectangleShape rect;
	double length;
	double thickness;
	double X1, Y1, X2, Y2;

public:
	Line() = default;

	Line(double x1, double y1, double x2, double y2) {
		thickness = 3;
		setPosition(x1, y1, x2, y2);
	}

	Line(sf::Vector2f& v1, sf::Vector2f& v2) : Line(v1.x, v1.y, v2.x, v2.y) {};

	void setPosition(double x1, double y1, double x2, double y2) {
		X1 = x1;
		Y1 = y1;
		X2 = x2;
		Y2 = y2;
		if (X1 > X2) {
			std::swap(X1, X2);
			std::swap(Y1, Y2);
		}
		length = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}

	void setColor(sf::Color color) {
		rect.setFillColor(color);
	}

	void setThickness(double width) {
		thickness = width;
	}

	void draw(sf::RenderWindow& window) {
		rect.setSize(sf::Vector2f(thickness, length));
		double alpha = acos((Y2 - Y1) / length) * 180. / PI;
		rect.setRotation(-alpha);
		rect.setPosition(sf::Vector2f(X1, Y1));
		window.draw(rect);
	}
};
