#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include "gl_includes.h"

class Widget {
public:
    Widget(float x, float y,float width,float height, const std::string& text);
    void render();
private:
    float x_Pos;
    float y_Pos;
    float m_Width;
    float m_Height;
    std::string m_Text;
};

#endif // !WIDGET_H
