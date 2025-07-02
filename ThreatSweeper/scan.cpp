#include "scan.h"

Scan::Scan()
{
    load_hashes();
    getStartupApplications();
}

void Scan::recursiveIterator()
{

    std::string path = "C:\\Users\\hugo\\source\\repos\\ThreatSweeper\\ThreatSweeper";
    std::string skipDirs[] = {
        "C:\\Windows\\System32\\DriverStore",
        "C:\\Windows\\Temp",
        "C:\\ProgramData",
        "C:\\$Recycle.Bin",
        "C:\\System Volume Information"
    };
    int skipCount = sizeof(skipDirs) / sizeof(skipDirs[0]);

    char buffer[80];
    
    const size_t maxFileSize = 100 * 1024 * 1024; // 100MB

    for (auto it = std::filesystem::recursive_directory_iterator(
        path, std::filesystem::directory_options::skip_permission_denied);
        it != std::filesystem::recursive_directory_iterator(); ++it)
    {
        try {

            std::time_t rawtime = std::time(nullptr);
            std::tm timeinfo;
            localtime_s(&timeinfo, &rawtime); // safer version for Windows

            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
            std::string currentTime(buffer);

            std::ofstream logFile("logs/logfile.txt", std::ios::app);
            std::string strPath = it->path().string();
            logFile << currentTime << "," << strPath << std::endl;
            logFile.close();
            logBuffer.push_back(strPath);

            bool shouldSkip = false;
            for (int i = 0; i < skipCount; i++)
            {
                if (strPath.compare(0, skipDirs[i].size(), skipDirs[i]) == 0)
                {
                    shouldSkip = true;
                    if (it->is_directory())
                    {
                        it.disable_recursion_pending(); // Hoppa över underkataloger helt
                    }
                    break;
                }
            }

            if (shouldSkip)
                continue;

            if (it->is_regular_file())
            {
                if (std::filesystem::file_size(it->path()) > maxFileSize) {
                    //std::cout << "Skipping scanning file larger than selected size: " << it->path() << std::endl;
                    continue;
                }

                
                if (hasMagicBytes(strPath)) {
                    //std::cout << "File has a different magic byte than extension name! -> " << strPath << std::endl;
                    std::ofstream threatFile("logs/threat_log.txt", std::ios::app);
                    threatFile << "[MAGIC BYTE MISMATCH] " << strPath << std::endl;
                    threatFile.close();
                    threatBuffer.push_back("[MAGIC BYTE MISMATCH] " + strPath);
                    // Optionally quarantine or rescan
                }

                // SHA-256 check comes second
                if (compareSHA256File(strPath)) {
                    //std::cout << "Warning: File is in SHA-256 virus database! -> " << strPath << std::endl;
                    //std::cout << "Removing file -> " << strPath << std::endl;
                    std::ofstream threatFile("logs/threat_log.txt", std::ios::app);
                    threatFile << "[SHA-256 MATCH] " << strPath << std::endl;
                    threatFile.close();
                    threatBuffer.push_back("[SHA-256 MATCH] " + strPath);
                    // removeFile(strPath);
                    continue; // Skip further checks for deleted file
                }

                // File change detection comes last
                std::string fileHash = fileToSHA256(strPath);
                if (checkFileChange(strPath, fileHash)) {
                    //std::cout << "Warning: File has changed since last scan! -> " << strPath << std::endl;
                    std::ofstream threatFile("logs/threat_log.txt", std::ios::app);
                    threatFile << "[HASH CHANGE] " << strPath << std::endl;
                    threatFile.close();
                    threatBuffer.push_back("[HASH CHANGE] " + strPath);
                    // Optionally prompt for quarantine, re-scan, or alert
                }



            }

        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            continue;
        }
    }
}

