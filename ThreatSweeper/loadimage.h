#ifndef LOADIMAGE_H
#define LOADIMAGE_H

#include <iostream>
#include <string>
#include "gl_includes.h"

class Loadimage
{
public:
	Loadimage(const std::string& imageName);
	void drawImage(float x, float y, float width, float height);
private:
	GLuint imageTexture;
};

#endif // !LOADIMAGE_H
