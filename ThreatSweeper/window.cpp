#include "window.h"

Window::Window(int width, int height, const char* title)
    : m_Width(width), m_Height(height), m_Title(title)
{
    // create dirs to have it orginized!
    createDirectorys();
   
    // Fetch SHA-256 hashes from Bazaar; maybe append more databases later to full_sha256.txt.
    std::ifstream file("data/full_sha256.txt");
    if (!file.good())
    {
        std::cout << "Creating sha256 file!" << std::endl;
        m_Hash = std::make_unique<DownloadHash>();
        m_Hash->downloadhash();
        m_Hash->unzip();
    }

    m_Scan = std::make_unique<Scan>();

    initGLFW();
    createWindow();
    initWindowContext();

    m_Italic = std::make_unique<Font>("fonts/Lato-Italic.ttf", 20.0f);
    m_LightItalic = std::make_unique<Font>("fonts/Lato-LightItalic.ttf", 40.0f);
    m_Bold = std::make_unique<Font>("fonts/Lato-Bold.ttf", 30.0f);
    m_Light = std::make_unique<Font>("fonts/Lato-Light.ttf", 20.0f);

    // sidebar buttons
    sidebarButtons.push_back(Button(10, 10,  130,30, "Overview", "overview", m_Italic.get()));
    sidebarButtons.push_back(Button(10, 60,  130, 30, "Firewall", "firewall", m_Italic.get()));
    sidebarButtons.push_back(Button(10, 110, 130, 30, "Scan", "scan", m_Italic.get()));
    sidebarButtons.push_back(Button(10, 160, 130, 30, "Privacy", "privacy", m_Italic.get()));
    sidebarButtons.push_back(Button(10, 210, 130, 30, "Settings", "settings", m_Italic.get()));

    // overview widget
    overviewWidget.push_back(Widget(180, 110, 120, 80, &threatsFound, "Threats Found", m_Italic.get(), m_Italic.get()));
    overviewWidget.push_back(Widget(320, 110, 180, 80, &lastSystemScan, "Last System Scan", m_Italic.get(), m_Italic.get()));
    overviewWidget.push_back(Widget(180, 210, 140, 80, &startupProcesses, "Startup Processes", m_Italic.get(), m_Italic.get()));
    overviewWidget.push_back(Widget(340, 210, 160, 80, &cpuUsage, "CPU Usage", m_Italic.get(), m_Italic.get()));
    overviewWidget.push_back(Widget(520, 210, 160, 80, &memoryUsage, "Memory Usage", m_Italic.get(), m_Italic.get()));
    
    // scanbuttons
    scanButtons.push_back(Button(180, 110, 100, 30, "Fullscan", "fullscan", m_Italic.get()));
    scanButtons.push_back(Button(300, 110, 100, 30, "Fastscan", "fastscan", m_Italic.get()));
    scanButtons.push_back(Button(180, 160, 100, 30, "Filescan", "filescan", m_Italic.get()));
    scanButtons.push_back(Button(300, 160, 100, 30, "Memoryscan", "memoryscan", m_Italic.get()));

    // consoles
    consoles.push_back(Console(180, 275, 590, 100,m_Scan->getThreatBuffer(), m_Italic.get()));
    consoles.push_back(Console(180, 475, 590, 100, m_Scan->getLogBuffer(), m_Italic.get()));

    m_Sidebar = std::make_unique<Sidebar>(sidebarButtons);
    m_Overview = std::make_unique<Overview>(overviewWidget);
    m_ScanPanel = std::make_unique<Scanpanel>(scanButtons,consoles);
}

void Window::createDirectorys()
{
    std::string dirs[] = {"data","logs","fonts"};
    for (const auto& dir : dirs)
    {
        if (!std::filesystem::exists(dir))
        {
            std::filesystem::create_directory(dir);
        }
    }


}


Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

GLFWwindow* Window::getWindow()
{
    return m_Window;
}

void Window::initGLFW()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize glfw" << std::endl;
    }
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void Window::createWindow()
{
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
    if (!m_Window)
    {
        std::cout << "Failed to create glfwWindow" << std::endl;
    }

    glfwSetWindowUserPointer(m_Window, this);

}

