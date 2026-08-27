#ifndef __LOGGER__
#define __LOGGER__

#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
#include <filesystem>

namespace fs = std::filesystem;

class Logger
{
private:
    static Logger *_instance;

    const char* _logFileName;
    std::fstream _logFile;

    const char* format_file_name() {
        time_t timestamp = time(&timestamp);
        struct tm now = *localtime(&timestamp);

        
    }

public:
    Logger()
    {
    }

    ~Logger()
    {
    }

    static Logger *getInstance()
    {
        if (_instance == nullptr)
        {
            _instance = new Logger();
        }
        return _instance;
    }

    void CloseLogFile();

};

#endif