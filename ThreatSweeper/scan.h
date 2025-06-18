#ifndef SCAN_H
#define SCAN_H

#include <filesystem>
#include <iostream>
#include <fstream>
#include <openssl/evp.h>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

class Scan
{
public:
    Scan();
    void recursiveIterator();
    std::string fileToSHA256(const std::string& filePath);
    bool compareSHA256File(const std::string& hashPath);
    bool compareSHA256Str(const std::string& hashStr);
private:
    void load_hashes();
    bool checkFileChange(const std::string& filePath, const std::string& hash);
    std::string filePermission(const std::string& filePath);
    bool hasMagicBytes(const std::string& filePath);
    void removeFile(const std::string& filePath);
private:
    std::unordered_set<std::string> hash_set;
    std::unordered_map<std::string, uint32_t> extensionMagicMap = {
    {".exe",   0x5A4D},       // MZ (4D 5A)
    {".dll",   0x5A4D},       // MZ (same as .exe)
    {".png",   0x474E5089},   // 89 50 4E 47
    {".jpg",   0xE0FFD8FF},   // FF D8 FF E0
    {".jpeg",  0xE0FFD8FF},   // FF D8 FF E0
    {".gif",   0x38374947},   // GIF89a (47 49 46 38 39 61)
    {".bmp",   0x4D42},       // BM (42 4D)
    {".pdf",   0x46445025},   // %PDF (25 50 44 46)
    {".zip",   0x04034B50},   // PK.. (50 4B 03 04)
    {".docx",  0x04034B50},   // DOCX is a ZIP
    {".xlsx",  0x04034B50},   // XLSX is a ZIP
    {".pptx",  0x04034B50},   // PPTX is a ZIP
    {".rar",   0x21726152},   // RAR! (52 61 72 21)
    {".7z",    0x377ABCAF},   // 7z archive (37 7A BC AF)
    {".gz",    0x8B1F},       // 1F 8B
    {".xz",    0xFD377A58},   // FD 37 7A 58
    {".mp3",   0xFFE3FFFB},   // FF FB (or FF F3, FF F2)
    {".wav",   0x46464952},   // RIFF (52 49 46 46)
    {".avi",   0x46464952},   // RIFF (52 49 46 46)
    {".mp4",   0x66747970},   // ftyp (66 74 79 70)
    {".mov",   0x66747970},   // QuickTime format (ftyp)
    {".flv",   0x464C5601},   // FLV (46 4C 56 01)
    {".wmv",   0x75B22630},   // ASF/WMV (30 26 B2 75)
    {".iso",   0x00000101},   // ISO9660, weak signature
    {".elf",   0x464C457F},   // ELF (7F 45 4C 46)
    {".class", 0xCAFEBABE},   // Java class (CA FE BA BE)
    {".dex",   0x03005A64},   // DEX (64 65 78 0A)
    {".psd",   0x38425053},   // PSD (38 42 50 53)
    {".torrent", 0x64313A},   // d1: (very loose match)
    {".xml",   0x003C003F},   // <? (3C 3F in UTF-16)
    {".html",  0x203C3C21},   // <! (21 3C 3C)
    {".rtf",   0x7B5C7274},   // {\rt (7B 5C 72 74)
    {".ps",    0x25215053},   // %!PS (25 21 50 53)
    {".sqlite",0x53514C69},   // SQLite format 3 (53 51 4C 69)
    {".db",    0x53514C69},   // Same as SQLite
    };


};

#endif
