#ifndef SCANPANEL_H
#define SCANPANEL_H

#define _USE_MATH_DEFINES  
#include <cmath>  

#include "widget.h"
#include "button.h"
#include "console.h"
#include "toggle.h"

class Scanpanel
{
public:
    Scanpanel(std::vector<Button>& buttons,std::vector<Console>& consoles);
    void render();
private:
    void drawCorner(float cx, float cy, float radius, int startAngle, int endAngle);
    void drawRoundedRect(float x, float y, float width, float height, float radius);

private:
    std::vector<Button>& m_Buttons;
    std::vector<Console>& m_Consoles;
};

#endif // !
