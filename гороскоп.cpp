#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// Функция записи ответа
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// HTTP GET запрос
string getHoroscope(string sign, string day) {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    string url = "https://ohmanda.com/api/horoscope/" + sign + "/" + day + "/";

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "Request error: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

// Красивый вывод JSON
void printPretty(string response) {
    try {
        json data = json::parse(response);

        cout << "Date: " << data["date"] << endl;
        cout << "Sign: " << data["sign"] << endl;
        cout << "------------------------------------\n";
        cout << data["horoscope"] << endl;
    }
    catch (...) {
        cout << "Error parsing JSON!\n";
        cout << response << endl;
    }
}

// UI
void printHeader() {
    cout << "====================================\n";
    cout << "        DAILY HOROSCOPE 🌙\n";
    cout << "====================================\n\n";
}

int main() {
    setlocale(LC_ALL, "");

    string signs[] = {
        "aries", "taurus", "gemini", "cancer",
        "leo", "virgo", "libra", "scorpio",
        "sagittarius", "capricorn", "aquarius", "pisces"
    };

    printHeader();

    cout << "Choose your zodiac sign:\n\n";

    for (int i = 0; i < 12; i++) {
        cout << i + 1 << ". " << signs[i] << endl;
    }

    int choice;
    cout << "\nEnter number: ";
    cin >> choice;

    if (choice < 1 || choice > 12) {
        cout << "Invalid choice!\n";
        return 0;
    }

    string selectedSign = signs[choice - 1];

    cout << "\nFetching horoscope...\n";

    string today = getHoroscope(selectedSign, "today");
    string tomorrow = getHoroscope(selectedSign, "tomorrow");

    cout << "\n🔮 HOROSCOPE FOR " << selectedSign << "\n";

    cout << "\n========== TODAY ==========\n";
    printPretty(today);

    cout << "\n========== TOMORROW ==========\n";
    printPretty(tomorrow);

    cout << "\n====================================\n";
    cout << "        HAVE A NICE DAY ✨\n";
    cout << "====================================\n";

    return 0;
}