void Window::initWindowContext()
{
    glfwMakeContextCurrent(m_Window); 
    glfwSetKeyCallback(m_Window, key_callback);
    glfwSetCursorPosCallback(m_Window, cursor_position_callback);
    glfwSetMouseButtonCallback(m_Window, mouse_callback);
    glfwSetCursorPosCallback(m_Window, cursor_position_callback);
    // Enable/Disable m_Vsync: 1 for VSync on, 0 for VSync off
    glfwSwapInterval(m_Vsync ? 1 : 0);

    setupProjection();
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!win) return;

    float mouseX = static_cast<float>(xpos);
    float mouseY = static_cast<float>(ypos);

    for (auto& b : win->sidebarButtons)
    {
        bool hoverBefore = b.getIsHovering();
        if (mouseX >= b.getX() && mouseX <= b.getX() + b.getWidth() &&
            mouseY >= b.getY() && mouseY <= b.getY() + b.getHeight())
        {
            b.setHovering(true);
        }
        else
        {
            b.setHovering(false);
        }
    }
    if (win->scan)
    {

        for (auto& b : win->scanButtons)
        {
            bool hoverBefore = b.getIsHovering();
            if (mouseX >= b.getX() && mouseX <= b.getX() + b.getWidth() &&
                mouseY >= b.getY() && mouseY <= b.getY() + b.getHeight())
            {
                b.setHovering(true);
            }
            else
            {
                b.setHovering(false);
            }
        }
        for (auto& c : win->consoles)
        {
            if (c.draggingScrollbar)
            {
                // Begränsa scrollbar inom konsolens yta
                float newY = mouseY - c.dragOffsetY;

                float minY = c.getY();
                float maxY = c.getY() + c.getHeight() - c.getHeight(); // scrollbar kan vara kortare i framtiden

                // Just nu är scrollbar_height fast till 10.0f, så vi kan hårdkoda maxY om du vill:
                c.setY(newY);
            }
        }
    }
}





void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!win) return;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    float mouseX = static_cast<float>(xpos);
    float mouseY = static_cast<float>(ypos);

    if (action == GLFW_PRESS)
    {
        for (auto& b : win->sidebarButtons)
        {
            if (mouseX >= b.getX() && mouseX <= b.getX() + b.getWidth() &&
                mouseY >= b.getY() && mouseY <= b.getY() + b.getHeight())
            {
                win->overview = b.getName() == "Overview";
                win->firewall = b.getName() == "Firewall";
                win->scan = b.getName() == "Scan";

                std::cout << b.getName() << " selected" << std::endl;
            }
        }

        if (win->scan)
        {
            for (auto& b : win->scanButtons)
            {
                if (mouseX >= b.getX() && mouseX <= b.getX() + b.getWidth() &&
                    mouseY >= b.getY() && mouseY <= b.getY() + b.getHeight())
                {
                    const std::string& name = b.getName();

                    if (name == "Fullscan")
                    {
                        std::cout << "clicked on fullscan!" << std::endl;
                        std::thread([win]() { win->m_Scan->recursiveIterator(); }).detach();
                    }
                    else if (name == "Fastscan")
                    {
                        std::cout << "clicked on Fastscan!" << std::endl;
                        std::thread([win]() { win->m_Scan->fastScan(); }).detach();
                    }
                    else if (name == "Filescan")
                    {
                        std::cout << "clicked on Filescan!" << std::endl;
                        const char* path = tinyfd_openFileDialog("Select file", "", 0, NULL, NULL, 0);
                        if (path) {
                            std::cout << "Selected file: " << path << std::endl;
                            std::string hash = win->m_Scan->fileToSHA256(path);
                            if (win->m_Scan->compareSHA256Str(hash))
                            {
                                std::cout << "Warning: File is in SHA-256 virus database! -> " << path << std::endl;
                                std::cout << "Removing file -> " << path << std::endl;
                                std::ofstream threatFile("logs/threat_log.txt", std::ios::app);
                                threatFile << "[SHA-256 MATCH] " << path << std::endl;
                                threatFile.close();
                            }
                        }
                    }
                    else if (name == "Memoryscan")
                    {
                        std::cout << "clicked on Memoryscan!" << std::endl;
                        std::thread([win]() { win->m_Scan->ScanMemory(); }).detach();
                    }
                }
            }

            for (auto& c : win->consoles)
            {
                float scrollbarX = c.getX() + c.getWidth() - 2.0f; // 2.0f is scrollbar width
                float scrollbarY = c.getY();
                float scrollbarH = 10.0f; // or c.getScrollbarHeight() if you add that

                if (mouseX >= scrollbarX && mouseX <= scrollbarX + 2.0f &&
                    mouseY >= c.getY() && mouseY <= c.getY() + c.getHeight())
                {
                    std::cout << "Start dragging scrollbar!" << std::endl;
                    c.draggingScrollbar = true;
                    c.dragOffsetY = mouseY - c.getY(); // or scrollbarY if separate
                }
            }
        }
    }
    else if (action == GLFW_RELEASE)
    {
        for (auto& c : win->consoles)
        {
            if (c.draggingScrollbar)
            {
                std::cout << "Stop dragging scrollbar!" << std::endl;
                c.draggingScrollbar = false;
            }
        }
    }
}


void Window::renderFont(Font* font, float x, float y,const std::string& text,float red,float green,float blue, float alpha)
{
    if (font && font->isValid())
    {
        font->setColor(red, green, blue, alpha);
        font->render(text, x,y, 1.0f);
    }
}

