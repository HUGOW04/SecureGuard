#include "scanpanel.h"

Scanpanel::Scanpanel( std::vector<Button>& buttons)
    : m_Buttons(buttons)
{
}

void Scanpanel::render()
{
    for (auto& button : m_Buttons)
    {
        button.render();
    }


}

