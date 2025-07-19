#ifndef OVERVIEW_H
#define OVERVIEW_H


#include <iostream>
#include <vector>
#include "gl_includes.h"
#include "widget.h"

class Overview
{
public:
    Overview(std::vector<Widget>& widget);
    void render();
private:
    float x_Pos;
    float y_Pos;
    const float width = 130;
    const float height = 60;
    std::vector<Widget>& m_Widgets;
};

#endif