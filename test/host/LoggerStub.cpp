#include "os/Logger.hpp"

std::queue<String> Logger::offlineLogQueue;
long Logger::logCounter = 0;

void Logger::processMessage(String&) {}
void Logger::log(String) {}
