#include "button.h"
#include "stb_easy_font.h"


Button::Button(float x, float y, float width, float height, std::string id, std::string buttonText, Font* font)
    : x_Pos(x), y_Pos(y),m_Width(width),m_Height(height), m_ButtonName(id), m_Text(buttonText), m_Font(font)
{
}

void Button::render()
{
    if (getIsHovering())
        glColor3f(0.4f, 0.3f, 0.3f); // Hover color
    else
        glColor3f(0.3f, 0.3f, 0.3f); // Default color

    // Draw button background
    glPointSize(3.0f);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2f(x_Pos, y_Pos);
    glVertex2f(x_Pos, y_Pos + m_Height);
    glVertex2f(x_Pos + m_Width, y_Pos);
    glVertex2f(x_Pos + m_Width, y_Pos + m_Height);
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    glLineWidth(5.0f);
    glBegin(GL_LINES);
    glVertex2f(x_Pos, y_Pos);
    glVertex2f(x_Pos, y_Pos + m_Height);
    glVertex2f(x_Pos + m_Width, y_Pos);
    glVertex2f(x_Pos + m_Width, y_Pos + m_Height);
    glVertex2f(x_Pos, y_Pos);
    glVertex2f(x_Pos + m_Width, y_Pos);
    glVertex2f(x_Pos, y_Pos + m_Height);
    glVertex2f(x_Pos + m_Width, y_Pos + m_Height);
    glEnd();

    glRectf(x_Pos, y_Pos, x_Pos + m_Width, y_Pos + m_Height);

    if (m_Font && m_Font->isValid())
    {
        float textX = x_Pos + 5.0f;
        float textY = y_Pos + Height() / 2 + 7.0f; // adjust for vertical alignment
        m_Font->setColor(1.0f, 1.0f, 1.0f, 1.0f);
        m_Font->render(m_Text, textX, textY, 1.0f);
    }
}

