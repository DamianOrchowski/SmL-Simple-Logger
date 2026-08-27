#ifndef __LOGGER__
#define __LOGGER__

#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class Logger
{
private:
    static Logger *_instance;

    std::string _logFileOpeningLine;

    std::string _logFileName;
    std::fstream _logFile;

    fs::path _logsPath = fs::path("logs/");

    struct tm get_time()
    {
        time_t timestamp = time(&timestamp);
        struct tm now = *localtime(&timestamp);
        return now;
    }

    const char *format_file_name(struct tm now)
    {
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H_%M_%S.log", &now);
        return buffer;
    }

    void create_log_file()
    {
        struct tm now = get_time();
        _logFileName = format_file_name(now);
        if (!fs::exists(_logsPath) || !fs::is_directory(_logsPath))
        {
            fs::create_directories(_logsPath);
        }
        fs::path logFilePath = _logsPath / _logFileName;
        _logFile.open(logFilePath);
        if (!_logFile.is_open())
        {
            std::cerr << "_logFile with path: " << logFilePath << " failed to open!\n";
        }
        char time_buffer[40];
        std::strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S %d-%m-%Y", &now);
        _logFile << "======================================\n";
        _logFile << time_buffer << '\n';
        if (!_logFileOpeningLine.empty())
        {
            _logFile << _logFileOpeningLine << '\n';
        }
        _logFile << "============= Logs Start =============\n";
        _logFile.close();
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

    void SetOpeningLine(std::string openingLine)
    {
        _logFileOpeningLine = openingLine;
    }

    void OpenLogFile()
    {
        if (_logFile.is_open())
        {
            return;
        }
        if (_logFileName.empty())
        {
            create_log_file();
        }
        fs::path logFilePath = _logsPath / _logFileName;
        _logFile.open(logFilePath);
    }

    void CloseLogFile()
    {
        if (!_logFile.is_open())
        {
            return;
        }
        _logFile.close();
    }
};

#endif