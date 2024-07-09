#include <iostream>
#include <restbed>
#include <memory>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>

using namespace std;
using namespace restbed;

//time function
void print_time_point(const chrono::time_point<chrono::system_clock>& time_point) {
    time_t time_t_value = chrono::system_clock::to_time_t(time_point);
    tm tm_value = *localtime(&time_t_value);
    cout << put_time(&tm_value, "%Y-%m-%d %H:%M:%S") << endl;
}

//Http function
chrono::time_point<chrono::system_clock> get_handler(const string& url, const string& file_name, int num,
    chrono::time_point<chrono::system_clock> prev_time, int interval) {
    chrono::time_point<chrono::system_clock> fetch_time = chrono::system_clock::now();
    string ip = url.substr(7, url.find(':'));

    auto request = make_shared<Request>(Uri(url));
    request->set_header("Content-Type", "image/jpeg");
    request->set_header("Host", ip);
    request->set_method("GET");

    auto response = Http::sync(request);
    if (response->get_status_code() / 100 == 2) {
        size_t content_length = response->get_header("Content-Length", 0);

        if (num != 0) {
            fetch_time = chrono::system_clock::now();
            auto elapsed_milliseconds = chrono::duration_cast<chrono::milliseconds>(fetch_time - prev_time);
            long elapsed_count = elapsed_milliseconds.count();
            if (elapsed_count < interval) {
                this_thread::sleep_for(chrono::milliseconds(interval - elapsed_count));
            }
        }
        fetch_time = chrono::system_clock::now();
        Http::fetch(content_length, response);

        const auto& body = response->get_body();
        vector<uint8_t> data(body.begin(), body.end());

        string file_name_with_num = file_name.substr(0, file_name.find('.')) + to_string(num) + file_name.substr(file_name.find('.'));
        int fd = open(file_name_with_num.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

        if (fd != -1) {
            if (write(fd, data.data(), data.size()) == -1) {
                cerr << "Can't write to file." << endl;
            }
            close(fd);
        } else {
            cerr << "Can't open file." << endl;
        }
    } else {
        cerr << "Can't get response. Status code is: " << response->get_status_code() << endl;
    }
    return fetch_time;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Invalid number of arguments." << endl;
        return -1;
    }

    const string url = argv[1];
    const string file_name = argv[2];
    const int interval = stoi(argv[3]); // in milliseconds
    const int num_photos = stoi(argv[4]);

    chrono::time_point<chrono::system_clock> time1 = chrono::system_clock::now();
    for (int num = 0; num < num_photos; ++num) {
        time1 = get_handler(url, file_name, num, time1, interval);
    }

    return 0;
}
