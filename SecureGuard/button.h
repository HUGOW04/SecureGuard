#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "font.h"
#include "gl_includes.h"
#include "loadimage.h"

struct Color {
    float r, g, b;
    Color(float red = 1.0f, float green = 1.0f, float blue = 1.0f)
        : r(red), g(green), b(blue) {
    }
};

class Button {
public:
    Button(float x, float y, float width, float height,
        const std::string& id, const std::string& buttonText,
        Font* font, Loadimage* img,
        Color normalColor = Color(0.07f, 0.09f, 0.15f),
        Color hoverColor = Color(0.2196f, 0.2549f, 0.3137f),
        Color selectedColor = Color(0.059f, 0.463f, 0.431f));

    void render();

    float getX() const { return x_Pos; }
    float getY() const { return y_Pos; }
    float getWidth() const { return m_Width; }
    float getHeight() const { return m_Height; }
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

    Color m_NormalColor;
    Color m_HoverColor;
    Color m_SelectedColor;
};

#endif
