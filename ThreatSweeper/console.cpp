#include "console.h"

Console::Console(float x, float y, float width, float height, const std::vector<std::string>& buffers, Font* font)
    : x_Pos(x), y_Pos(y), m_Width(width), m_Height(height), m_Font(font), m_Buffer(buffers)
{
    scrollbar_width = 2.0f;
    scrollbar_height = 10.0f;
    scrollbar_xPos = x_Pos + m_Width - scrollbar_width;
    scrollbar_yPos = y_Pos;
}

void Console::setY(float y)
{
    if (y < y_Pos)
    {
        scrollbar_yPos = y_Pos;
    }
    else if (y + scrollbar_height > y_Pos + m_Height)
    {
        scrollbar_yPos = y_Pos + m_Height - scrollbar_height;
    }
    else
    {
        scrollbar_yPos = y;
    }
}

void Console::render()
{
    // --- Background ---
    glColor3f(0.3f, 0.3f, 0.3f);
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

    // Draw scrollbar background (track)
    glColor3f(0.1f, 0.1f, 0.1f); // Darker color for the background
    glBegin(GL_QUADS);
    glVertex2f(scrollbar_xPos, y_Pos);
    glVertex2f(scrollbar_xPos + scrollbar_width, y_Pos);
    glVertex2f(scrollbar_xPos + scrollbar_width, y_Pos + m_Height);
    glVertex2f(scrollbar_xPos, y_Pos + m_Height);
    glEnd();

    // Draw scrollbar (thumb)
    glColor3f(0.2f, 0.2f, 0.2f); // Lighter gray for the slider itself
    glBegin(GL_QUADS);
    glVertex2f(scrollbar_xPos, scrollbar_yPos);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos + scrollbar_height);
    glVertex2f(scrollbar_xPos, scrollbar_yPos + scrollbar_height);
    glEnd();

    // Detail points on corners
    glPointSize(6.0f);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2f(scrollbar_xPos, scrollbar_yPos);
    glVertex2f(scrollbar_xPos, scrollbar_yPos + scrollbar_height);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos + scrollbar_height);
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    // Border lines
    glLineWidth(8.0f);
    glBegin(GL_LINES);
    glVertex2f(scrollbar_xPos, scrollbar_yPos);
    glVertex2f(scrollbar_xPos, scrollbar_yPos + scrollbar_height);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos + scrollbar_height);
    glVertex2f(scrollbar_xPos, scrollbar_yPos);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos);
    glVertex2f(scrollbar_xPos, scrollbar_yPos + scrollbar_height);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos + scrollbar_height);
    glEnd();


    //slider
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(scrollbar_xPos, scrollbar_yPos);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos + scrollbar_height);
    glVertex2f(scrollbar_xPos, scrollbar_yPos + scrollbar_height);
    glEnd();

    
    glPointSize(6.0f);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2f(scrollbar_xPos, scrollbar_yPos);
    glVertex2f(scrollbar_xPos, scrollbar_yPos + scrollbar_height);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos + scrollbar_height);
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    
    glLineWidth(8.0f);
    glBegin(GL_LINES);
    glVertex2f(scrollbar_xPos, scrollbar_yPos);
    glVertex2f(scrollbar_xPos, scrollbar_yPos + scrollbar_height);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos + scrollbar_height);
    glVertex2f(scrollbar_xPos, scrollbar_yPos);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos);
    glVertex2f(scrollbar_xPos, scrollbar_yPos + scrollbar_height);
    glVertex2f(scrollbar_xPos + scrollbar_width, scrollbar_yPos + scrollbar_height);
    glEnd();

    std::string combinedLogs;
    for (const auto& line : m_Buffer) {
        combinedLogs += line + "\n";
    }

    if (!(m_Font && m_Font->isValid()))
        return;

    const float lineHeight = 15.0f;
    int maxLines = static_cast<int>(m_Height / lineHeight);
    int totalLines = static_cast<int>(m_Buffer.size());

    float scrollableHeight = m_Height - scrollbar_height;
    float scrollPercentage = 0.0f;

    if (scrollableHeight > 0.0f)
    {
        scrollPercentage = (scrollbar_yPos - y_Pos) / scrollableHeight;
        scrollPercentage = std::clamp(scrollPercentage, 0.0f, 1.0f);
    }

    int maxScrollableLines = std::max(0, totalLines - maxLines);
    int startLine = static_cast<int>(scrollPercentage * maxScrollableLines);
    startLine = std::clamp(startLine, 0, maxScrollableLines);

    int linesToRender = std::min(maxLines, totalLines - startLine);

    float textX = x_Pos + 5.0f; // some left padding

    m_Font->setColor(1.0f, 1.0f, 1.0f, 1.0f);

    int startIndex = startLine + linesToRender - 1;  // last line to render
    int endIndex = startLine;                         // first line to render

    float textY = y_Pos + m_Height - lineHeight;     // start from bottom

    // Render from bottom up (newest lines last in vector are on top)
    for (int i = startIndex; i >= endIndex; --i)
    {
        m_Font->render(m_Buffer[i], textX, textY, 1.0f);
        //std::cout << m_Buffer[i] << std::endl;
        textY -= lineHeight;  // move up to next line
    }






}