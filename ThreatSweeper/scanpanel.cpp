#include "scanpanel.h"

Scanpanel::Scanpanel(std::vector<Button>& buttons, std::vector<Console>& consoles, std::vector<Toggle>& toggle)
    : m_Buttons(buttons),m_Consoles(consoles),m_Toggle(toggle)
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

    for (auto& toggle : m_Toggle)
    {
        toggle.render();
    }
}

