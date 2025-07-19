#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "gl_includes.h"
#include "font.h"
#include <string>
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 (M_PI / 2.0)
#endif

class Performance
{
public:
	Performance(const std::string* valuePtr, Font* valueFont, double x, double y);
	void render();
	void drawCorner(float cx, float cy, float radius, int startAngle, int endAngle);
	void drawRoundedRect(float x, float y, float width, float height, float radius);
private:
	const std::string* m_ValueText; // <-- pointer to value that can change
	Font* m_ValueFont;
	double x_Pos, y_Pos;
};

#endif // !PERFORMANCE_H