void Scan::fastScan()
{
    char username[1024];
    DWORD username_len = sizeof(username);
    GetUserNameA(username, &username_len);

    std::string path = "C:\\Users\\"+ std::string(username)+"\\Downloads";

    char buffer[80];

    const size_t maxFileSize = 100 * 1024 * 1024; // 100MB

    for (auto it = std::filesystem::recursive_directory_iterator(
        path, std::filesystem::directory_options::skip_permission_denied);
        it != std::filesystem::recursive_directory_iterator(); ++it)
    {
        try {

            std::time_t rawtime = std::time(nullptr);
            std::tm timeinfo;
            localtime_s(&timeinfo, &rawtime); // safer version for Windows

            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
            std::string currentTime(buffer);

            std::ofstream logFile("logs/logfile.txt", std::ios::app);
            std::string strPath = it->path().string();
            logFile << currentTime << "," << strPath << std::endl;
            logFile.close();
            logBuffer.push_back(strPath);

            if (it->is_regular_file())
            {
                if (std::filesystem::file_size(it->path()) > maxFileSize) {
                    //std::cout << "Skipping scanning file larger than selected size: " << it->path() << std::endl;
                    continue;
                }


                if (hasMagicBytes(strPath)) {
                    //std::cout << "File has a different magic byte than extension name! -> " << strPath << std::endl;
                    std::ofstream threatFile("logs/threat_log.txt", std::ios::app);
                    threatFile << "[MAGIC BYTE MISMATCH] " << strPath << std::endl;
                    threatFile.close();
                    threatBuffer.push_back("[MAGIC BYTE MISMATCH] " + strPath);
                    // Optionally quarantine or rescan
                }

                // SHA-256 check comes second
                if (compareSHA256File(strPath)) {
                    //std::cout << "Warning: File is in SHA-256 virus database! -> " << strPath << std::endl;
                    //std::cout << "Removing file -> " << strPath << std::endl;
                    std::ofstream threatFile("logs/threat_log.txt", std::ios::app);
                    threatFile << "[SHA-256 MATCH] " << strPath << std::endl;
                    threatFile.close();
                    threatBuffer.push_back("[SHA-256 MATCH] " + strPath);
                    // removeFile(strPath);
                    continue; // Skip further checks for deleted file
                }

                // File change detection comes last
                std::string fileHash = fileToSHA256(strPath);
                if (checkFileChange(strPath, fileHash)) {
                    //std::cout << "Warning: File has changed since last scan! -> " << strPath << std::endl;
                    std::ofstream threatFile("logs/threat_log.txt", std::ios::app);
                    threatFile << "[HASH CHANGE] " << strPath << std::endl;
                    threatFile.close();
                    threatBuffer.push_back("[HASH CHANGE] " + strPath);
                    // Optionally prompt for quarantine, re-scan, or alert
                }



            }

        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            continue;
        }
    }
}

void Scan::removeFile(const std::string& filePath)
{  
    remove(filePath.c_str());
}

std::string Scan::GetProcessName(DWORD pid)
{
    std::string processName = "Unknown";

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (hProcess)
    {
        char buffer[MAX_PATH];
        DWORD size = MAX_PATH;

        if (QueryFullProcessImageNameA(hProcess, 0, buffer, &size))
        {
            std::string fullPath(buffer);
            size_t pos = fullPath.find_last_of("\\/");
            if (pos != std::string::npos)
                processName = fullPath.substr(pos + 1);
            else
                processName = fullPath;
        }
        CloseHandle(hProcess);
    }

    return processName;
}

void Scan::ScanMemory()
{
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        std::cerr << "Failed to enumerate processes." << std::endl;
        return;
    }

    cProcesses = cbNeeded / sizeof(DWORD);

    for (size_t i = 0; i < cProcesses; ++i)
    {
        DWORD pid = aProcesses[i];
        if (pid == 0)
            continue;

        HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE, FALSE, pid);
        if (!hProcess)
            continue;

        std::cout << "Scanning PID: " << pid << std::endl;

        MEMORY_BASIC_INFORMATION mbi;
        unsigned char* addr = nullptr;

        while (VirtualQueryEx(hProcess, addr, &mbi, sizeof(mbi)) == sizeof(mbi))
        {
            // Only scan committed, readable memory that's not guarded or inaccessible
            if ((mbi.State == MEM_COMMIT) &&
                !(mbi.Protect & PAGE_GUARD) &&
                !(mbi.Protect & PAGE_NOACCESS))
            {
                std::vector<char> buffer(mbi.RegionSize);
                SIZE_T bytesRead;

                if (ReadProcessMemory(hProcess, mbi.BaseAddress, buffer.data(), mbi.RegionSize, &bytesRead))
                {
                    std::string processName = GetProcessName(pid);

                    // Check if the memory region has EXECUTE + WRITE permissions
                    bool hasExecWrite = (mbi.Protect & PAGE_EXECUTE_READWRITE) || (mbi.Protect & PAGE_EXECUTE_WRITECOPY);

                    // Check if the process is NOT in the ignore list
                    bool isNotIgnored = ignoreProcesses.find(processName) == ignoreProcesses.end();

                    if (hasExecWrite && isNotIgnored) {
                        std::cout << "[!] Suspicious memory protection (EXECUTE + WRITE) at: " << mbi.BaseAddress << std::endl;
                        std::cout << "Process: " << processName << " (PID " << pid << ")" << std::endl;  // <-- added this line

                        std::ofstream log("logs/memory_suspicious_log.txt", std::ios::app);
                        log << "Suspicious region (EXECUTE_WRITE): " << mbi.BaseAddress << "\n";
                        log << "PID: " << pid << " (" << processName << ")" << "\n";
                        log << "Region Size: " << mbi.RegionSize << " bytes\n\n";

                        //TerminateProcess(hProcess, 1);
                    }
                  
                }
            }

            addr += mbi.RegionSize; // Move to next region
        }

        CloseHandle(hProcess);
    }
}



