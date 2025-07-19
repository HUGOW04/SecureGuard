#include "performance.h"

Performance::Performance(const std::string* valuePtr, Font* valueFont,double x,double y)
    : m_ValueText(valuePtr),m_ValueFont(valueFont),x_Pos(x),y_Pos(y)
{

}

void Performance::drawCorner(float cx, float cy, float radius, int startAngle, int endAngle) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int angle = startAngle; angle <= endAngle; angle++) {
        float rad = angle * (float)M_PI / 180.0f;
        glVertex2f(cx + cosf(rad) * radius, cy + sinf(rad) * radius);
    }
    glEnd();
}

void Performance::drawRoundedRect(float x, float y, float width, float height, float radius) {
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

void Performance::render()
{

    int percent = std::stoi(*m_ValueText);

    // ----- Draw Background Rounded Rectangle First -----
    const float boxX = x_Pos;
    const float boxY = y_Pos;
    const float boxW = 140.0f;
    const float boxH = 140.0f;
    const float radius = 20.0f;

    glColor3f(0.2196f, 0.2549f, 0.3137f);  // Dark background
    drawRoundedRect(boxX, boxY, boxW, boxH, radius);

    // Center of the circle inside the box
    const float cx = boxX + boxW / 2.0f;
    const float cy = boxY + boxH / 2.0f;
    const float outerR = 60.0f;
    const float innerR = 50.0f;
    const int segments = 300;
    int greenSegments = segments * percent / 100.0f;

    // ----- Draw Gray Background Ring -----
    glColor3f(0.3f, 0.3f, 0.3f);  // Gray ring
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segments; ++i)
    {
        float angle = -2.0f * M_PI * i / segments + M_PI_2;
        float xOuter = cx + outerR * cos(angle);
        float yOuter = cy - outerR * sin(angle);
        float xInner = cx + innerR * cos(angle);
        float yInner = cy - innerR * sin(angle);
        glVertex2f(xOuter, yOuter);
        glVertex2f(xInner, yInner);
    }
    glEnd();

    // ----- Draw Green Foreground Ring -----
    glColor3f(0.2196f, 0.6f, 0.4f);  // Green ring
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= greenSegments; ++i)
    {
        float angle = -2.0f * M_PI * i / segments + M_PI_2;
        float xOuter = cx + outerR * cos(angle);
        float yOuter = cy - outerR * sin(angle);
        float xInner = cx + innerR * cos(angle);
        float yInner = cy - innerR * sin(angle);
        glVertex2f(xOuter, yOuter);
        glVertex2f(xInner, yInner);
    }
    glEnd();

    // ----- Draw Centered Text in Circle -----
    if (m_ValueFont && m_ValueFont->isValid())
    {
        m_ValueFont->setColor(1.0f, 1.0f, 1.0f, 0.9f);
        std::string text = *m_ValueText;

        // Estimate centering (adjust based on font metrics)
        float textX = cx - text.length() * 5.0f;  // ~11px width per char at scale 1
        float textY = cy;                  // Centered vertically

        m_ValueFont->render(text, textX, textY, 1.0f);
    }
}
