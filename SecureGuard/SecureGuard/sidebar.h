#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <iostream>
#include <vector>
#include "button.h"

class Sidebar
{
public:
    Sidebar(std::vector<Button>& buttons);
    void render();
private:
    std::vector<Button>& m_Buttons;
};

#endif // !SIDEBAR_H