unsigned long long Window::fileTimeToInt64(const FILETIME& ft) {
    return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime);
}

float Window::calculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks) {
    unsigned long long totalTicksSinceLast = totalTicks - previousTotalTicks;
    unsigned long long idleTicksSinceLast = idleTicks - previousIdleTicks;

    previousTotalTicks = totalTicks;
    previousIdleTicks = idleTicks;

    if (totalTicksSinceLast == 0) return 0.0f;
    return 1.0f - ((float)idleTicksSinceLast / totalTicksSinceLast);
}

float Window::getTotalCPULoad() {
    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
        return -1.0f;

    unsigned long long idle = fileTimeToInt64(idleTime);
    unsigned long long kernel = fileTimeToInt64(kernelTime);
    unsigned long long user = fileTimeToInt64(userTime);

    unsigned long long total = kernel + user;
    return calculateCPULoad(idle, total) * 100.0f;
}

float Window::getTotalMemoryLoad()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    DWORDLONG availPhysMem = memInfo.ullAvailPhys;
    DWORDLONG usedPhysMem = totalPhysMem - availPhysMem;

    double usedRatio = static_cast<double>(usedPhysMem) / static_cast<double>(totalPhysMem);

    return usedRatio * 100;
}

void Window::loadAllLogs()
{
    loadSystemScanLog();
    loadStartupProcessesLog();
    loadThreatLog();
}

void Window::loadStartupProcessesLog()
{
    std::ifstream startupFile("logs/startup_log.txt");
    if (!startupFile.good()) return;

    std::string line;
    int lineRow = 0;
    while (std::getline(startupFile, line))
    {
        lineRow++;
    }
    startupProcesses = std::to_string(lineRow);
}

void Window::loadSystemScanLog()
{
    std::ifstream logfile("logs/logfile.txt", std::ios::ate); // open at end
    if (!logfile.is_open()) return;

    std::streampos fileSize = logfile.tellg();
    std::string lastLine;
    char ch;

    for (std::streamoff i = 1; i <= fileSize; ++i) {
        logfile.seekg(-i, std::ios::end);
        logfile.get(ch);

        if (ch == '\n' && !lastLine.empty()) break;
        if (ch != '\n' && ch != '\r') lastLine.insert(0, 1, ch);
    }

    size_t commaPos = lastLine.find(',');
    if (commaPos != std::string::npos) {
        lastSystemScan = lastLine.substr(0, commaPos);
    }
}

void Window::loadThreatLog()
{
    std::ifstream startupFile("logs/threat_log.txt");
    if (!startupFile.good()) return;

    std::string line;
    int lineRow = 0;
    while (std::getline(startupFile, line))
    {
        lineRow++;
    }
    threatsFound = std::to_string(lineRow);
}

void Window::setupProjection()
{
    glViewport(0, 0, m_Width, m_Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, m_Width, m_Height, 0.0, -1.0, 1.0);  // Flip Y here
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Window::handleEvents()
{

    while (!glfwWindowShouldClose(m_Window))
    {
        glClearColor(0.102f, 0.102f, 0.180f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // get values from files
        loadAllLogs();

        // side buttons
        m_Sidebar->render();

        if (overview)
        {
            //Security Overview
           renderFont(m_LightItalic.get(), 180.0f, 40.0f, "Security Overview", 1.0f, 1.0f, 1.0f, 1.0f);
           renderFont(m_Light.get(), 180.0f, 60.0f, "Your system protection status at a glance", 1.0f, 1.0f, 1.0f, 1.0f);
           m_Overview->render();
           auto now = std::chrono::steady_clock::now();
           auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCpuSampleTime);

           if (elapsed.count() >= 1000) // 1000ms = 1s
           {
               float cpuLoad = getTotalCPULoad();
               float memoryLoad = getTotalMemoryLoad();
               cpuUsage = std::to_string(static_cast<int>(cpuLoad)) + "%";
               memoryUsage = std::to_string(static_cast<int>(memoryLoad)) + "%";
              
               lastCpuSampleTime = now;
           }
        }
        else if (firewall)
        {
            renderFont(m_LightItalic.get(), 180.0f, 40.0f, "Firewall Protection", 1.0f, 1.0f, 1.0f, 1.0f);
            renderFont(m_Light.get(), 180.0f, 60.0f, "Control network access and monitor connections", 1.0f, 1.0f, 1.0f, 1.0f);
        }
        else if (scan)
        {
            renderFont(m_LightItalic.get(), 180.0f, 40.0f, "Antivirus Protection", 1.0f, 1.0f, 1.0f, 1.0f);
            renderFont(m_Light.get(), 180.0f, 60.0f, "Real-time malware detection and removal", 1.0f, 1.0f, 1.0f, 1.0f);
            m_ScanPanel->render();
            
        }

        glfwSwapBuffers(m_Window);   
        glfwPollEvents();        
    }
}