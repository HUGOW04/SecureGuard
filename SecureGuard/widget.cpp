#include "widget.h"

Widget::Widget(float x, float y, float width, float height,
    const std::string* valuePtr, const std::string& labelText,
    Font* valueFont, Font* labelFont)
    : x_Pos(x), y_Pos(y), m_Width(width), m_Height(height),
    m_ValueText(valuePtr), m_LabelText(labelText),
    m_ValueFont(valueFont), m_LabelFont(labelFont)
{
}

void Widget::render()
{
    // --- Background ---
    glColor3f(0.2196f, 0.2549f, 0.3137f);
    glBegin(GL_QUADS);
    glVertex2f(x_Pos, y_Pos);
    glVertex2f(x_Pos + m_Width, y_Pos);
    glVertex2f(x_Pos + m_Width, y_Pos + m_Height);
    glVertex2f(x_Pos, y_Pos + m_Height);
    glEnd();

    // --- Borders (points) ---
    glPointSize(6.0f);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2f(x_Pos, y_Pos);
    glVertex2f(x_Pos, y_Pos + m_Height);
    glVertex2f(x_Pos + m_Width, y_Pos);
    glVertex2f(x_Pos + m_Width, y_Pos + m_Height);
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    // --- Borders (lines) ---
    glLineWidth(8.0f);
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


    if (m_ValueFont && m_ValueFont->isValid() && m_ValueText)
    {
        float centerX = x_Pos + 35;
        float valueY = y_Pos + m_Height - 20.0f;

        m_ValueFont->setColor(1.0f, 1.0f, 1.0f, 0.9f);
        m_ValueFont->render(*m_ValueText, centerX, valueY, 1.0f);
    }

    if (m_LabelFont && m_LabelFont->isValid())
    {
        float labelX = x_Pos + 35;
        float labelY = y_Pos + m_Height * 0.35f;

        m_LabelFont->setColor(1.0f, 1.0f, 1.0f, 1.0f);
        m_LabelFont->render(m_LabelText, labelX, labelY, 1.0f);
    }
}


