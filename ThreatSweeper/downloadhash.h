#ifndef DOWNLOADHASH_H
#define DOWNLOADHASH_H

#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream> 
#include <zip.h>

class DownloadHash
{
public:
    static size_t write_data(void* contents, size_t size, size_t nmemb, void* userp);
    void downloadhash();
    void unzip();

private:
    CURL* curl;
    FILE* file;
    zip* archive;
    CURLcode res;
    const char* shaName = "full_sha256.txt";
    const char* fileName = "download.zip";
    const char* SHA256_FULL_URL = "https://bazaar.abuse.ch//export/txt/sha256/full/";
    
};

#endif