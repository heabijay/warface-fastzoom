#include <string>

void logger(const char* sender, const char* msg);
void logger(const char* sender, std::string msg);
void logger(std::string sender, std::string msg);
void logger(std::string sender, const char* msg);