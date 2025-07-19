#include "scanpanel.h"

Scanpanel::Scanpanel(std::vector<Button>& buttons, std::vector<Console>& consoles)
    : m_Buttons(buttons),m_Consoles(consoles)
{
}

void Scanpanel::render()
{
    // Line 1
    glColor3f(0.2196f, 0.6f, 0.4f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(260.0f, 81.0f);
    glVertex2f(260.0f + 620.0f, 81.0f);
    glEnd();

    for (auto& button : m_Buttons)
    {
        button.render();
    }

    for (auto& console : m_Consoles)
    {
        console.render();
    }

}

