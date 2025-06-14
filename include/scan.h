#ifndef SCAN_H
#define SCAN_H

#include <filesystem>
#include <iostream>
#include <fstream>
#include <openssl/sha.h>
#include <sstream>
#include <unordered_set>

class Scan
{
public:
    Scan();
    void recursiveIterator();
    std::string fileToSHA256(std::string& filePath);
    bool compareSHA256File(std::string& hashPath);
    bool compareSHA256Str(std::string& hashStr);
    void load_hashes();

private:
    std::unordered_set<std::string> hash_set;


};

#endif