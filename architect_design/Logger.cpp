#include "core/Logger.h"
#include "core/DateTime.h"
#include "core/FileManager.h"

#include <filesystem>
#include <iostream>
#include <system_error>

namespace fs = std::filesystem;



Logger& Logger::instance() {
    
    static Logger inst;
    return inst;
}


Logger::Logger() {
    ensureLogDirectory();

    logFile_.open(FileManager::LOG_FILE, std::ios::app);

    if (!logFile_.is_open()) {
        
        std::cerr << "[Logger] Could not open log file: "
                  << FileManager::LOG_FILE << '\n';
    }
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.flush();
        logFile_.close();
    }
}

void Logger::log(Level level, const std::string& message) {
    if (level < minLevel_) return;

    std::string entry = buildEntry(level, message);

    if (logFile_.is_open()) {
        logFile_ << entry;
        
        logFile_.flush();
    } else {
        
        std::cerr << entry;
    }
}

void Logger::info (const std::string& message) { log(Level::INFO,    message); }
void Logger::warn (const std::string& message) { log(Level::WARNING, message); }
void Logger::error(const std::string& message) { log(Level::ERROR,   message); }

void Logger::setMinLevel(Level min) {
    minLevel_ = min;
}


const char* Logger::levelTag(Level l) noexcept {
    switch (l) {
        case Level::INFO:    return "[INFO]   ";
        case Level::WARNING: return "[WARNING]";
        case Level::ERROR:   return "[ERROR]  ";
    }
    return "[?]";   
}

std::string Logger::buildEntry(Level level, const std::string& message) const {
    // Format: "[2026-06-10 14:30:22] [INFO]   Customer Registered\n"
    // DateTime::toLogStamp() produces "[YYYY-MM-DD HH:MM:SS]" — consistent
    // with every other timestamp in the system.
    std::string entry;
    entry.reserve(80);
    entry += DateTime::now().toLogStamp();
    entry += ' ';
    entry += levelTag(level);
    entry += ' ';
    entry += message;
    entry += '\n';
    return entry;
}

void Logger::ensureLogDirectory() {
    std::error_code ec;
    fs::create_directories(FileManager::LOG_DIR, ec);

    if (ec) {
        std::cerr << "[Logger] Could not create log directory '"
                  << FileManager::LOG_DIR << "': " << ec.message() << '\n';
    }
}
