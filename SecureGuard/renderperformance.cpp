#include "renderperformance.h"

// Constructor definition
RenderPerformance::RenderPerformance(std::vector<Performance>& performanceList, std::vector<Console>& consoles)
    : m_PerformanceList(performanceList), m_Consoles(consoles)
{
}

// Render method definition
void RenderPerformance::render()
{

    for (Performance& perf : m_PerformanceList)
    {
        perf.render(); 
    }

    for (auto& console : m_Consoles)
    {
        console.render();
    }
}
