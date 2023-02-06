#include <curl/curl.h>
#include <iostream>

size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    static ifstream file("pic.jpg", ios::binary);
    if (!file.good())
        return CURL_READFUNC_ABORT;
    file.read((char*)ptr, size*nmemb);
    return file.gcount();
}

int main() {
    CURL *curl;
    CURLcode res;
    curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Transfer-Encoding: chunked");

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/upload");
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    curl_slist_free_all(headers);
    return 0;
}
