#ifndef OVERVIEW_H
#define OVERVIEW_H

#include <GLFW/glfw3.h>

class Overview
{
public:
    Overview(GLFWwindow* window);
    void Render();
private:
    GLFWwindow* m_Window;
};

#endif