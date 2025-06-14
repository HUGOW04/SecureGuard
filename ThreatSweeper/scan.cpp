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
    std::string skipDirs[] = { "/proc","/sys","/dev","/run" };
    int skipCount = sizeof(skipDirs) / sizeof(skipDirs[0]);
#endif

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

