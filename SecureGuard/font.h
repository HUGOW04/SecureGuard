#ifndef FONT_H
#define FONT_H
#include <string>
#include "gl_includes.h"
#include "stb_truetype.h"

class Font {
public:
    Font(const std::string& ttfPath, float pixelSize);
    ~Font();
    void render(const std::string& text, float x, float y, float scale = 1.0f);
    void setColor(float r, float g, float b, float a = 1.0f);
    bool isValid() const { return valid; }
    float getTextWidth(const std::string& text, float scale = 1.0f);

private:
    GLuint textureID = 0;
    stbtt_bakedchar cdata[160]; // Characters 32-191 (160 characters total, includes © at 169)
    bool valid = false;
    float colorR = 1.0f, colorG = 1.0f, colorB = 1.0f, colorA = 1.0f;
};

#endif
