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


private:
    GLuint textureID = 0;
    stbtt_bakedchar cdata[96]; // ASCII 32..127
    bool valid = false;
    float colorR = 1.0f, colorG = 1.0f, colorB = 1.0f, colorA = 1.0f;
};

#endif