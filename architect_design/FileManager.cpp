#include "core/FileManager.h"

#include <filesystem>
#include <iostream>
#include <system_error>

namespace fs = std::filesystem;

// ── Singleton ─────────────────────────────────────────────────────────────────

FileManager& FileManager::instance() {
    static FileManager inst;
    return inst;
}

// ── Bootstrap ─────────────────────────────────────────────────────────────────

bool FileManager::ensureDirectories() const {
    bool ok = true;

    ok &= createDirectory(DATA_DIR);
    ok &= createDirectory(LOG_DIR);
    ok &= createDirectory(COUNTER_DIR);
    ok &= createDirectory(CONFIG_DIR);

    return ok;
}

// ── Private ───────────────────────────────────────────────────────────────────

bool FileManager::createDirectory(const std::string& path) const {
    std::error_code ec;
    fs::create_directories(path, ec);

    if (ec) {
        // Write directly to stderr — Logger is not available yet when
        // FileManager is called at startup (Logger depends on LOG_DIR existing).
        std::cerr << "[FileManager] Failed to create directory '"
                  << path << "': " << ec.message() << '\n';
        return false;
    }

    return true;
}
