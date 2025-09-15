#include "system.h"

System::System(std::vector<Button>& system)
	: m_System(system)
{

}

void System::render()
{
	for (auto& system : m_System)
	{
		system.render();
	}
}