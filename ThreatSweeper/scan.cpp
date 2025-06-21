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


    std::ofstream logFile("logfile.txt");
    const size_t maxFileSize = 100 * 1024 * 1024; // 100MB

    for (auto it = std::filesystem::recursive_directory_iterator(
        path, std::filesystem::directory_options::skip_permission_denied);
        it != std::filesystem::recursive_directory_iterator(); ++it)
    {
        try {
            std::string strPath = it->path().string();
            logFile << strPath << std::endl;
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
                if (std::filesystem::file_size(it->path()) > maxFileSize)
                    continue;
                if (hasMagicBytes(strPath))
                {
                    // maybe prompt quarantine, re-scan, etc.
                }

                
                if (compareSHA256File(strPath))
                {
                    std::cout << "Warning: File is in SHA-256 virus database! -> " << strPath << std::endl;
                    std::cout << "Removing file ->" << strPath << std::endl;
                    removeFile(strPath); // Remove infected file
                    continue; // Skip further checks for deleted file
                }

                std::string fileHash = fileToSHA256(strPath);
                if (checkFileChange(strPath, fileHash)) {
                    std::cout << "Warning: File has changed since last scan! -> " << strPath << std::endl;
                    // maybe prompt quarantine, re-scan, etc.
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

void Scan::getStartupApplications()
{
    char username[1024];
    DWORD username_len = 1024;
    GetUserNameA(username, &username_len);
    

    std::string startUpPath = "C:/Users/" + std::string(username) + std::string("/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup");
    std::cout << startUpPath << std::endl;

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(startUpPath))
    {
        std::string fileName = dir_entry.path().filename().string();
        std::cout << fileName << std::endl;
    }

    std::string globalStartup = "C:/ProgramData/Microsoft/Windows/Start Menu/Programs/Startup";
    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(globalStartup))
    {
        std::string fileName = dir_entry.path().filename().string();
        std::cout << fileName << std::endl;
    }

    // LÄGG TILL SÅ DEN JÄMFÖR MED EN FIL FRÅN FÖRRA GÅNGEN
    
}

std::string Scan::filePermission(const std::string& filePath)
{
    std::string permStr;

    std::ifstream in(filePath);
    permStr += in.good() ? 'r' : '-';
    in.close();

    // Test write (append-mode, no truncation)
    std::ofstream out(filePath, std::ios::app);
    permStr += out.good() ? 'w' : '-';
    out.close();

    // Test execute (based on file extension)
    std::string ext = std::filesystem::path(filePath).extension().string();
    permStr += (ext == ".exe" || ext == ".bat" || ext == ".cmd" || ext == ".com") ? 'x' : '-';

    return permStr;
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
            std::cout << "Value not the same for file: " << filePath << std::endl;
            std::cout << "Expected: 0x" << std::hex << expectedMagic
                << " Read: 0x" << readMagic << std::dec << std::endl;
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
    std::ofstream touchFile("heuristic_log.txt", std::ios::app);
    touchFile.close();

    std::string permission = filePermission(filePath);

    // Read all lines
    std::ifstream readfile("heuristic_log.txt");
    std::vector<std::string> lines;
    std::string line;
    bool found = false;
    bool changed = false;

    while (std::getline(readfile, line))
    {
        size_t firstComma = line.find(',');
        if (firstComma == std::string::npos)
        {
            lines.push_back(line);
            continue;
        }
        size_t secondComma = line.find(',', firstComma + 1);
        if (secondComma == std::string::npos)
        {
            lines.push_back(line);
            continue;
        }

        std::string existingPath = line.substr(0, firstComma);
        std::string existingHash = line.substr(firstComma + 1, secondComma - firstComma - 1);
        std::string existingPerm = line.substr(secondComma + 1);

        if (existingPath == filePath)
        {
            found = true;
            if (existingHash != hash || existingPerm != permission)
            {
                // Update line with new hash and permission
                line = filePath + "," + hash + "," + permission;
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
        lines.push_back(filePath + "," + hash + "," + permission);
        changed = false; // It's new, so no "change", just added
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
    std::ofstream writefile("heuristic_log.txt", std::ios::trunc);
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
    std::cout << "Path: "<<hashPath<< " Hash: " << hash << std::endl;
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
        std::cout << "Found: " << hashStr << std::endl;
        return true;
    }

    return false;
}


void Scan::load_hashes()
{
    std::ifstream file("full_sha256.txt");
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

