#include <iostream>

#include "window.h"
#include "downloadhash.h"
#include "scan.h"



int main(int argc, char** argv) 
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
    (DONE) Real-time file system monitoring
    (DONE) Heuristic detection
    (DONE) Memory monitor (scan RAM for threats)

    (PARTIALLY DONE) GUI (OpenGL integration)
    (PARTIALLY DONE) Autorun (monitor startup apps/processes)
    
    (NOT IMPLEMENTED) Quarantine systems (isolate/remove threats)
    (NOT IMPLEMENTED) Sandbox execution (run suspicious code safely)
    (NOT IMPLEMENTED) Behavior analysis (track program actions) 

    (MAYBE IMPLEMENT) Maskin learning (detect unknown malware)
    */

    //Scan scan;
    //scan.ScanMemory();
    //std::string path = "/home/hugo/projects/AntiVirus/.vscode/settings.json";
    //std::string hash = "5a08a0a77df1c63fed7063ce1f7ce936228937f28d07708a856c9dd24ca22a1d";
    //std::cout<<scan.fileToSHA256("Trojan.exe") << std::endl;
    //std::cout<<scan.compareSHA256Str(hash)<<std::endl;

}