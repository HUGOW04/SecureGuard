#include "downloadhash.h"

size_t DownloadHash::write_data(void* ptr, size_t size, size_t nmemb, void* userp) {
    return fwrite(ptr, size, nmemb, static_cast<FILE*>(userp));
}

void DownloadHash::downloadhash()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        FILE* file = nullptr;
        if (fopen_s(&file, fileName, "wb") == 0 && file != nullptr)
        {
            curl_easy_setopt(curl, CURLOPT_URL, SHA256_FULL_URL);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }

            fclose(file);
        }
        else
        {
            fprintf(stderr, "Failed to open file: %s\n", fileName);
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}


void DownloadHash::unzip()
{
    // Öppna zip-arkivet
    int err = 0;
    archive = zip_open(fileName, 0, &err);
    if (!archive) {
        std::cerr << "Failed to open zip file: " << fileName << std::endl;
        return;
    }

    // Ta första filens namn
    zip_int64_t numFiles = zip_get_num_entries(archive, 0);
    if (numFiles <= 0) {
        std::cerr << "No files found in the zip archive." << std::endl;
        zip_close(archive);
        return;
    }

    struct zip_stat st;
    zip_stat_init(&st);
    if (zip_stat_index(archive, 0, 0, &st) != 0) {
        std::cerr << "Failed to stat first file in zip." << std::endl;
        zip_close(archive);
        return;
    }

    // Öppna filen i zip
    zip_file* zf = zip_fopen_index(archive, 0, 0);
    if (!zf) {
        std::cerr << "Failed to open file inside zip: " << st.name << std::endl;
        zip_close(archive);
        return;
    }

    // Läs innehållet till minne
    std::vector<char> buffer(st.size);
    zip_fread(zf, buffer.data(), st.size);
    zip_fclose(zf);
    zip_close(archive);

    // Konvertera till ström för att behandla rader
    std::istringstream stream(std::string(buffer.begin(), buffer.end()));
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(stream, line)) {
        lines.push_back(line);
    }

    // Skip 9 första och sista raden
    if (lines.size() <= 10) {
        std::cerr << "För få rader i filen efter extrahering." << std::endl;
        return;
    }

    std::ofstream out(shaName, std::ios::trunc);
    if (!out) {
        std::cerr << "Failed to open output file: " << shaName << std::endl;
        return;
    }

    for (size_t i = 9; i < lines.size() - 1; ++i) {
        out << lines[i] << '\n';
    }

    out.close();
    std::cout << "Extracted and cleaned: " << st.name << " → " << shaName << std::endl;
}

