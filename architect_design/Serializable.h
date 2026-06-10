#pragma once

#include <string>
class Serializable {
public:
    virtual ~Serializable() = default;
    [[nodiscard]] virtual std::string serialize() const = 0;
    virtual bool deserialize(const std::string& line) = 0;
};
