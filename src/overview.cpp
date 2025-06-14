#include "overview.h"

Overview::Overview(GLFWwindow* window)
{
    m_Window = window;
}

void Overview::Render()
{
    glClearColor(100.0f, 0.3f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}