#include "overview.h"

Overview::Overview(std::vector<Widget>& widget)
	: m_Widgets(widget)
{

}

void Overview::render()
{

	// Line 1
	glColor3f(0.2196f, 0.6f, 0.4f);
	glLineWidth(8.0f);
	glRectf(260, 80, 260 + 620, 80 + 2);
	glEnd();

	// Line 2
	glColor3f(0.2196f, 0.3f, 0.4f);
	glRectf(260, 330, 260 + 620, 330 + 2);
	for (auto& widget : m_Widgets)
	{


		widget.render();

	}
}
