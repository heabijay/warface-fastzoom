#include <mutex>
#include <iostream>

std::mutex loggerMtx;
void logger(const char* sender, const char* msg) {
    time_t now = time(NULL);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d.%m.%Y %I:%M:%S %p", &tstruct);

    loggerMtx.lock();
    std::cout << "[" << buf << "] " << sender << ": " << msg << std::endl;
    loggerMtx.unlock();
}

void logger(const char* sender, std::string msg) {
    logger(sender, msg.c_str());
}

void logger(std::string sender, std::string msg) {
    logger(sender.c_str(), msg.c_str());
}

void logger(std::string sender, const char* msg) {
    logger(sender.c_str(), msg);
}