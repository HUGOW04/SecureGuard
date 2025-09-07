#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <vector>
#include <iostream>
#include "font.h"
#include <cmath>
#include "gl_includes.h"
#include "loadimage.h"

class Button {
public:
    Button(float x, float y,float width,float height, std::string id, std::string buttonText, Font* font, Loadimage* img);
    void render();

    float getX() const { return x_Pos; }
    float getY() const { return y_Pos; }
    float getWidth() { return m_Width; }
    float getHeight() { return m_Height; }
    std::string getName() const { return m_ButtonName; }
    void setHovering(bool state) { isHovering = state; }
    void setSelect(bool state) { selected = state; }
    bool getIsHovering() const { return isHovering; }


private:
    float x_Pos;
    float y_Pos;
    float m_Width;
    float m_Height;
    std::string m_ButtonName;
    std::string m_Text;
    bool isHovering = false;
    bool selected = false;
    Font* m_Font;
    Loadimage* m_Img;
};

#endif