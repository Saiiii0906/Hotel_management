#include "FileManager.h"

#include <filesystem>
#include <iostream>
#include <system_error>

namespace fs = std::filesystem;



FileManager& FileManager::instance() {
    static FileManager inst;
    return inst;
}



bool FileManager::ensureDirectories() const {
    bool ok = true;

    ok &= createDirectory(DATA_DIR);
    ok &= createDirectory(LOG_DIR);
    ok &= createDirectory(COUNTER_DIR);
    ok &= createDirectory(CONFIG_DIR);

    return ok;
}



bool FileManager::createDirectory(const std::string& path) const {
    std::error_code ec;
    fs::create_directories(path, ec);

    if (ec) {
        
        std::cerr << "[FileManager] Failed to create directory '"
                  << path << "': " << ec.message() << '\n';
        return false;
    }

    return true;
}
