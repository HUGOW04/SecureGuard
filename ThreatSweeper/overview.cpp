#include "overview.h"

Overview::Overview(std::vector<Widget>& widget)
	: m_Widgets(widget)
{

}

void Overview::render()
{
	for (auto& widget : m_Widgets)
	{

		glColor3f(0.2196f, 0.6f, 0.4f);
		glLineWidth(8.0f);

		glRectf(260, 80, 260 + 620, 80 + 2);

		widget.render();

	}
} 

