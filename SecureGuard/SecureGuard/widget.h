#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include "gl_includes.h"
#include "font.h"
#include "loadimage.h"

class Widget {
public:
    Widget(float x, float y, float width, float height,
        const std::string* valuePtr, const std::string& labelText,
        Font* valueFont, Font* labelFont, Loadimage* img);

    void render();

private:
    float x_Pos;
    float y_Pos;
    float m_Width;
    float m_Height;

    const std::string* m_ValueText; // <-- pointer to value that can change
    std::string m_LabelText;

    Font* m_ValueFont;
    Font* m_LabelFont;
    Loadimage* m_Img;
};


#endif
