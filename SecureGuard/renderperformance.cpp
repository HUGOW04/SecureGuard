#include "renderperformance.h"

// Constructor definition
RenderPerformance::RenderPerformance(std::vector<Performance>& performanceList)
    : m_PerformanceList(performanceList)
{
}

// Render method definition
void RenderPerformance::render()
{

    for (Performance& perf : m_PerformanceList)
    {
        perf.render(); 
    }
}
