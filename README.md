# Windows AntiVirus

## Project Overview
This is an improved version of my previous antivirus application.  
[View Previous AntiVirus Project](https://github.com/HUGOW04/TuAntivirus)

This project is a Windows-based malware scanner developed in C++ with OpenGL, using SHA-256 hashing for secure file comparison. It provides an enhanced interface and improved detection capabilities over the previous version.

---

## Implemented Features
- **Signature scanning** using SHA-256 hashes for files.  
- **Optimized hash lookup**: Loads the hash database into memory using a `std::unordered_set` and checks hashes using constant-time average lookup (`O(1)`), instead of looping through the file each time (`O(n)`).  
- **Memory scanning** for known threats.  
- **Real-time autorun process checks** to monitor startup programs.  
- **Heuristic detection** to identify suspicious behaviors and unknown malware.  
- **User-friendly reporting and logging** for all scan actions and detected threats.  
- **System tray integration** showing CPU and memory statistics.  
- **Graphical interface** fully implemented with OpenGL.  
- **Desktop notifications** for scan results.  

---

## Future Improvements
Planned features include:
- Network scanning using WinAPI hooks.  
- Detection of newly added files before execution with quarantine functionality.  
- Phishing email protection integrated with "Have I Been Pwned".  
- Sandbox execution and behavioral analysis for suspicious programs.  
- Neural network or machine learning integration to detect unknown threats.  
- Full quarantine system to isolate or remove detected malware.  
- Kernel-level antivirus integration for deeper system protection.  
- Automatic updates for virus definitions.  

---

## Screenshots
<p align="center">
  <img src="https://github.com/user-attachments/assets/fc922aae-9f29-48d2-bfed-2c38f47cf5d4" alt="AntiVirus Main Interface" width="100%"/>
</p>
