#include <iostream>
#include <GLFW/glfw3.h>

#include "window.h"
#include "downloadhash.h"
#include "scan.h"



int main()
{
    /*
    DownloadHash hash;
    hash.downloadhash();
    hash.unzip();
    */

    Window window(800,600,"ThreatSweeper");
    window.handleEvents();

    /*
    TO DO
    (DONE) Signature scanning
    (NOT IMPLEMENTED) Heuristic detection
    (NOT IMPLEMENTED) Quarantine systems
    (PARTIALLY DONE) Real-time file system monitoring
    (PARTIALLY DONE) GUI (OpenGL integration)
    (NOT IMPLEMENTED) Sandbox execution
    */

    //Scan scan;
    //scan.recursiveIterator();
    //std::string path = "/home/hugo/projects/AntiVirus/.vscode/settings.json";
    //Hash: 47aef119441d99d27b882550dcc1beafcc0eecb581ff1a59e8f6786c7ac5e1c3 Path: /home/hugo/projects/AntiVirus/.vscode/settings.json
    //std::string hash = "47aef119441d99d27b882550dcc1beafcc0eecb581ff1a59e8f6786c7ac5e1c3";
    //std::cout<<scan.fileToSHA256(path)<<std::endl;
    //std::cout<<scan.compareSHA256Str(hash)<<std::endl;

}
