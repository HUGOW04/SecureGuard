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
    m_Regular = std::make_unique<Font>("fonts/Roboto-Light.ttf", 20.0f);
    m_Bold = std::make_unique<Font>("fonts/Roboto-Bold.ttf", 30.0f);
    m_Light = std::make_unique<Font>("fonts/Lato-Light.ttf", 20.0f);

    // load images
    m_LogoImage = std::make_unique<Loadimage>("images/shield.png");
    m_HomeImage = std::make_unique<Loadimage>("images/home.png");

    // sidebar buttons
    sidebarButtons.push_back(Button(20, 60, 200, 40, "Overview", "overview", m_Regular.get(), m_HomeImage.get()));
    sidebarButtons.push_back(Button(20, 110, 200, 40, "Firewall", "firewall", m_Regular.get(),NULL));
    sidebarButtons.push_back(Button(20, 160, 200, 40, "Scan", "scan", m_Regular.get(),NULL));
    sidebarButtons.push_back(Button(20, 210, 200, 40, "Performance", "performance", m_Regular.get(), NULL));
    sidebarButtons.push_back(Button(20, 260, 200, 40, "Setting", "setting", m_Regular.get(), NULL));
    
    // system buttons
    systemButtons.push_back(Button(m_Width - 20, 0, 20, 20, "x", "x", m_Italic.get(),NULL));
    systemButtons.push_back(Button(m_Width - 40, 0, 20, 20, "-", "-", m_Italic.get(),NULL));

    // overview widget
    overviewWidget.push_back(Widget(265, 110, 190, 140, &lastSystemScan, "Security Status", m_Italic.get(), m_Italic.get()));
    overviewWidget.push_back(Widget(475, 110, 190, 140, &threatsFound, "Threats Found", m_Italic.get(), m_Italic.get()));
    overviewWidget.push_back(Widget(685, 110, 190, 140, &lastSystemScan, "Updates", m_Italic.get(), m_Italic.get()));

    // Overview quick-actions
    overviewWidget.push_back(Widget(265, 345, 285, 80, &threatsFound, "Run Quick Scan", m_Italic.get(), m_Italic.get()));
    overviewWidget.push_back(Widget(590, 345, 285, 80, &lastSystemScan, "Scan History", m_Italic.get(), m_Italic.get()));

    // scanbuttons
    scanButtons.push_back(Button(280, 110, 100, 30, "Fullscan", "fullscan", m_Italic.get(),NULL));
    scanButtons.push_back(Button(400, 110, 100, 30, "Fastscan", "fastscan", m_Italic.get(),NULL));
    scanButtons.push_back(Button(280, 160, 100, 30, "Filescan", "filescan", m_Italic.get(),NULL));
    scanButtons.push_back(Button(400, 160, 100, 30, "Memoryscan", "memoryscan", m_Italic.get(),NULL));

    // consoles
    //consoles.push_back(Console(180, 210, 590, 150, m_Scan->getThreatBuffer(), m_Italic.get()));
    consoles.push_back(Console(260, 320, 620, 140, m_Scan->getLogBuffer(), m_Italic.get()));

    // toggle
    settingToggle.push_back(Toggle(800, 170, 40, 20));
    settingToggle.push_back(Toggle(800, 220, 40, 20));
    settingToggle.push_back(Toggle(800, 270, 40, 20));


    std::vector<std::string> settingsTitle = {
     "Real-time Protection",
     "Automatic Updates",
     "Automatic Startup"
    };

    std::vector<std::string> settingsDescription = {
        "Continuously monitors your system for threats.",
        "Automatically download and install virus definition updates.",
        "Starts automatically with your system for continuous protection."
    };
    
    performances.push_back(Performance(&cpuUsage, m_Regular.get(), 260, 100));
    performances.push_back(Performance(&memoryUsage, m_Regular.get(), 500, 100));

    m_Sidebar = std::make_unique<Sidebar>(sidebarButtons);
    m_System = std::make_unique<System>(systemButtons);
    m_Overview = std::make_unique<Overview>(overviewWidget);
    m_ScanPanel = std::make_unique<Scanpanel>(scanButtons, consoles);
    m_Setting = std::make_unique<Setting>(m_Regular.get(), m_Light.get(),settingsTitle, settingsDescription,settingToggle);
    m_RenderPerformance = std::make_unique<RenderPerformance>(performances);


}

void Window::createDirectorys()
{
    std::string dirs[] = { "data","logs","fonts","images" };
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
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
}