void Scan::getStartupApplications()
{
    char username[1024];
    DWORD username_len = sizeof(username);
    GetUserNameA(username, &username_len);

    std::string userStartupPath = "C:/Users/" + std::string(username) + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup";
    std::string globalStartupPath = "C:/ProgramData/Microsoft/Windows/Start Menu/Programs/Startup";

    std::vector<std::string> currentStartupFiles;

    // Collect current startup files from user startup folder
    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(userStartupPath))
    {
        std::string filePath = dirEntry.path().string();
        if (dirEntry.path().extension() != ".ini")
        {
            currentStartupFiles.push_back(filePath);
        }
    }

    // Collect current startup files from global startup folder
    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(globalStartupPath))
    {
        std::string filePath = dirEntry.path().string();
        if (dirEntry.path().extension() != ".ini")
        {
            currentStartupFiles.push_back(filePath);
        }
    }

    // Ensure startup_log.txt exists (create if missing)
    std::ofstream createFile("logs/startup_log.txt", std::ios::app);
    createFile.close();

    // Read previous startup entries from file
    std::ifstream readFile("logs/startup_log.txt");
    std::string line;
    std::vector<std::string> previousStartupFiles;
    while (std::getline(readFile, line))
    {
        previousStartupFiles.push_back(line);
    }
    readFile.close();

    // Detect and print newly added startup files
    for (const auto& file : currentStartupFiles)
    {
        if (std::find(previousStartupFiles.begin(), previousStartupFiles.end(), file) == previousStartupFiles.end())
        {
            std::cout << "[Warning] " << file << " has been added to startup folders." << std::endl;
        }
    }

    // Overwrite startup_log.txt with current startup files
    std::ofstream writeFile("logs/startup_log.txt", std::ios::trunc);
    for (const auto& file : currentStartupFiles)
    {
        writeFile << file << "\n";
    }
    writeFile.close();
}




bool Scan::hasMagicBytes(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        return false; // File couldn't be opened
    }

    std::string strExtension = std::filesystem::path(filePath).extension().string();

    try {
        uint32_t expectedMagic = extensionMagicMap.at(strExtension);

        // Determine how many bytes to read based on the expected magic value
        uint32_t readMagic = 0;

        if (expectedMagic <= 0xFFFF) {
            // 2-byte magic number (like PE files: 0x5A4D)
            uint16_t magic16;
            file.read(reinterpret_cast<char*>(&magic16), sizeof(magic16));
            if (!file) {
                return false; // Read failed
            }
            readMagic = magic16;
        }
        else {
            // 4-byte magic number
            file.read(reinterpret_cast<char*>(&readMagic), sizeof(readMagic));
            if (!file) {
                return false; // Read failed
            }
        }

        file.close();

        if (expectedMagic == readMagic) {
            //std::cout << "value and magic is the same " << filePath << std::endl;
            return false; // Magic bytes match - file is legitimate
        }
        else {
           // std::cout << "Value not the same for file: " << filePath << std::endl;
           // std::cout << "Expected: 0x" << std::hex << expectedMagic << " Read: 0x" << readMagic << std::dec << std::endl;
            return true; // Magic bytes don't match - potential threat
        }
    }
    catch (const std::exception&) {
        file.close();
        return false; // Extension not found in map or other error
    }
}

