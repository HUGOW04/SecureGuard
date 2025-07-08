#ifndef TOGGLE_H
#define TOGGLE_H

#define _USE_MATH_DEFINES
#include <cmath>
#include "gl_includes.h"
#include "font.h"

class Toggle
{
public:
	Toggle(float x, float y, float width, float height);
	void render();
	float getX() { return x_Pos; }
	float getY() { return y_Pos; }
	float getWidth() { return m_Width; }
	float getHeight() { return m_Height; }
	void setState(bool state);
	bool getState();
private:
	float x_Pos;
	float y_Pos;
	float m_Width;
	float m_Height;
	bool m_Sate = false; // on/off
};

#endif // !TOGGLE_H
