#pragma once

#include <string>


class FileManager {
public:
    static FileManager& instance();

    
    static constexpr const char* DATA_DIR      = "data/";
    static constexpr const char* LOG_DIR       = "data/logs/";
    static constexpr const char* COUNTER_DIR   = "data/counters/";
    static constexpr const char* CONFIG_DIR    = "config/";

    static constexpr const char* COUNTER_FILE  = "data/counters/id_counters.txt";
    static constexpr const char* CONFIG_FILE   = "config/hotel.cfg";
    static constexpr const char* LOG_FILE      = "data/logs/hotel.log";

   
    static constexpr const char* ROOMS_FILE     = "data/rooms.txt";
    static constexpr const char* CUSTOMERS_FILE = "data/customers.txt";
    static constexpr const char* BOOKINGS_FILE  = "data/bookings.txt";
    static constexpr const char* BILLS_FILE     = "data/bills.txt";
    static constexpr const char* STAFF_FILE     = "data/staff.txt";

    
    bool ensureDirectories() const;

   
    FileManager(const FileManager&)            = delete;
    FileManager& operator=(const FileManager&) = delete;

private:
    FileManager()  = default;
    ~FileManager() = default;

    
    bool createDirectory(const std::string& path) const;
};
