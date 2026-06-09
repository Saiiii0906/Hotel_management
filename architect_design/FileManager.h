#pragma once

#include <string>

// Centralised file-system authority for the Hotel Management System.
//
// Responsibilities (Phase 3):
//   - Own all path constants — one place to change a path, zero drift.
//   - Guarantee the data directory tree exists before any module writes to it.
//
// Responsibilities (Phase 4+, added as business modules are built):
//   - Expose typed load/save methods: loadRooms(), saveRooms(), etc.
//   - Delegate all I/O to FileHandler<T> — no raw file logic here.
//
// What this class is NOT:
//   - Not a business-logic class. It does not validate entities.
//   - Not a cache. It does not hold data in memory.
//   - Not a query engine. Filtering is the caller's job.
class FileManager {
public:
    static FileManager& instance();

    // ── Path constants ────────────────────────────────────────────────────────
    // All paths are relative to the working directory (project root).
    // Callers must not construct paths themselves — always use these constants.

    static constexpr const char* DATA_DIR      = "data/";
    static constexpr const char* LOG_DIR       = "data/logs/";
    static constexpr const char* COUNTER_DIR   = "data/counters/";
    static constexpr const char* CONFIG_DIR    = "config/";

    static constexpr const char* COUNTER_FILE  = "data/counters/id_counters.txt";
    static constexpr const char* CONFIG_FILE   = "config/hotel.cfg";
    static constexpr const char* LOG_FILE      = "data/logs/hotel.log";

    // Phase 4 paths — declared now so callers can reference them;
    // the corresponding load/save methods are added in Phase 4.
    static constexpr const char* ROOMS_FILE     = "data/rooms.txt";
    static constexpr const char* CUSTOMERS_FILE = "data/customers.txt";
    static constexpr const char* BOOKINGS_FILE  = "data/bookings.txt";
    static constexpr const char* BILLS_FILE     = "data/bills.txt";
    static constexpr const char* STAFF_FILE     = "data/staff.txt";

    // ── Bootstrap ─────────────────────────────────────────────────────────────

    // Create the full directory tree if any part is missing.
    // Safe to call multiple times — no-op when directories already exist.
    // Called once at program startup, before any module accesses the file system.
    // Returns false if any directory could not be created.
    bool ensureDirectories() const;

    // ── Singleton boilerplate ─────────────────────────────────────────────────
    FileManager(const FileManager&)            = delete;
    FileManager& operator=(const FileManager&) = delete;

private:
    FileManager()  = default;
    ~FileManager() = default;

    // Create a single directory and all missing parents.
    // Returns false and logs the system error on failure.
    bool createDirectory(const std::string& path) const;
};
