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
    
    Window window(800,600,"ThreatSweeper");
    window.handleEvents();
    */


    /*
    TO DO
    (DONE) Signature scanning
    (DONE) Real-time file system monitoring
    (DONE) Heuristic detection

    (PARTIALLY DONE) GUI (OpenGL integration)

    (NOT IMPLEMENTED) Quarantine systems (isolate/remove threats)
    (NOT IMPLEMENTED) Sandbox execution (run suspicious code safely)
    (NOT IMPLEMENTED) Autorun (monitor startup apps/processes)
    (NOT IMPLEMENTED) Memory monitor (scan RAM for threats)
    (NOT IMPLEMENTED) Behavior analysis (track program actions)

    (MAYBE IMPLEMENT) Maskin learning (detect unknown malware)
    */

    Scan scan;
    scan.recursiveIterator();
    //std::string path = "/home/hugo/projects/AntiVirus/.vscode/settings.json";
    //std::string hash = "5a08a0a77df1c63fed7063ce1f7ce936228937f28d07708a856c9dd24ca22a1d";
    //std::cout<<scan.fileToSHA256("Trojan.exe") << std::endl;
    //std::cout<<scan.compareSHA256Str(hash)<<std::endl;

}
