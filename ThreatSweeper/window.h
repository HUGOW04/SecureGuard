#ifndef WINDOW_H
#define WINDOW_H


#include <memory>
#include <iostream>

#include "gl_includes.h"
#include "overview.h"
#include "widget.h"
#include "sidebar.h"
#include "button.h"

class Window
{

public:
    Window(int width, int height, const char* title);
    ~Window();
    GLFWwindow* getWindow();
    void handleEvents();
    void initWindowContext();
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    void setupProjection();
private:
    void initGLFW();
    void createWindow();

private:
    GLFWwindow* m_Window;
    std::unique_ptr<Overview> m_Overview;
    std::unique_ptr<Sidebar> m_Sidebar;
    int m_Width;
    int m_Height;
    int m_Vsync = 1;
    const char* m_Title;

    // render
    bool overview = true;
    bool firewall = false;
    bool scan = false;
    
    // fonts
    std::unique_ptr<Font> m_Font;

    // buttons
    std::vector<Button> sideBarButtons;
    
    // widgets
    std::vector<Widget> overviewWidget;

};


#endif