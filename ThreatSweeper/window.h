#ifndef WINDOW_H
#define WINDOW_H


#include <memory>
#include <iostream>
#include <thread>
#include <algorithm>

#include "gl_includes.h"
#include "overview.h"
#include "widget.h"
#include "sidebar.h"
#include "button.h"
#include "downloadhash.h"
#include "scanpanel.h"
#include "scan.h"
#include "console.h"
#include "toggle.h"
#include "system.h"
#include "loadimage.h"
#include "tinyfiledialogs.h"

class Window
{

public:
    Window(int width, int height, const char* title);
    ~Window();
    void handleEvents();
private:
    void createDirectorys();
    GLFWwindow* getWindow();
    void initWindowContext();
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    void setupProjection();
    void renderFont(Font* font, float x, float y, const std::string& text, float red, float green, float blue, float alpha);
    void initGLFW();
    void createWindow();

    //logs
    void loadAllLogs();
    void loadSystemScanLog();
    void loadStartupProcessesLog();
    void loadThreatLog();

    // Memory
    float getTotalMemoryLoad();
    // CPU
    float getTotalCPULoad();
    float calculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
    unsigned long long fileTimeToInt64(const FILETIME& ft);
private:
    GLFWwindow* m_Window;
    std::unique_ptr<System> m_System;
    std::unique_ptr<Overview> m_Overview;
    std::unique_ptr<Sidebar> m_Sidebar;
    std::unique_ptr<DownloadHash> m_Hash;
    std::unique_ptr<Scanpanel> m_ScanPanel;
    std::unique_ptr<Scan> m_Scan;
    std::unique_ptr<Loadimage> m_LogoImage;
    std::unique_ptr<Loadimage> m_HomeImage;

    int m_Width;
    int m_Height;
    int m_Vsync = 1;
    const char* m_Title;
    bool draggingWindow = false;
    double dragOffsetX = 0.0, dragOffsetY = 0.0;

    // Static state for calculating CPU deltas
    unsigned long long previousTotalTicks = 0;
    unsigned long long previousIdleTicks = 0;
    std::chrono::steady_clock::time_point lastCpuSampleTime;

    // render
    bool overview = true;
    bool firewall = false;
    bool scan = false;
    
    // render values
    std::string threatsFound = "0";
    std::string lastSystemScan = "never";
    std::string startupProcesses = "0";
    std::string cpuUsage = "0%";
    std::string memoryUsage = "0%";

    // fonts
    std::unique_ptr<Font> m_Regular;
    std::unique_ptr<Font> m_Italic;
    std::unique_ptr<Font> m_Bold;
    std::unique_ptr<Font> m_Light;

    // buttons
    std::vector<Button> sidebarButtons;
    std::vector<Button> scanButtons;
    std::vector<Button> systemButtons;
    
    // widgets
    std::vector<Widget> overviewWidget;

    // consoles
    std::vector<Console> consoles;

    // toggle
    std::vector<Toggle> toggle;


};


#endif
