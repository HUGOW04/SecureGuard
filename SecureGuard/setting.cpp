#include "setting.h"

Setting::Setting(Font* title, Font* text, std::vector<std::string>& settingsTitle, std::vector<std::string>& settingsDescriptions, std::vector<Toggle>& toggle)
    :  m_FontTitle(title), m_FontText(text), m_SettingTitles(settingsTitle),m_SettingDescriptions(settingsDescriptions),m_Toggles(toggle)
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
    // Line 1
    glColor3f(0.2196f, 0.6f, 0.4f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(260.0f, 81.0f);
    glVertex2f(260.0f + 620.0f, 81.0f);
    glEnd();

	glColor3f(0.2196f, 0.2549f, 0.3137f);
    drawRoundedRect(260.0f, 120.0f, 620.0f, 330.0f, 20.0f);

    for (auto& toggle : m_Toggles)
    {
        toggle.render();
    }

    if (m_FontTitle && m_FontTitle->isValid())
    {

        float labelX =  280.0f;
        float labelY = 120.0f;
        m_FontTitle->setColor(1.0f, 1.0f, 1.0f, 1.0f);
        for (auto& title : m_SettingTitles)
        {
            labelY += 50;
            m_FontTitle->render(title, labelX, labelY, 1.0f);
        }

    }
    if (m_FontText && m_FontText->isValid())
    {

        float labelX = 280.0f;
        float labelY = 140.0f;
        m_FontText->setColor(0.8f, 0.8f, 0.8f, 1.0f);
        for (auto& title : m_SettingDescriptions)
        {
            labelY += 50;
            m_FontText->render(title, labelX, labelY, 1.0f);
        }

    }

}