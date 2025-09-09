#ifndef FIREWALL_H
#define FIREWALL_H

#define _USE_MATH_DEFINES  
#include <cmath>        
#include <vector>

#include "font.h"
#include "toggle.h"
#include "gl_includes.h"

class Firewall
{
public:
	Firewall(Font* title, Font* text, std::vector<std::string>& settingsTitle, std::vector<std::string>& settingsDescriptions, std::vector<Toggle>& toggle);
    void drawCorner(float cx, float cy, float radius, int startAngle, int endAngle);
    void drawRoundedRect(float x, float y, float width, float height, float radius);
    void render();
private:
    Font* m_FontTitle;
    Font* m_FontText;
    std::vector<Toggle>& m_Toggles;
    std::vector<std::string> m_SettingTitles;
    std::vector<std::string> m_SettingDescriptions;
};

#endif // !FIREWALL_H
