#ifndef CONSOLE_H
#define CONSOLE_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include "gl_includes.h"
#include "font.h"

class Console {
public:
    Console(float x, float y, float width, float height, const std::vector<std::string>& buffers, Font* font);
    void render();
    float getX() const { return scrollbar_xPos; }
    float getY() const { return scrollbar_yPos; }
    float getWidth() { return  scrollbar_width; }
    float getHeight() { return scrollbar_height; }
    void setY(float y);
private:
    std::vector<std::string> wrapText(const std::string& text, float maxWidth);
public:
    bool draggingScrollbar = false;
    float dragOffsetY = 0.0f;
    bool  moveSliderToBottom = false;

private:
    float x_Pos;
    float y_Pos;
    float m_Width;
    float m_Height;

    float scrollbar_xPos;
    float scrollbar_yPos;
    float scrollbar_width;
    float scrollbar_height;
    const std::vector<std::string>& m_Buffer;
    std::vector<std::string> m_BufferCopy; // copy of the buffer
    std::vector<std::string> m_ChangeBuffer;
    Font* m_Font;
};

#endif // !CONSOLE_H
