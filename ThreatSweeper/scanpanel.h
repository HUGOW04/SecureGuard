#ifndef SCANPANEL_H
#define SCANPANEL_H

#include "widget.h"
#include "button.h"

class Scanpanel
{
public:
    Scanpanel(std::vector<Button>& buttons);
    void render();

private:
    std::vector<Button>& m_Buttons;
};

#endif // !
