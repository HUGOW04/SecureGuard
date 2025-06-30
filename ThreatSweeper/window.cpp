#include "window.h"

Window::Window(int width, int height, const char* title)
    : m_Width(width), m_Height(height), m_Title(title)
{
    initGLFW();
    createWindow();
    initWindowContext();

    m_Font = std::make_unique<Font>("Lato-LightItalic.ttf", 20.0f);

    sideBarButtons.push_back(Button(10, 10, "Overview", "overview", m_Font.get()));
    sideBarButtons.push_back(Button(10, 60, "Firewall", "firewall", m_Font.get()));
    sideBarButtons.push_back(Button(10, 110, "Scan", "scan", m_Font.get()));

    overviewWidget.push_back(Widget(200, 10,100,50,"gell"));
    overviewWidget.push_back(Widget(200, 300, 100, 50, "gell"));

    m_Sidebar = std::make_unique<Sidebar>(sideBarButtons);
    m_Overview = std::make_unique<Overview>(overviewWidget);
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

GLFWwindow* Window::getWindow()
{
    return m_Window;
}

void Window::initGLFW()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize glfw" << std::endl;
    }
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void Window::createWindow()
{
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
    if (!m_Window)
    {
        std::cout << "Failed to create glfwWindow" << std::endl;
    }

    glfwSetWindowUserPointer(m_Window, this);

}

void Window::initWindowContext()
{
    glfwMakeContextCurrent(m_Window); 
    glfwSetKeyCallback(m_Window, key_callback);
    glfwSetCursorPosCallback(m_Window, cursor_position_callback);
    glfwSetMouseButtonCallback(m_Window, mouse_callback);
    // Enable/Disable m_Vsync: 1 for VSync on, 0 for VSync off
    glfwSwapInterval(m_Vsync ? 1 : 0);

    setupProjection();
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!win) return;

    float mouseX = static_cast<float>(xpos);
    float mouseY = static_cast<float>(ypos);

    for (auto& b : win->sideBarButtons)
    {
        bool hoverBefore = b.getIsHovering();
        if (mouseX >= b.getX() && mouseX <= b.getX() + b.Width() &&
            mouseY >= b.getY() && mouseY <= b.getY() + b.Height())
        {
            b.setHovering(true);
        }
        else
        {
            b.setHovering(false);
        }
    }
}




void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action != GLFW_PRESS) return; // Handle only press

    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!win) return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    float mouseX = static_cast<float>(xpos);
    float mouseY = static_cast<float>(ypos);

    for (auto& b : win->sideBarButtons)
    {
        if (mouseX >= b.getX() && mouseX <= b.getX() + b.Width() &&
            mouseY >= b.getY() && mouseY <= b.getY() + b.Height())
        {
            if (b.getName() == "Overview")
            {
                win->overview = true;
                win->firewall = false;
                win->scan = false;
                std::cout << "Overview selected" << std::endl;
            }
            else if (b.getName() == "Firewall")
            {
                win->overview = false;
                win->firewall = true;
                win->scan = false;
                std::cout << "Firewall selected" << std::endl;
            }
            else if (b.getName() == "Scan")
            {
                win->overview = false;
                win->firewall = false;
                win->scan = true;
                std::cout << "Scan selected" << std::endl;
            }
        }
    }
}



void Window::setupProjection()
{
    glViewport(0, 0, m_Width, m_Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, m_Width, m_Height, 0.0, -1.0, 1.0);  // Flip Y here
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Window::handleEvents()
{

    while (!glfwWindowShouldClose(m_Window))
    {

        m_Sidebar->render(m_Window, m_Width, m_Height);

        if (overview)
        {
           m_Overview->render();
        }
        //std::cout << m_Width << " " << m_Height << std::endl;
        glfwSwapBuffers(m_Window);   // Swap the front/back buffers so you see the rendering
        glfwPollEvents();            // Process window events such as closing, input, resize, etc.
    }
}