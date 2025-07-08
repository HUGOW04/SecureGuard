#ifndef SETTING_H
#define SETTING_H

#define _USE_MATH_DEFINES  
#include <cmath>        
#include <vector>

#include "font.h"
#include "toggle.h"
#include "gl_includes.h"

class Setting
{
public:
    Setting(Font* font,std::vector<Toggle>& toggle);
    void drawCorner(float cx, float cy, float radius, int startAngle, int endAngle);
    void drawRoundedRect(float x, float y, float width, float height, float radius);
    void render();
private:
    Font* m_Font;
    std::vector<Toggle>& m_Toggle;
};

#endif // !SETTING_H
