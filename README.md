# Windows AntiVirus

## Project Overview
This is an **improved version** of my previous antivirus applications:  
- [First AntiVirus Project](https://github.com/HUGOW04/TuAntivirus)  
- [Second AntiVirus Project](https://github.com/HUGOW04/AntiVirus)  

This project is a **Windows-based malware scanner** developed in **C++ with OpenGL**, using **SHA-256 hashing** for secure file comparison. It provides a modern interface, improved performance, and stronger detection capabilities compared to previous versions.

---

## Implemented Features
- **Full Scan** – Thoroughly scans the entire system for known malware and threats.  
- **Fast Scan** – Quickly scans critical system areas for efficient detection.  
- **Memory Scan** – Analyzes running processes and memory for hidden threats.  
- **File Scan & Drag-and-Drop** – Scan individual files or drag them into the app for instant analysis.  
- **Signature scanning** using **SHA-256 hashes** for files.  
- **Optimized hash lookup** with `std::unordered_set` → **O(1)** average lookup time.  
- **Real-time autorun checks** to monitor startup programs.  
- **Heuristic detection** to identify suspicious behaviors and unknown malware.  
- **False positive reduction** via whitelisting, file type verification, and signature checks.  
- **User-friendly reporting & logging** of all scans and detections.  
- **System tray integration** with CPU and memory statistics.  
- **Modern graphical interface** fully implemented in OpenGL.  
- **Desktop notifications** for scan results.  

---

## Future Improvements
Planned enhancements:  
- Network scanning using **WinAPI hooks**.  
- Real-time detection of **newly added files** before execution.  
- Phishing email protection with **Have I Been Pwned** integration.  
- Sandbox execution for suspicious programs.  
- AI/ML-based detection of unknown threats.  
- Quarantine system to isolate/remove malware.  
- Kernel-level antivirus integration for deeper protection.  
- Automatic virus definition updates.  

---

## Screenshots
<p align="center">
  <img width="900" height="475" alt="Scan Interface" src="https://github.com/user-attachments/assets/fc922aae-9f29-48d2-bfed-2c38f47cf5d4"/>
  <img width="900" height="475" alt="Overview Dashboard" src="https://github.com/user-attachments/assets/8041c5f2-3b8e-4a1b-88ea-8ff96af96684" />
</p>

<p align="center">
   <img width="900" height="475" alt="Performance Monitor" src="https://github.com/user-attachments/assets/c9ca57a1-57d0-429d-916a-73b2d9974c9f"/>
</p>

---

This version builds on my earlier antivirus projects and evolves them into a **more complete, security-suite-like application** with scanning, performance monitoring, and an OpenGL-powered UI.
