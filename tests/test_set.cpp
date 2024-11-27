#include <gtest/gtest.h>
#include <curl/curl.h>
#include <string>

size_t curl_write_cb(void* contents, size_t size, size_t nmemb, void* userp) {
    std::vector<char>* buffer = reinterpret_cast<std::vector<char>*>(userp);
    buffer->insert(buffer->end(), (char*)contents, (char*)contents + size * nmemb);
    return size * nmemb;
}

std::string curl_process(const std::string& method, const std::string& file_contents, const std::string& url = DEFAULT_URL, const std::string& port = DEFAULT_PORT)
{
    std::ostringstream oss;
    oss << url << ':' << port << '/' << method;
    std::string r_url = oss.str();
    CURL* curl;
    CURLcode res;    

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, r_url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, file_contents.size());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, file_contents.c_str());

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::vector<char> buffer;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::string error_msg = curl_easy_strerror(res);
        throw std::runtime_error("curl : " + error_msg);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return std::string(buffer.begin(), buffer.end());
};

std::string testCompressDecompress(const std::string& str)
{
    return curl_process("decompress", curl_process("compress", str));
};


TEST(TEST, CompressDecompress)
{

    EXPECT_EQ(testCompressDecompress("testing archiving"), "testing archiving");
    EXPECT_NE(testCompressDecompress("testing archiving 1"), "testing archiving 2");
}