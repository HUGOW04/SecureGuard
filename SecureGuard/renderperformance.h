#ifndef RENDERPERFORMANCE_H
#define RENDERPERFORMANCE_H

#include <vector>
#include "performance.h"
#include "console.h"

class RenderPerformance
{
public:
    RenderPerformance(std::vector<Performance>& performanceList, std::vector<Console>& consoles);
    void render();

private:
    std::vector<Performance>& m_PerformanceList;
    std::vector<Console>& m_Consoles;
};

#endif // RENDERPERFORMANCE_H
