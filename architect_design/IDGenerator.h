#pragma once

#include <string>
#include <unordered_map>


class IDGenerator {
public:
    static IDGenerator& instance();

    
    [[nodiscard]] std::string next(const std::string& prefix);


    [[nodiscard]] std::string peek(const std::string& prefix) const;

    
    void reload();

    IDGenerator(const IDGenerator&)            = delete;
    IDGenerator& operator=(const IDGenerator&) = delete;

private:
    IDGenerator();
    ~IDGenerator() = default;

    void load();
    void persist() const;

    
    std::string format(const std::string& prefix, int counter) const;

    
    static int padWidth(const std::string& prefix) noexcept;

    
    static void ensureCounterDirectory();

    
    std::unordered_map<std::string, int> counters_;
};
