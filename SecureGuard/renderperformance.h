#ifndef RENDERPERFORMANCE_H
#define RENDERPERFORMANCE_H

#include <vector>
#include "performance.h"

class RenderPerformance
{
public:
    RenderPerformance(std::vector<Performance>& performanceList);
    void render();

private:
    std::vector<Performance>& m_PerformanceList;
};

#endif // RENDERPERFORMANCE_H
