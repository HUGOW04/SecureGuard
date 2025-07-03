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
        "ServiceHub.DataWarehouseHost.exe"
    };


    std::unordered_map<std::string, std::vector<uint8_t>> extensionMagicMap = {
        {".exe",  {0x4D, 0x5A}},       // MZ
        {".dll",  {0x4D, 0x5A}},       // MZ
        {".png",  {0x89, 0x50, 0x4E, 0x47}},
        {".jpg",  {0xFF, 0xD8, 0xFF, 0xE0}},
        {".jpeg", {0xFF, 0xD8, 0xFF, 0xE0}},
        {".gif",  {0x47, 0x49, 0x46, 0x38, 0x39, 0x61}}, // GIF89a
        {".bmp",  {0x42, 0x4D}},
        {".pdf",  {0x25, 0x50, 0x44, 0x46}}, // %PDF
        {".zip",  {0x50, 0x4B, 0x03, 0x04}},
        {".docx", {0x50, 0x4B, 0x03, 0x04}},
        {".xlsx", {0x50, 0x4B, 0x03, 0x04}},
        {".pptx", {0x50, 0x4B, 0x03, 0x04}},
        {".rar",  {0x52, 0x61, 0x72, 0x21}}, // Rar!
        {".7z",   {0x37, 0x7A, 0xBC, 0xAF}},
        {".gz",   {0x1F, 0x8B}},
        {".xz",   {0xFD, 0x37, 0x7A, 0x58}},
        {".mp3",  {0xFF, 0xFB}},  // Actually could also be FF F3 or FF F2, but handle basic
        {".wav",  {0x52, 0x49, 0x46, 0x46}}, // RIFF
        {".avi",  {0x52, 0x49, 0x46, 0x46}},
        {".mp4",  {0x66, 0x74, 0x79, 0x70}}, // ftyp
        {".mov",  {0x66, 0x74, 0x79, 0x70}},
        {".flv",  {0x46, 0x4C, 0x56, 0x01}},
        {".wmv",  {0x30, 0x26, 0xB2, 0x75}},
        {".iso",  {0x00, 0x00, 0x01, 0x01}},
        {".elf",  {0x7F, 0x45, 0x4C, 0x46}},
        {".class",{0xCA, 0xFE, 0xBA, 0xBE}},
        {".dex",  {0x64, 0x65, 0x78, 0x0A}},
        {".psd",  {0x38, 0x42, 0x50, 0x53}},
        {".torrent",{0x64, 0x31, 0x3A}}, // d1:
        {".xml",  {0x3C, 0x3F}},         // "<?"
        {".html", {0x3C, 0x21}},         // "<!"
        {".rtf",  {0x7B, 0x5C, 0x72, 0x74}},
        {".ps",   {0x25, 0x21, 0x50, 0x53}},
        {".sqlite",{0x53, 0x51, 0x4C, 0x69}},
        {".db",   {0x53, 0x51, 0x4C, 0x69}}
    };



};

#endif