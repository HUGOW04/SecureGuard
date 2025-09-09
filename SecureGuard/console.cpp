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

std::vector<std::string> Console::wrapText(const std::string& text, float maxWidth) {
    std::vector<std::string> wrappedLines;
    if (!m_Font || !m_Font->isValid()) {
        wrappedLines.push_back(text);
        return wrappedLines;
    }

    std::string currentLine;
    std::string word;

    auto flushLine = [&]() {
        if (!currentLine.empty()) {
            wrappedLines.push_back(currentLine);
            currentLine.clear();
        }
        };

    auto breakLongWord = [&](const std::string& longWord) {
        std::string part;
        for (char c : longWord) {
            part += c;
            if (m_Font->getTextWidth(part, 1.0f) > maxWidth) {
                if (part.length() > 1) {
                    wrappedLines.push_back(part.substr(0, part.length() - 1));
                    part = part.back();
                }
            }
        }
        if (!part.empty()) {
            wrappedLines.push_back(part);
        }
        };

    for (char c : text) {
        if (c == '\n') {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            float testWidth = m_Font->getTextWidth(testLine, 1.0f);

            if (testWidth > maxWidth) {
                if (!currentLine.empty()) {
                    flushLine();
                }

                float wordWidth = m_Font->getTextWidth(word, 1.0f);
                if (wordWidth > maxWidth) {
                    breakLongWord(word);
                    word.clear();
                    continue;
                }
            }

            if (!word.empty()) {
                if (!currentLine.empty()) currentLine += " ";
                currentLine += word;
                word.clear();
            }

            if (c == '\n') flushLine();
        }
        else {
            word += c;
        }
    }

    // Handle final word
    if (!word.empty()) {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        float testWidth = m_Font->getTextWidth(testLine, 1.0f);

        if (testWidth > maxWidth) {
            if (!currentLine.empty()) flushLine();

            if (m_Font->getTextWidth(word, 1.0f) > maxWidth) {
                breakLongWord(word);
            }
            else {
                currentLine = word;
            }
        }
        else {
            if (!currentLine.empty()) currentLine += " ";
            currentLine += word;
        }
    }

    if (!currentLine.empty()) wrappedLines.push_back(currentLine);

    return wrappedLines;
}


void Console::render()
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

    /*
    if (m_ChangeBuffer != m_Buffer)
    {
        std::cout << "not the same" << std::endl;
        scrollbar_yPos = y_Pos + m_Height - scrollbar_height;
        m_ChangeBuffer = m_Buffer;
    }    
    */

    // Fixed rendering code - use full console width
    if (!(m_Font && m_Font->isValid()))
        return;

    std::vector<std::string> wrappedLines;
    for (const auto& line : m_Buffer) {
        std::vector<std::string> wrapped = wrapText(line, m_Width-20.0f);
        wrappedLines.insert(wrappedLines.end(), wrapped.begin(), wrapped.end());
    }

    const float lineHeight = 15.0f;
    int maxLines = static_cast<int>(m_Height / lineHeight);
    int totalLines = static_cast<int>(wrappedLines.size());

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

    float textX = x_Pos + 10.0f;      // left padding
    float textY = y_Pos + 10.0f;      // start at top + padding
    m_Font->setColor(1.0f, 1.0f, 1.0f, 1.0f);

    int endLine = startLine + linesToRender; // exclusive end

    for (int i = startLine; i < endLine; ++i) {
        m_Font->render(wrappedLines[i], textX, textY, 1.0f);
        textY += lineHeight;   // move down for next line
    }


}