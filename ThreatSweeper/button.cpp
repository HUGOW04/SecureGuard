#include "button.h"
#include "stb_easy_font.h"


Button::Button(float x, float y, float width, float height, std::string id, std::string buttonText, Font* font, Loadimage* img)
    : x_Pos(x), y_Pos(y), m_Width(width), m_Height(height), m_ButtonName(id), m_Text(buttonText), m_Font(font), m_Img(img)
{

}

void Button::render()
{
    if (selected)
    {
        glColor3f(0.059f, 0.463f, 0.431f);
    }
    else
    {
        if (getIsHovering())
            glColor3f(0.2196f, 0.2549f, 0.3137f); // Slightly brighter teal on hover
        else
            glColor3f(0.07f, 0.09f, 0.15f); // Normal teal

    }
    
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
            float imageX = x_Pos + padding-5;
            float imageY = y_Pos + (m_Height - imageSize) / 2.0f + 5.0f;
            
 
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Återställ färg så att bilden inte färgas
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

