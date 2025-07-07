#include <iostream>

#include "window.h"
#include "scan.h"

int main(int argc, char** argv) 
{

    Window window(900,500,"ThreatSweeper");
    window.handleEvents();

    /*
    TO DO
    (DONE) Signature scanning
    (DONE) Real-time file system monitoring
    (DONE) Heuristic detection
    (DONE) Memory monitor (scan RAM for threats)
    (DONE) Autorun (monitor startup apps/processes)
    (DONE) System tray (CPU/Memory stats)
    (DONE) FIX ALLOCATED MEMORY TO CLEANUP AT CLOSE!

    (PARTIALLY DONE) GUI (OpenGL integration)
    
    (NOT IMPLEMENTED) Network Scan/winapi rules / like wireshark :)
   
    (NOT IMPLEMENTED) FIND NEW ADDED FILES (WINDOWS API HOOK)
    (NOT IMPLEMENTED) Desktop Notifications
    (NOT IMPLEMENTED) physing mail protection/ leaked mail checker in have I been phoned website

    (NOT IMPLEMENTED) Quarantine systems (isolate/remove threats)
    (NOT IMPLEMENTED) Sandbox execution (run suspicious code safely)
    (NOT IMPLEMENTED) Behavior analysis (track program actions) 

    (MAYBE IMPLEMENT) Maskin learning (detect unknown malware)
    */
}