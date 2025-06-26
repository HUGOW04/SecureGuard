#include "window.h"

Window::Window(int width, int height, const char* title)
    : m_Width(width), m_Height(height), m_Title(title)
{
    initGLFW();
    createWindow();
    initWindowContext();
    m_Overview = std::make_unique<Overview>(m_Window);

    m_Proxy = std::make_unique<Proxy>();
    m_Proxy->start();  // Start proxy on a separate thread
}

Window::~Window()
{
    if (m_Proxy) {
        m_Proxy->stop();  // Stop proxy thread and join
    }

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
}

void Window::createWindow()
{
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
    if (!m_Window)
    {
        std::cout << "Failed to create glfwWindow" << std::endl;
    }

}

void Window::initWindowContext()
{
    glfwMakeContextCurrent(m_Window);
    glfwSetKeyCallback(m_Window, key_callback);
    glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
    // Enable/Disable m_Vsync: 1 for VSync on, 0 for VSync off
    glfwSwapInterval(m_Vsync ? 1 : 0);
    glOrtho(0.0, m_Width, 0.0, m_Height, 0.0, 1.0); // this creates a canvas you can do 2D drawing on
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::handleEvents()
{

    while (!glfwWindowShouldClose(m_Window))
    {
        m_Overview->Render();
        
        glfwSwapBuffers(m_Window);   // Swap the front/back buffers so you see the rendering
        glfwPollEvents();            // Process window events such as closing, input, resize, etc.
    }
}
