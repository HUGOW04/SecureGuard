#include "performance.h"

Performance::Performance(const std::string* valuePtr, Font* valueFont,double x,double y)
    : m_ValueText(valuePtr),m_ValueFont(valueFont),x_Pos(x),y_Pos(y)
{

}

void Performance::drawCorner(float cx, float cy, float radius, int startAngle, int endAngle) {
    const int segments = 32; // Higher resolution for smoother corners
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);

    // Convert to more precise angle calculation
    float startRad = startAngle * M_PI / 180.0f;
    float endRad = endAngle * M_PI / 180.0f;
    float angleStep = (endRad - startRad) / segments;

    for (int i = 0; i <= segments; i++) {
        float rad = startRad + i * angleStep;
        glVertex2f(cx + cosf(rad) * radius, cy + sinf(rad) * radius);
    }
    glEnd();
}

void Performance::drawRoundedRect(float x, float y, float width, float height, float radius) {
    // Temporarily disable anti-aliasing for solid shapes to avoid seams
    bool blendWasEnabled = glIsEnabled(GL_BLEND);
    bool polyWasEnabled = glIsEnabled(GL_POLYGON_SMOOTH);
    bool lineWasEnabled = glIsEnabled(GL_LINE_SMOOTH);

    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    if (!blendWasEnabled) glDisable(GL_BLEND);

    float right = x + width;
    float bottom = y + height;

    // Draw as a single triangle fan for seamless rendering
    glBegin(GL_TRIANGLES);

    // Main rectangle (center part)
    // Top rectangle
    glVertex2f(x + radius, y);
    glVertex2f(right - radius, y);
    glVertex2f(x + radius, bottom);

    glVertex2f(right - radius, y);
    glVertex2f(right - radius, bottom);
    glVertex2f(x + radius, bottom);

    // Left rectangle
    glVertex2f(x, y + radius);
    glVertex2f(x + radius, y + radius);
    glVertex2f(x, bottom - radius);

    glVertex2f(x + radius, y + radius);
    glVertex2f(x + radius, bottom - radius);
    glVertex2f(x, bottom - radius);

    // Right rectangle
    glVertex2f(right - radius, y + radius);
    glVertex2f(right, y + radius);
    glVertex2f(right - radius, bottom - radius);

    glVertex2f(right, y + radius);
    glVertex2f(right, bottom - radius);
    glVertex2f(right - radius, bottom - radius);

    glEnd();

    // Draw rounded corners with higher precision
    drawCorner(x + radius, y + radius, radius, 180, 270);       // Top-left
    drawCorner(right - radius, y + radius, radius, 270, 360);   // Top-right
    drawCorner(right - radius, bottom - radius, radius, 0, 90); // Bottom-right
    drawCorner(x + radius, bottom - radius, radius, 90, 180);   // Bottom-left

    // Restore previous OpenGL state
    if (polyWasEnabled) glEnable(GL_POLYGON_SMOOTH);
    if (lineWasEnabled) glEnable(GL_LINE_SMOOTH);
    if (blendWasEnabled) glEnable(GL_BLEND);
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

    // NOW enable anti-aliasing for the circles only
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // Center of the circle inside the box
    const float cx = boxX + boxW / 2.0f;
    const float cy = boxY + boxH / 2.0f;
    const float outerR = 60.0f;
    const float innerR = 50.0f;

    // Increased segments for smoother circles
    const int segments = 720;
    int progressSegments = segments * percent / 100.0f;

    // ----- Draw Gray Background Ring -----
    glColor4f(0.3f, 0.3f, 0.3f, 1.0f);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < segments; ++i)
    {
        float angle1 = -2.0f * M_PI * i / segments + M_PI_2;
        float angle2 = -2.0f * M_PI * (i + 1) / segments + M_PI_2;

        float x1Outer = cx + outerR * cos(angle1);
        float y1Outer = cy - outerR * sin(angle1);
        float x1Inner = cx + innerR * cos(angle1);
        float y1Inner = cy - innerR * sin(angle1);

        float x2Outer = cx + outerR * cos(angle2);
        float y2Outer = cy - outerR * sin(angle2);
        float x2Inner = cx + innerR * cos(angle2);
        float y2Inner = cy - innerR * sin(angle2);

        // First triangle
        glVertex2f(x1Outer, y1Outer);
        glVertex2f(x1Inner, y1Inner);
        glVertex2f(x2Outer, y2Outer);

        // Second triangle
        glVertex2f(x1Inner, y1Inner);
        glVertex2f(x2Inner, y2Inner);
        glVertex2f(x2Outer, y2Outer);
    }
    glEnd();

    // ----- Draw Colored Foreground Ring -----
    if (percent < 50)
    {
        glColor4f(0.2196f, 0.6f, 0.4f, 1.0f);  // Green
    }
    else if (percent >= 50 && percent < 75)
    {
        glColor4f(0.95f, 0.5f, 0.2f, 1.0f);    // Orange
    }
    else // percent >= 75
    {
        glColor4f(0.9f, 0.2f, 0.2f, 1.0f);     // Red
    }

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < progressSegments; ++i)
    {
        float angle1 = -2.0f * M_PI * i / segments + M_PI_2;
        float angle2 = -2.0f * M_PI * (i + 1) / segments + M_PI_2;

        float x1Outer = cx + outerR * cos(angle1);
        float y1Outer = cy - outerR * sin(angle1);
        float x1Inner = cx + innerR * cos(angle1);
        float y1Inner = cy - innerR * sin(angle1);

        float x2Outer = cx + outerR * cos(angle2);
        float y2Outer = cy - outerR * sin(angle2);
        float x2Inner = cx + innerR * cos(angle2);
        float y2Inner = cy - innerR * sin(angle2);

        // First triangle
        glVertex2f(x1Outer, y1Outer);
        glVertex2f(x1Inner, y1Inner);
        glVertex2f(x2Outer, y2Outer);

        // Second triangle
        glVertex2f(x1Inner, y1Inner);
        glVertex2f(x2Inner, y2Inner);
        glVertex2f(x2Outer, y2Outer);
    }
    glEnd();

    // Optional: Add smooth end cap for progress ring
    if (progressSegments > 0 && progressSegments < segments)
    {
        float endAngle = -2.0f * M_PI * progressSegments / segments + M_PI_2;
        float capX = cx + (outerR + innerR) / 2.0f * cos(endAngle);
        float capY = cy - (outerR + innerR) / 2.0f * sin(endAngle);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(capX, capY);
        float capRadius = (outerR - innerR) / 2.0f;
        for (int i = 0; i <= 16; ++i)
        {
            float capAngle = 2.0f * M_PI * i / 16;
            glVertex2f(capX + capRadius * cos(capAngle),
                capY + capRadius * sin(capAngle));
        }
        glEnd();
    }

    // ----- Draw Centered Text in Circle -----
    if (m_ValueFont && m_ValueFont->isValid())
    {
        m_ValueFont->setColor(1.0f, 1.0f, 1.0f, 0.9f);
        std::string text = *m_ValueText;
        float textX = cx - text.length() * 5.0f;
        float textY = cy;
        m_ValueFont->render(text, textX, textY, 1.0f);
    }

    // Restore OpenGL state
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}
