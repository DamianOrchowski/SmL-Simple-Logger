#include <Logger.hpp>
#include <sstream>

std::shared_ptr<Logger> Logger::_instance;
std::string Logger::_logFileOpeningLine; 
bool Logger::_logDate = false;