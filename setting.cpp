#include "setting.h"

Setting::Setting(Font* font, std::vector<Toggle>& toggle)
    :  m_Font(font), m_Toggle(toggle)
{

}

void Setting::drawCorner(float cx, float cy, float radius, int startAngle, int endAngle) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int angle = startAngle; angle <= endAngle; angle++) {
        float rad = angle * (float)M_PI / 180.0f;
        glVertex2f(cx + cosf(rad) * radius, cy + sinf(rad) * radius);
    }
    glEnd();
}

void Setting::drawRoundedRect(float x, float y, float width, float height, float radius) {
    float right = x + width;
    float bottom = y + height;

    // Center and sides
    glBegin(GL_QUADS);
    glVertex2f(x + radius, y);
    glVertex2f(right - radius, y);
    glVertex2f(right - radius, bottom);
    glVertex2f(x + radius, bottom);

    glVertex2f(x, y + radius);
    glVertex2f(x + radius, y + radius);
    glVertex2f(x + radius, bottom - radius);
    glVertex2f(x, bottom - radius);

    glVertex2f(right - radius, y + radius);
    glVertex2f(right, y + radius);
    glVertex2f(right, bottom - radius);
    glVertex2f(right - radius, bottom - radius);
    glEnd();

    // Rounded corners
    drawCorner(x + radius, y + radius, radius, 180, 270);       // Top-left
    drawCorner(right - radius, y + radius, radius, 270, 360);   // Top-right
    drawCorner(right - radius, bottom - radius, radius, 0, 90); // Bottom-right
    drawCorner(x + radius, bottom - radius, radius, 90, 180);   // Bottom-left
}

void Setting::render()
{
	glColor3f(0.2196f, 0.2549f, 0.3137f);
    drawRoundedRect(260.0f, 120.0f, 620.0f, 330.0f, 20.0f);

    for (auto& toggle : m_Toggle)
    {
        toggle.render();
    }

    if (m_Font && m_Font->isValid())
    {
        float labelX =  260.0f;
        float labelY = 200.0f;

        m_Font->setColor(1.0f, 1.0f, 1.0f, 1.0f);
        m_Font->render("hi", labelX, labelY, 1.0f);
    }
}