bool Scan::checkFileChange(const std::string& filePath, const std::string& hash)
{
    // Ensure the file exists (create if not)
    std::ofstream touchFile("logs/heuristic_log.txt", std::ios::app);
    touchFile.close();

    // Read all lines
    std::ifstream readfile("logs/heuristic_log.txt");
    std::vector<std::string> lines;
    std::string line;
    bool found = false;
    bool changed = false;

    while (std::getline(readfile, line))
    {
        size_t comma = line.find(',');
        if (comma == std::string::npos)
        {
            lines.push_back(line);
            continue;
        }

        std::string existingPath = line.substr(0, comma);
        std::string existingHash = line.substr(comma + 1);

        if (existingPath == filePath)
        {
            found = true;
            if (existingHash != hash)
            {
                // Update line with new hash
                line = filePath + "," + hash;
                changed = true;
            }
            // else no change, keep line as is
        }
        lines.push_back(line);
    }
    readfile.close();

    if (!found)
    {
        // Append new entry
        lines.push_back(filePath + "," + hash);
        changed = false; // New file entry, not considered a change
        std::cout << "Writing new file entry!" << std::endl;
    }
    else if (changed)
    {
        std::cout << "File info updated for " << filePath << std::endl;
    }
    else
    {
        // No change detected
        return false;
    }

    // Write back all lines to file
    std::ofstream writefile("logs/heuristic_log.txt", std::ios::trunc);
    for (const auto& l : lines)
    {
        writefile << l << "\n";
    }
    writefile.close();

    return changed;
}

std::string Scan::fileToSHA256(const std::string& filePath)
{
    FILE* file = nullptr;
    unsigned char buf[8192];
    size_t len;
    std::ostringstream oss;

    // Use EVP instead of SHA256_CTX
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        std::cerr << "Failed to create EVP_MD_CTX" << std::endl;
        return "";
    }


    if (fopen_s(&file, filePath.c_str(), "rb") != 0)
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        EVP_MD_CTX_free(ctx);
        return "";
    }

    unsigned char output[EVP_MAX_MD_SIZE];
    unsigned int outputLen = 0;

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        std::cerr << "EVP_DigestInit_ex failed" << std::endl;
        fclose(file);
        EVP_MD_CTX_free(ctx);
        return "";
    }

    while ((len = fread(buf, 1, sizeof(buf), file)) != 0) {
        if (EVP_DigestUpdate(ctx, buf, len) != 1) {
            std::cerr << "EVP_DigestUpdate failed" << std::endl;
            fclose(file);
            EVP_MD_CTX_free(ctx);
            return "";
        }
    }

    fclose(file);

    if (EVP_DigestFinal_ex(ctx, output, &outputLen) != 1) {
        std::cerr << "EVP_DigestFinal_ex failed" << std::endl;
        EVP_MD_CTX_free(ctx);
        return "";
    }

    EVP_MD_CTX_free(ctx);

    oss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < outputLen; ++i)
        oss << std::setw(2) << static_cast<int>(output[i]);

    return oss.str();
}


bool Scan::compareSHA256File(const std::string& hashPath)
{
    std::string hash = fileToSHA256(hashPath);
    //std::cout << "Path: "<<hashPath<< " Hash: " << hash << std::endl;
    if (hash_set.find(hash) != hash_set.end())
    {
        return true;
    }

    return false;
}


bool Scan::compareSHA256Str(const std::string& hashStr)
{
    if (hash_set.find(hashStr) != hash_set.end())
    {
        
        return true;
    }

    return false;
}


void Scan::load_hashes()
{
    std::ifstream file("logs/full_sha256.txt");
    std::string line;

    while (std::getline(file, line))
    {
        size_t start = line.find_first_not_of(" \t\r\n");
        size_t end = line.find_last_not_of(" \t\r\n");
        std::string trimmed = (start == std::string::npos || end == std::string::npos)
                                ? ""
                                : line.substr(start, end - start + 1);


        if (!trimmed.empty())
        {
            hash_set.insert(trimmed);
        }
    }
}

