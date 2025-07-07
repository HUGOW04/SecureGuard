#include "toggle.h"


Toggle::Toggle(float x, float y, float width, float height, Font* font)
    : x_Pos(x), y_Pos(y), m_Width(width), m_Height(height), m_Font(font)
{
   
}

void Toggle::setState(bool state)
{
    m_Sate = state;
}

bool Toggle::getState()
{
    return m_Sate;
}

void Toggle::render()
{
    const float radius = m_Height / 2.0f;
    const int segments = 16;
    float right = x_Pos + m_Width;
    float top = y_Pos + m_Height;

    // --- Rounded background ---
    glColor3f(0.3f, 0.3f, 0.3f);

    // Center rectangle
    glBegin(GL_QUADS);
    glVertex2f(x_Pos + radius, y_Pos);
    glVertex2f(right - radius, y_Pos);
    glVertex2f(right - radius, top);
    glVertex2f(x_Pos + radius, top);
    glEnd();

    // Left and right rectangles
    glBegin(GL_QUADS);
    glVertex2f(x_Pos, y_Pos + radius);
    glVertex2f(x_Pos + radius, y_Pos + radius);
    glVertex2f(x_Pos + radius, top - radius);
    glVertex2f(x_Pos, top - radius);

    glVertex2f(right - radius, y_Pos + radius);
    glVertex2f(right, y_Pos + radius);
    glVertex2f(right, top - radius);
    glVertex2f(right - radius, top - radius);
    glEnd();

    // Corner circles
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x_Pos + radius, y_Pos + radius); // center
    for (int i = 0; i <= segments; ++i) {
        float theta = M_PI + (i * M_PI_2 / segments);
        glVertex2f(x_Pos + radius + cosf(theta) * radius, y_Pos + radius + sinf(theta) * radius);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(right - radius, y_Pos + radius);
    for (int i = 0; i <= segments; ++i) {
        float theta = -M_PI_2 + (i * M_PI_2 / segments);
        glVertex2f(right - radius + cosf(theta) * radius, y_Pos + radius + sinf(theta) * radius);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(right - radius, top - radius);
    for (int i = 0; i <= segments; ++i) {
        float theta = 0 + (i * M_PI_2 / segments);
        glVertex2f(right - radius + cosf(theta) * radius, top - radius + sinf(theta) * radius);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x_Pos + radius, top - radius);
    for (int i = 0; i <= segments; ++i) {
        float theta = M_PI_2 + (i * M_PI_2 / segments);
        glVertex2f(x_Pos + radius + cosf(theta) * radius, top - radius + sinf(theta) * radius);
    }
    glEnd();

   

    // Detail points on corners
    glPointSize(20.0f);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    if (m_Sate)
    {
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex2f(x_Pos + m_Width - 15, y_Pos + 15);
    }
    else
    {
        glColor3f(0.2f, 0.2f, 0.2f);
        glVertex2f(x_Pos+15, y_Pos+15);
    }
    glEnd();
    glDisable(GL_POINT_SMOOTH);
    if (m_Font && m_Font->isValid())
    {
        float textY = y_Pos + m_Height / 2 + 5.0f; 

        if (m_Sate)
        {
            
            float textX_on = x_Pos + 5.0f;
            m_Font->setColor(0.0f, 1.0f, 0.0f, 0.8f);
            m_Font->render("ON", textX_on, textY, 1.0f);
        }
        else
        {
            float textWidth = 8.0f * 3;
            float textX_off = x_Pos + m_Width - textWidth - 5.0f;

            m_Font->setColor(1.0f, 0.0f, 0.0f, 0.8f);
            m_Font->render("OFF", textX_off, textY, 1.0f);
        }
    }

}
