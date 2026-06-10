#pragma once

// FileHandler<T> — generic, header-only persistence template.
//
// Works with any T that:
//   1. Inherits Serializable (provides serialize() / deserialize())
//   2. Is DefaultConstructible (so loadAll() can construct before populating)
//
// All file I/O in the system flows through this class.
// FileManager owns the path constants and calls these methods.
// Business managers (RoomManager, CustomerManager…) call FileManager.
// Nobody calls FileHandler directly except FileManager.
//
// Atomic save contract:
//   saveAll() writes to filePath_ + ".tmp", then renames.
//   A crash mid-write leaves the original file intact.
//
// Error policy:
//   Methods return bool — false means "something went wrong."
//   Logger is called with details before returning false.
//   No exceptions are thrown from file I/O.
//
// Usage:
//   FileHandler<Room> h("data/rooms.txt");
//   auto rooms = h.loadAll();
//   h.append(newRoom);
//   h.deleteById("R003", [](const Room& r){ return r.roomId(); });

#include "core/Serializable.h"
#include "core/Logger.h"

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <algorithm>
#include <vector>

namespace fs = std::filesystem;

template<typename T>
class FileHandler {
    static_assert(std::is_base_of<Serializable, T>::value,
                  "FileHandler<T>: T must inherit Serializable");
    static_assert(std::is_default_constructible<T>::value,
                  "FileHandler<T>: T must be DefaultConstructible");

public:
    // ── Construction ─────────────────────────────────────────────────────────

    explicit FileHandler(const std::string& filePath)
        : filePath_(filePath)
    {
        ensureDirectoryExists();
    }

    // ── Core operations ───────────────────────────────────────────────────────

    // Read every non-empty line from the file.
    // Construct a default T, call deserialize(line).
    // Lines that fail deserialize are logged and skipped — partial corruption
    // does not destroy the rest of the file.
    [[nodiscard]] std::vector<T> loadAll() const {
        std::vector<T> result;

        std::ifstream file(filePath_);
        if (!file.is_open()) {
            // An absent file is not an error — it just means no records exist yet.
            if (fs::exists(filePath_)) {
                Logger::instance().error(
                    "FileHandler: could not open for reading: " + filePath_);
            }
            return result;
        }

        std::string line;
        int lineNum = 0;

        while (std::getline(file, line)) {
            ++lineNum;
            if (line.empty()) continue;

            T obj;
            if (!obj.deserialize(line)) {
                Logger::instance().warn(
                    "FileHandler: deserialize failed on line "
                    + std::to_string(lineNum) + " of " + filePath_
                    + " — record skipped");
                continue;
            }
            result.push_back(std::move(obj));
        }

        return result;
    }

    // Overwrite the file with every object in items.
    // Atomic: writes to filePath_ + ".tmp", renames on success.
    // Returns false if the write or rename failed.
    bool saveAll(const std::vector<T>& items) const {
        const std::string tmp = tmpPath();

        std::ofstream file(tmp, std::ios::trunc);
        if (!file.is_open()) {
            Logger::instance().error(
                "FileHandler: could not open temp file for writing: " + tmp);
            return false;
        }

        for (const T& item : items) {
            file << item.serialize() << '\n';
        }
        file.close();

        std::error_code ec;
        fs::rename(tmp, filePath_, ec);
        if (ec) {
            Logger::instance().error(
                "FileHandler: atomic rename failed for " + filePath_
                + ": " + ec.message());
            fs::remove(tmp, ec);   // clean up orphaned temp file
            return false;
        }

        return true;
    }

    // Serialize item and append one line to the file.
    // Does not load existing records — O(1) for a single append.
    bool append(const T& item) const {
        std::ofstream file(filePath_, std::ios::app);
        if (!file.is_open()) {
            Logger::instance().error(
                "FileHandler: could not open for append: " + filePath_);
            return false;
        }

        file << item.serialize() << '\n';
        return true;
    }

    // Load all records, remove the one where getId(record) == id, save all.
    // IdFn: callable with signature std::string(const T&).
    //
    // Example:
    //   handler.deleteById("R003", [](const Room& r){ return r.roomId(); });
    //
    // Returns false if the id was not found or the save failed.
    template<typename IdFn>
    bool deleteById(const std::string& id, IdFn getId) const {
        auto records = loadAll();
        const std::size_t before = records.size();

        records.erase(
            std::remove_if(records.begin(), records.end(),
                [&](const T& item){ return getId(item) == id; }),
            records.end()
        );

        if (records.size() == before) {
            Logger::instance().warn(
                "FileHandler::deleteById: id '" + id
                + "' not found in " + filePath_);
            return false;
        }

        return saveAll(records);
    }

    // Load all records, replace the one where getId(record) == getId(updated).
    // IdFn: callable with signature std::string(const T&).
    //
    // Example:
    //   handler.updateById(updatedRoom, [](const Room& r){ return r.roomId(); });
    //
    // Returns false if the id was not found or the save failed.
    template<typename IdFn>
    bool updateById(const T& updated, IdFn getId) const {
        auto records = loadAll();
        bool found = false;

        for (T& item : records) {
            if (getId(item) == getId(updated)) {
                item  = updated;
                found = true;
                break;
            }
        }

        if (!found) {
            Logger::instance().warn(
                "FileHandler::updateById: id '" + getId(updated)
                + "' not found in " + filePath_);
            return false;
        }

        return saveAll(records);
    }

    // ── Utility ───────────────────────────────────────────────────────────────

    // True if the file exists and can be read.
    [[nodiscard]] bool fileExists() const {
        return fs::exists(filePath_);
    }

    // Number of non-empty lines in the file — one per record.
    // O(n) — reads the file. Use sparingly.
    [[nodiscard]] std::size_t count() const {
        return loadAll().size();
    }

private:
    std::string filePath_;

    // Create the parent directory of filePath_ if it does not exist.
    void ensureDirectoryExists() const {
        fs::path p(filePath_);
        if (p.has_parent_path()) {
            std::error_code ec;
            fs::create_directories(p.parent_path(), ec);
            if (ec) {
                Logger::instance().error(
                    "FileHandler: could not create directory '"
                    + p.parent_path().string() + "': " + ec.message());
            }
        }
    }

    // The staging path used by saveAll() for atomic writes.
    [[nodiscard]] std::string tmpPath() const {
        return filePath_ + ".tmp";
    }
};
