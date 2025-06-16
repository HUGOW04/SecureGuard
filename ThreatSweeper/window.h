#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

#include "overview.h"
#include "proxy.h"

class Window
{

public:
    Window(int width, int height, const char* title);
    ~Window();
    GLFWwindow* getWindow();
    void handleEvents();
    void initWindowContext();
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
    void initGLFW();
    void createWindow();

private:
    GLFWwindow* m_Window;
    std::unique_ptr<Overview> m_Overview;
    std::unique_ptr<Proxy> m_Proxy;
    int m_Width;
    int m_Height;
    int m_Vsync = 1;
    const char* m_Title;
};


#endif
