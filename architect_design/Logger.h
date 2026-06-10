#pragma once

#include <fstream>
#include <string>


class Logger {
public:
    enum class Level { INFO = 0, WARNING = 1, ERROR = 2 };

    static Logger& instance();

    
    void log(Level level, const std::string& message);

    
    void info (const std::string& message);
    void warn (const std::string& message);
    void error(const std::string& message);

    
    void setMinLevel(Level min);

   
    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger();    
    ~Logger();   

    std::ofstream logFile_;
    Level         minLevel_{ Level::INFO };

    
    static const char* levelTag(Level l) noexcept;

    
    std::string buildEntry(Level level, const std::string& message) const;

    
    static void ensureLogDirectory();
};
