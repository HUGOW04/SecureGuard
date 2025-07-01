#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <vector>
#include <iostream>
#include "gl_includes.h"
#include "font.h"

class Button {
public:
    Button(float x, float y, std::string id, std::string buttonText, Font* font);
    void render();

    float getX() const { return x_Pos; }
    float getY() const { return y_Pos; }
    std::string getName() const { return m_ButtonName; }
    void setHovering(bool state) { isHovering = state; }
    bool getIsHovering() const { return isHovering; }

    static constexpr int Width() { return 130; }
    static constexpr int Height() { return 30; }

private:
    float x_Pos;
    float y_Pos;
    std::string m_ButtonName;
    std::string m_Text;
    bool isHovering = false;
    Font* m_Font;
};

#endif