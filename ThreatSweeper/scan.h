#ifndef SCAN_H
#define SCAN_H

#include <filesystem>
#include <iostream>
#include <fstream>
#include <openssl/evp.h>
#include <sstream>
#include <unordered_set>

class Scan
{
public:
    Scan();
    void recursiveIterator();
    std::string fileToSHA256(const std::string& filePath);
    bool compareSHA256File(const std::string& hashPath);
    bool compareSHA256Str(const std::string& hashStr);
    void load_hashes();
    bool checkFileChange(const std::string& filePath, const std::string& hash);
    std::string filePermission(const std::string& filePath);
private:
    std::unordered_set<std::string> hash_set;


};

#endif
