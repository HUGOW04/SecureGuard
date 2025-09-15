#include "button.h"
#include "stb_easy_font.h"

Button::Button(float x, float y, float width, float height,
    const std::string& id, const std::string& buttonText,
    Font* font, Loadimage* img,
    Color normalColor, Color hoverColor, Color selectedColor)
    : x_Pos(x), y_Pos(y), m_Width(width), m_Height(height),
    m_ButtonName(id), m_Text(buttonText), m_Font(font), m_Img(img),
    m_NormalColor(normalColor), m_HoverColor(hoverColor), m_SelectedColor(selectedColor)
{
}


void Button::render()
{
    // pick color depending on state
    if (selected)
        glColor3f(m_SelectedColor.r, m_SelectedColor.g, m_SelectedColor.b);
    else if (getIsHovering())
        glColor3f(m_HoverColor.r, m_HoverColor.g, m_HoverColor.b);
    else
        glColor3f(m_NormalColor.r, m_NormalColor.g, m_NormalColor.b);

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
        float textX;
        float textY;

        if (m_Img)
        {
            float imageSize = 30.0f;
            float padding = 10.0f;
            float imageX = x_Pos + padding - 5;
            float imageY = y_Pos + (m_Height - imageSize) / 2.0f + 5.0f;

            glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // reset color for image
            m_Img->drawImage(imageX, imageY, imageSize, imageSize);

            textX = imageX + imageSize + padding;
            textY = y_Pos + m_Height / 2 + 7.0f;
        }
        else
        {
            textX = x_Pos + 5.0f;
            textY = y_Pos + m_Height / 2 + 7.0f;
        }

        m_Font->setColor(1.0f, 1.0f, 1.0f, 1.0f);
        m_Font->render(m_Text, textX, textY, 1.0f);
    }
}
