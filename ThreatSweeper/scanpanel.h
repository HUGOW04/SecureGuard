#ifndef SCANPANEL_H
#define SCANPANEL_H

#include "widget.h"
#include "button.h"
#include "console.h"
#include "toggle.h"

class Scanpanel
{
public:
    Scanpanel(std::vector<Button>& buttons,std::vector<Console>& consoles, std::vector<Toggle>& toggle);
    void render();

private:
    std::vector<Button>& m_Buttons;
    std::vector<Console>& m_Consoles;
    std::vector<Toggle>& m_Toggle;
};

#endif // !
