#include "overview.h"

Overview::Overview(std::vector<Widget>& widget)
	: m_Widgets(widget)
{

}

void Overview::render()
{

	// Line 1
	glColor3f(0.2196f, 0.6f, 0.4f);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex2f(260.0f, 81.0f);       
	glVertex2f(260.0f + 620.0f, 81.0f); 
	glEnd();

	// Line 2
	glColor3f(0.2196f, 0.6f, 0.4f); 
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex2f(260.0f, 331.0f);            
	glVertex2f(260.0f + 620.0f, 331.0f);  
	glEnd();


	for (auto& widget : m_Widgets)
	{


		widget.render();

	}
}
