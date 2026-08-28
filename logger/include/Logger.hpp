#ifndef __LOGGER__
#define __LOGGER__

#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>
#include <iostream>
#include <filesystem>
#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace fs = std::filesystem;

enum LOG_TYPE_ENUM
{
    MESS,
    WARN,
    ERR,
    CRIT,
};

struct LogType
{
    LOG_TYPE_ENUM e;
    LogType(LOG_TYPE_ENUM _e) : e(_e) {}

    std::string toString()
    {
        switch (e)
        {
        case MESS:
            return "MESSAGE";
        case WARN:
            return "WARNING";
        case ERR:
            return "ERROR";
        case CRIT:
            return "CRITICAL";
        default:
            return "MESSAGE";
        }
    }
};

class Logger
{
private:
    static std::shared_ptr<Logger> _instance;
    static std::string _logFileOpeningLine;
    static bool _logDate;

    std::string _logFileName;
    std::string _logFileName_full;
    uint16_t _logFileCount = 0;
    std::fstream _logFile;

    fs::path _logsPath = fs::path("logs/");

    std::mutex _mutex;

    struct tm get_time()
    {
        time_t timestamp = time(&timestamp);
        struct tm now = *localtime(&timestamp);
        return now;
    }

    std::string format_file_name(struct tm now)
    {
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H_%M_%S", &now);
        return (std::string)buffer;
    }

    void create_log_file()
    {
        std::unique_lock<std::mutex> lock(_mutex);

        struct tm now = get_time();
        if (_logFileName.empty())
        {
            _logFileName = format_file_name(now);
        }
        std::ostringstream oss;
        oss << _logFileName << "." << _logFileCount << ".log";
        _logFileName_full = oss.str();
        if (!fs::exists(_logsPath) || !fs::is_directory(_logsPath))
        {
            fs::create_directories(_logsPath);
        }
        fs::path logFilePath = _logsPath / _logFileName_full;
        _logFile.open(logFilePath, std::ios::app);
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
        _logFileCount++;
        close_log_file();
    }

    void open_log_file()
    {
        if (_logFile.is_open())
        {
            return;
        }
        fs::path logFilePath = _logsPath / _logFileName_full;
        if (!fs::exists(logFilePath) || fs::file_size(logFilePath) >= (int)(INT32_MAX / 1000))
        {
            create_log_file();
        }
        logFilePath = _logsPath / _logFileName_full;
        _logFile.open(logFilePath, std::ios::app);
    }

    void close_log_file()
    {
        if (!_logFile.is_open())
        {
            return;
        }
        _logFile.close();
    }

public:
    Logger()
    {
        create_log_file();
    }

    ~Logger()
    {
        close_log_file();
    }

    static std::shared_ptr<Logger> getInstance()
    {
        if (_instance == nullptr)
        {
            _instance = std::make_shared<Logger>();
        }
        return _instance;
    }

    static void SetOpeningLine(std::string openingLine)
    {
        _logFileOpeningLine = openingLine;
    }

    static void SetLogDate(bool logDate)
    {
        _logDate = logDate;
    }

    void LogMessage(LogType type, std::string message)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        open_log_file();
        struct tm now = get_time();
        char buffer[80];
        std::string format = "%H:%M:%S";
        if (_logDate)
        {
            format = "%H:%M:%S %d-%m-%Y";
        }
        std::strftime(buffer, sizeof(buffer), format.c_str(), &now);
        std::ostringstream oss;
        oss << "[" << buffer << "] = [" << type.toString() << "] = " << message << '\n';
        std::cout << oss.str();
        _logFile << oss.str();
        close_log_file();
    }
};

#define SET_LOG_DATE(log)       \
    do                          \
    {                           \
        Logger::SetLogDate(log) \
    } while (false)

#define SET_OPENING_LINE(line)        \
    do                                \
    {                                 \
        Logger::SetOpeningLine(line); \
    } while (false)

#define LOG(message)                                                 \
    do                                                               \
    {                                                                \
        std::ostringstream oss;                                      \
        oss << message;                                              \
        Logger::getInstance()->LogMessage(LogType(MESS), oss.str()); \
    } while (false)

#define LOG_FILE(message)                                                   \
    do                                                                      \
    {                                                                       \
        std::ostringstream oss;                                             \
        oss << "[" << __FILE__ << "] = [" << __LINE__ << "] = " << message; \
        Logger::getInstance()->LogMessage(LogType(MESS), oss.str());        \
    } while (false)

#define LOG_WARNING(message)                                         \
    do                                                               \
    {                                                                \
        std::ostringstream oss;                                      \
        oss << message;                                              \
        Logger::getInstance()->LogMessage(LogType(WARN), oss.str()); \
    } while (false)

#define LOG_FILE_WARNING(message)                                           \
    do                                                                      \
    {                                                                       \
        std::ostringstream oss;                                             \
        oss << "[" << __FILE__ << "] = [" << __LINE__ << "] = " << message; \
        Logger::getInstance()->LogMessage(LogType(WARN), oss.str());        \
    } while (false)

#define LOG_ERROR(message)                                          \
    do                                                              \
    {                                                               \
        std::ostringstream oss;                                     \
        oss << message;                                             \
        Logger::getInstance()->LogMessage(LogType(ERR), oss.str()); \
    } while (false)

#define LOG_FILE_ERROR(message)                                             \
    do                                                                      \
    {                                                                       \
        std::ostringstream oss;                                             \
        oss << "[" << __FILE__ << "] = [" << __LINE__ << "] = " << message; \
        Logger::getInstance()->LogMessage(LogType(ERR), oss.str());         \
    } while (false)

#define LOG_CRITICAL(message)                                        \
    do                                                               \
    {                                                                \
        std::ostringstream oss;                                      \
        oss << message;                                              \
        Logger::getInstance()->LogMessage(LogType(CRIT), oss.str()); \
    } while (false)

#define LOG_FILE_CRITICAL(message)                                          \
    do                                                                      \
    {                                                                       \
        std::ostringstream oss;                                             \
        oss << "[" << __FILE__ << "] = [" << __LINE__ << "] = " << message; \
        Logger::getInstance()->LogMessage(LogType(CRIT), oss.str());        \
    } while (false)

#endif