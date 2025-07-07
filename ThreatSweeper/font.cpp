#include "font.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "gl_includes.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

Font::Font(const std::string& ttfPath, float pixelSize)
    : textureID(0), valid(false), colorR(1.0f), colorG(1.0f), colorB(1.0f), colorA(1.0f)
{

    std::ifstream file(ttfPath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open font file: " << ttfPath << std::endl;
        return;
    }

    std::vector<unsigned char> ttfBuffer((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());



    const int texWidth = 512;
    const int texHeight = 512;
    std::vector<unsigned char> bitmap(texWidth * texHeight);

    int result = stbtt_BakeFontBitmap(
        ttfBuffer.data(), 0, pixelSize,
        bitmap.data(), texWidth, texHeight,
        32, 96, cdata
    );

    if (result <= 0) {
        std::cerr << "Failed to bake font bitmap. Result: " << result << std::endl;
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Use GL_RED format; red channel contains glyph alpha
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, texWidth, texHeight, 0,
        GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.data());


    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set texture environment mode to modulate so color multiplies with texture alpha
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error creating font texture: " << error << std::endl;
        glDeleteTextures(1, &textureID);
        textureID = 0;
        return;
    }

    valid = true;
}

Font::~Font() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

void Font::setColor(float r, float g, float b, float a) {
    colorR = r;
    colorG = g;
    colorB = b;
    colorA = a;
}

float Font::getTextWidth(const std::string& text, float scale) {
    if (!valid || textureID == 0) {
        return 0.0f;
    }

    float totalWidth = 0.0f;
    float xpos = 0.0f;
    float ypos = 0.0f;

    for (char c : text) {
        if (c < 32 || c >= 128) continue;

        stbtt_aligned_quad q;
        float oldXpos = xpos;
        stbtt_GetBakedQuad(cdata, 512, 512, c - 32, &xpos, &ypos, &q, 1);

        // Calculate the advance for this character
        float charAdvance = xpos - oldXpos;
        totalWidth += charAdvance;
    }

    return totalWidth * scale;
}

void Font::render(const std::string& text, float x, float y, float scale) {
    if (!valid || textureID == 0) {
        std::cerr << "Font not valid for rendering" << std::endl;
        return;
    }

    // Save OpenGL state
    GLboolean texture2DEnabled = glIsEnabled(GL_TEXTURE_2D);
    GLboolean blendEnabled = glIsEnabled(GL_BLEND);
    GLint currentTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Enable blending for proper text rendering (use alpha from red channel)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set color to white with alpha (alpha comes from texture's red channel)
    glColor4f(colorR, colorG, colorB, colorA);

    glBegin(GL_QUADS);

    float xpos = x;
    float ypos = y;

    for (char c : text) {
        if (c < 32 || c >= 128) continue;

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(cdata, 512, 512, c - 32, &xpos, &ypos, &q, 1);

        // Scale the quad
        float x0 = q.x0 * scale;
        float y0 = q.y0 * scale;
        float x1 = q.x1 * scale;
        float y1 = q.y1 * scale;

        glTexCoord2f(q.s0, q.t0); glVertex2f(x0, y0);
        glTexCoord2f(q.s1, q.t0); glVertex2f(x1, y0);
        glTexCoord2f(q.s1, q.t1); glVertex2f(x1, y1);
        glTexCoord2f(q.s0, q.t1); glVertex2f(x0, y1);
    }

    glEnd();

    // Restore OpenGL state
    if (!blendEnabled) {
        glDisable(GL_BLEND);
    }

    if (texture2DEnabled) {
        glBindTexture(GL_TEXTURE_2D, currentTexture);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }
}
