#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>
#include "button.h"

class System
{
public:
	System(std::vector<Button>& system);
	void render();
private:
	std::vector<Button>& m_System;

};

#endif // !SYSTEM_H
