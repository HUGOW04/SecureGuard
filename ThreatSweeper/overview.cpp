#include "overview.h"

Overview::Overview(std::vector<Widget>& widget)
	: m_Widgets(widget)
{

}

void Overview::render()
{
	for (auto& widget : m_Widgets)
	{
		widget.render();
	}
} 

