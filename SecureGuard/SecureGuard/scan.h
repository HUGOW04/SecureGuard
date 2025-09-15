#ifndef SCAN_H
#define SCAN_H

#include <filesystem>
#include <iostream>
#include <fstream>
#include <openssl/evp.h>
#include <sstream>
#include <Windows.h>
#include <psapi.h>
#include <unordered_set>
#include <unordered_map>
#include <ctime>
#include <array>

class Scan
{
public:
    Scan();
    void recursiveIterator();
    std::string fileToSHA256(const std::string& filePath);
    bool compareSHA256File(const std::string& hashPath);
    bool compareSHA256Str(const std::string& hashStr);
    void fastScan();
    void ScanMemory();
    const std::vector<std::string>& getLogBuffer() const { return logBuffer;}
    const std::vector<std::string>& getThreatBuffer() const { return threatBuffer; }
    void setLogBuffer(const std::string& log) { logBuffer.push_back(log); }
    void setThreatBuffer(const std::string& threat) { threatBuffer.push_back(threat); }
private:
    void load_hashes();
    bool checkFileChange(const std::string& filePath, const std::string& hash);
    bool hasMagicBytes(const std::string& filePath);
    void removeFile(const std::string& filePath);
    void getStartupApplications();
    std::string GetProcessName(DWORD pid);
private:
    // buffers
    std::vector<std::string> logBuffer;
    std::vector<std::string> threatBuffer;

    std::unordered_set<std::string> hash_set;
    std::unordered_set<std::string> ignoreProcesses = {
        // System and Windows processes
        "System Idle Process",
        "System",
        "wininit.exe",
        "csrss.exe",
        "smss.exe",
        "winlogon.exe",
        "services.exe",
        "lsass.exe",
        "svchost.exe",
        "explorer.exe",
        "taskhostw.exe",
        "dwm.exe",
        "spoolsv.exe",

        // Browsers and extensions
        "msedge.exe",
        "chrome.exe",
        "firefox.exe",
        "opera.exe",

        // Antivirus and security software
        "MsMpEng.exe",             // Windows Defender
        "SecurityHealthService.exe",

        // Development tools
        "devenv.exe",              // Visual Studio
        "code.exe",                // VS Code
        "MSBuild.exe",

        // Other common apps
        "LockApp.exe",
        "RuntimeBroker.exe",
        "PhoneExperienceHost.exe",
        "Discord.exe",
        "steam.exe",
        "steamwebhelper.exe",
        "msedgewebview2.exe",
        "Surfshark.exe",
        "brave.exe",
        "RadeonSoftware.exe",
        "AMDRSSrcExt.exe",
        "CrossDeviceService.exe",

        // Microsoft ServiceHub and related processes
        "Microsoft.ServiceHub.Controller.exe",
        "ServiceHub.VSDetouredHost.exe",
        "PerfWatson2.exe",
        "ServiceHub.IdentityHost.exe",
        "ServiceHub.ThreadedWaitDialog.exe",
        "ServiceHub.IntellicodeModelService.exe",
        "ServiceHub.IndexingService.exe",
        "vctip.exe",
        "ServiceHub.Host.dotnet.x64.exe",
        "ServiceHub.TestWindowStoreHost.exe",
        "ServiceHub.DataWarehouseHost.exe",

        // Newly added from logs
        "NVIDIA Overlay.exe",
        "SecureGuard.exe"
    };

    std::unordered_map<std::string, std::vector<uint8_t>> extensionMagicMap = {
        {".exe", {0x4D, 0x5A}},       // MZ
        {".dll", {0x4D, 0x5A}},       // MZ
        {".sys", {0x4D, 0x5A}},       // MZ
        // Optional: .bat, .com, etc.
    };




};

#endif