#include <Logger.hpp>
#include <sstream>

Logger* Logger::_instance;
std::string Logger::_logFileOpeningLine; 
bool Logger::_logDate = false;