void Window::createWindow()
{
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
    if (!m_Window)
    {
        std::cout << "Failed to create glfwWindow" << std::endl;
    }

    glfwSetWindowUserPointer(m_Window, this);

    GLFWimage images[1];
    images[0].pixels = stbi_load("images/shield.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(m_Window, 1, images);
    stbi_image_free(images[0].pixels);

    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    int screen_width = mode->width;
    int screen_height = mode->height;


    int pos_x = (screen_width - m_Width) / 2;
    int pos_y = (screen_height - m_Height) / 2;

    glfwSetWindowPos(m_Window, pos_x, pos_y);

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

    // Move window if dragging
    if (win->draggingWindow)
    {
        int winX, winY;
        glfwGetWindowPos(window, &winX, &winY);

        int newWinX = winX + static_cast<int>(xpos - win->dragOffsetX);
        int newWinY = winY + static_cast<int>(ypos - win->dragOffsetY);

        glfwSetWindowPos(window, newWinX, newWinY);
    }

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
                float newY = mouseY - c.dragOffsetY;

                float minY = c.getY();
                float maxY = c.getY() + c.getHeight();

                // Optionally clamp:
                // newY = std::max(minY, std::min(newY, maxY));

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

    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
    {
        std::cout << "x: " << mouseX << " y: " << mouseY << std::endl;
        // Enable window dragging if clicked in top 40 pixels
        if (mouseY <= 40.0f)
        {
            win->draggingWindow = true;
            win->dragOffsetX = xpos;
            win->dragOffsetY = ypos;
        }

        for (auto& b : win->sidebarButtons)
        {
            if (mouseX >= b.getX() && mouseX <= b.getX() + b.getWidth() &&
                mouseY >= b.getY() && mouseY <= b.getY() + b.getHeight())
            {
                std::string name = b.getName();

                win->overview = (name == "Overview");
                win->firewall = (name == "Firewall");
                win->scan = (name == "Scan");
                win->performance = (name == "Performance");
                win->setting = (name == "Setting");
            }
        }
        for (auto& b : win->systemButtons)
        {
            if (mouseX >= b.getX() && mouseX <= b.getX() + b.getWidth() &&
                mouseY >= b.getY() && mouseY <= b.getY() + b.getHeight())
            {
                if (b.getName() == "x")
                {
                    glfwSetWindowShouldClose(win->m_Window, GLFW_TRUE);
                }
                else if (b.getName() == "-")
                {
                    glfwIconifyWindow(win->m_Window);
                }

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
                                std::string threatStr = std::string("[SHA-256 MATCH] ") + path;

                                std::ofstream threatFile("logs/threat_log.txt", std::ios::app);
                                threatFile << threatStr << std::endl;
                                threatFile.close();

                                win->m_Scan->setThreatBuffer(threatStr);
                            }

                            win->m_Scan->setLogBuffer(path);
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
                float scrollbarX = c.getX() + c.getWidth() - 2.0f;
                if (mouseX >= scrollbarX && mouseX <= scrollbarX + 2.0f &&
                    mouseY >= c.getY() && mouseY <= c.getY() + c.getHeight())
                {
                    std::cout << "Start dragging scrollbar!" << std::endl;
                    c.draggingScrollbar = true;
                    c.dragOffsetY = mouseY - c.getY();
                }
            }

            
        }
        else if (win->setting)
        {
            for (auto& t : win->settingToggle)
            {
                if (mouseX >= t.getX() && mouseX <= t.getX() + t.getWidth() &&
                    mouseY >= t.getY() && mouseY <= t.getY() + t.getHeight())
                {
                    t.setState(!t.getState());
                }
            }
        }
    }
    else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
    {
        win->draggingWindow = false;

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



void Window::renderFont(Font* font, float x, float y, const std::string& text, float red, float green, float blue, float alpha)
{
    if (font && font->isValid())
    {
        font->setColor(red, green, blue, alpha);
        font->render(text, x, y, 1.0f);
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
        glClearColor(0.13f, 0.16f, 0.21f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        // get values from files
        loadAllLogs();

        // side buttons
        m_Sidebar->render();
        // sytem buttons
        m_System->render();
        // logo
        renderFont(m_Bold.get(), 60.0f, 40.0f, "SecureGuard", 1.0f, 1.0f, 1.0f, 1.0f);

        if (m_LogoImage)
            m_LogoImage->drawImage(20, 15, 40, 40);

        for (auto& b : sidebarButtons)
        {
            if (overview && b.getName() == "Overview")
            {
                b.setSelect(true);
            }
            else if (firewall && b.getName() == "Firewall")
            {
                b.setSelect(true);
            }
            else if (scan && b.getName() == "Scan")
            {
                b.setSelect(true);
            }
            else if (performance && b.getName() == "Performance")
            {
                b.setSelect(true);
            }
            else if (setting && b.getName() == "Setting")
            {
                b.setSelect(true);
            }
            else
            {
                b.setSelect(false);
            }
        }


        if (overview)
        {
            //Security Overview
            renderFont(m_Bold.get(), 260.0f, 70.0f, "Dashboard", 1.0f, 1.0f, 1.0f, 1.0f);
            renderFont(m_Bold.get(), 260.0f, 320.0f, "Quick Actions", 1.0f, 1.0f, 1.0f, 1.0f);
            
            m_Overview->render();
           
        }
        else if (firewall)
        {
            renderFont(m_Bold.get(), 260.0f, 70.0f, "Firewall Protection", 1.0f, 1.0f, 1.0f, 1.0f);
            //renderFont(m_Light.get(), 180.0f, 60.0f, "Control network access and monitor connections", 1.0f, 1.0f, 1.0f, 1.0f);
        }
        else if (scan)
        {
            renderFont(m_Bold.get(), 260.0f, 70.0f, "Scan", 1.0f, 1.0f, 1.0f, 1.0f);
            //renderFont(m_Light.get(), 180.0f, 60.0f, "Real-time malware detection and removal", 1.0f, 1.0f, 1.0f, 1.0f);
            m_ScanPanel->render();

        }
        else if (performance)
        {
            renderFont(m_Bold.get(), 260.0f, 70.0f, "Performance", 1.0f, 1.0f, 1.0f, 1.0f);
            m_RenderPerformance->render();
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
        else if (setting)
        {
            renderFont(m_Bold.get(), 260.0f, 70.0f, "Setting", 1.0f, 1.0f, 1.0f, 1.0f);
            m_Setting->render();
        }

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}