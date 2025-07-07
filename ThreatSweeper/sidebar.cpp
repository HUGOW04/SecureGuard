#include "sidebar.h"

Sidebar::Sidebar(std::vector<Button>& buttons)
	: m_Buttons(buttons)
{

}

void Sidebar::render()
{
	
	glColor3f(0.07f, 0.09f, 0.15f); // RGB(32, 41, 54) — #202936
	glBegin(GL_QUADS);
	glVertex2f(0.0f, 0.0f);       // Bottom-left
	glVertex2f(0.0f, 600.0f);     // Top-left
	glVertex2f(240.0f, 600.0f);   // Top-right
	glVertex2f(240.0f, 0.0f);     // Bottom-right
	glEnd();

	for (auto& button : m_Buttons)
	{
		button.render();
	}
}