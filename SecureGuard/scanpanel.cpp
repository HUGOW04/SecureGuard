#include "scanpanel.h"

Scanpanel::Scanpanel(std::vector<Button>& buttons, std::vector<Console>& consoles)
    : m_Buttons(buttons),m_Consoles(consoles)
{
}

void Scanpanel::render()
{

    for (auto& button : m_Buttons)
    {
        button.render();
    }

    for (auto& console : m_Consoles)
    {
        console.render();
    }

}

