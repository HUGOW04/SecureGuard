#include "loadimage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>


Loadimage::Loadimage(const std::string& imageName) : imageTexture(0) {
    if (imageName.empty()) {
        std::cerr << "Error: Empty image name provided" << std::endl;
        return;
    }

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);

    // Load the image
    unsigned char* data = stbi_load(imageName.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

    if (!data) {
        std::cerr << "Failed to load image: " << imageName << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        return;
    }

    // Generate and bind texture
    glGenTextures(1, &imageTexture);
    glBindTexture(GL_TEXTURE_2D, imageTexture);

    // Upload texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Set texture parameters (no mipmaps)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Clean up image data
    stbi_image_free(data);

    // Check for any errors
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
    else {
        std::cout << "Texture created successfully with ID: " << imageTexture << std::endl;
    }
}

void Loadimage::drawImage(float x, float y, float width, float height) {
    if (imageTexture == 0) {
        std::cerr << "Warning: Attempting to draw with invalid texture" << std::endl;
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageTexture);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(x - 4, y - 5);
    glTexCoord2f(1, 0); glVertex2f(x - 4 + width, y - 5);
    glTexCoord2f(1, 1); glVertex2f(x - 4 + width, y + height - 5);
    glTexCoord2f(0, 1); glVertex2f(x - 4, y + height - 5);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}