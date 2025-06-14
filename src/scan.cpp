#include "scan.h"

Scan::Scan()
{
    load_hashes();
}

void Scan::recursiveIterator()
{
#ifdef _WIN32
    std::string path = "C:\\";
    std::string skipDirs[] = {
        "C:\\Windows\\System32\\DriverStore",
        "C:\\Windows\\Temp",
        "C:\\ProgramData",
        "C:\\$Recycle.Bin",
        "C:\\System Volume Information"
    };
    int skipCount = sizeof(skipDirs) / sizeof(skipDirs[0]);
#endif

#ifdef __unix__
    std::string path = "/";
    std::string skipDirs[] = {"/proc","/sys","/dev","/run"};
    int skipCount = sizeof(skipDirs) / sizeof(skipDirs[0]);
#endif

    std::ofstream logFile("logfile.txt");
    const size_t maxFileSize = 100 * 1024 * 1024; // 100MB

    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(
             path, std::filesystem::directory_options::skip_permission_denied))
    {
        try {
            std::string strPath = dirEntry.path().string();
            logFile << strPath << std::endl;

            bool shouldSkip = false;
            for (int i = 0; i < skipCount; i++)
            {
                if (strPath.compare(0, skipDirs[i].size(), skipDirs[i]) == 0)
                {
                    shouldSkip = true;
                    break;
                }
            }

            if (shouldSkip)
                continue;

            if (std::filesystem::is_regular_file(dirEntry.path()))
            {
                if (std::filesystem::file_size(dirEntry.path()) > maxFileSize)
                    continue;

                compareSHA256File(strPath);
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            continue;
        }
    }
}


std::string Scan::fileToSHA256(std::string& filePath)
{
    FILE *file;

    unsigned char buf[8192];
    unsigned char output[SHA256_DIGEST_LENGTH];
    size_t len;
    std::ostringstream oss;
    SHA256_CTX sha256;
    file = fopen(filePath.c_str(), "rb");

    if(file == NULL)
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
    else
    {
        SHA256_Init(&sha256);
        while((len = fread(buf, 1, sizeof buf, file)) != 0)
            SHA256_Update(&sha256, buf, len);
        fclose(file);
        SHA256_Final(output, &sha256);
        
        
        oss << std::hex << std::setfill('0');
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        {
            oss << std::setw(2) << static_cast<int>(static_cast<unsigned char>(output[i]));
        }

    }
    return oss.str();
}

bool Scan::compareSHA256File(std::string& hashPath)
{
    std::string hash = fileToSHA256(hashPath);
    std::cout << "Hash: " <<hash<<" Path: "<<hashPath<<std::endl;
    if (hash_set.find(hash) != hash_set.end())
    {
        std::cout << "Found: " <<hash<<" Path: "<<hashPath<<std::endl;
        return true;
    }

    return false;
}


bool Scan::compareSHA256Str(std::string& hashStr)
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

