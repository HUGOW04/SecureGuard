#include "sidebar.h"

Sidebar::Sidebar(std::vector<Button>& buttons)
	: m_Buttons(buttons)
{

}

void Sidebar::render(GLFWwindow* m_Window, int m_Width, int m_Height)
{
	glClearColor(0.102f, 0.102f, 0.180f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(1);
	glColor3f(0.3f, 0.3f, 0.3f);
	glBegin(GL_LINES);
	glVertex2i(150.0, m_Height);
	glVertex2i(150.0, 0.0);
	glEnd();

	for (auto& button : m_Buttons)
	{
		button.render(m_Window, m_Width, m_Height);
	}
}