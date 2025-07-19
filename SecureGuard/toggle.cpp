#include "toggle.h"


Toggle::Toggle(float x, float y, float width, float height)
    : x_Pos(x), y_Pos(y), m_Width(width), m_Height(height)
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
    const int segments = 32;
    float right = x_Pos + m_Width;
    float top = y_Pos + m_Height;

    // --- Background ---
    if (m_Sate)
        glColor3f(0.059f, 0.463f, 0.431f); // ON
    else
        glColor3f(0.3f, 0.3f, 0.3f); // OFF

    // Center rectangle
    glBegin(GL_QUADS);
    glVertex2f(x_Pos + radius, y_Pos);
    glVertex2f(right - radius, y_Pos);
    glVertex2f(right - radius, top);
    glVertex2f(x_Pos + radius, top);
    glEnd();

    // Side rectangles
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
    auto drawCircle = [&](float cx, float cy, float startAngle) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= segments; ++i) {
            float theta = startAngle + (i * M_PI_2 / segments);
            glVertex2f(cx + cosf(theta) * radius, cy + sinf(theta) * radius);
        }
        glEnd();
        };

    drawCircle(x_Pos + radius, y_Pos + radius, M_PI);          // Top-left
    drawCircle(right - radius, y_Pos + radius, -M_PI_2);       // Top-right
    drawCircle(right - radius, top - radius, 0);               // Bottom-right
    drawCircle(x_Pos + radius, top - radius, M_PI_2);          // Bottom-left

    // --- Toggle Circle ---
    float circle_radius = m_Height * 0.4f; // scale to 80% of height
    float padding = (m_Height - circle_radius * 2.0f) / 2.0f;
    float circle_cx;

    if (m_Sate)
        circle_cx = x_Pos + m_Width - radius; // right
    else
        circle_cx = x_Pos + radius; // left

    float circle_cy = y_Pos + m_Height / 2.0f;

    glColor3f(0.95f, 0.95f, 0.95f); // Circle color
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(circle_cx, circle_cy);
    for (int i = 0; i <= segments; ++i) {
        float theta = i * 2.0f * M_PI / segments;
        glVertex2f(circle_cx + cosf(theta) * circle_radius, circle_cy + sinf(theta) * circle_radius);
    }
    glEnd();
}
