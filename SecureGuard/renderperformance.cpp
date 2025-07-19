#include "renderperformance.h"

// Constructor definition
RenderPerformance::RenderPerformance(std::vector<Performance>& performanceList)
    : m_PerformanceList(performanceList)
{
}

// Render method definition
void RenderPerformance::render()
{
    glColor3f(0.2196f, 0.6f, 0.4f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(260.0f, 81.0f);
    glVertex2f(260.0f + 620.0f, 81.0f);
    glEnd();

    for (Performance& perf : m_PerformanceList)
    {
        perf.render(); 
    }
}
