#pragma once

#include <string>

// Contract for all persistable entities.
//
// Every class that FileHandler<T> manages must inherit this interface.
// The pipe-delimited format is enforced by convention, not by this class —
// see FileHandler for the parsing contract.
//
// Implementation rules:
//   - serialize()   must be const and produce a single line (no '\n')
//   - deserialize() must return false on malformed input, not throw
//   - Neither method should perform file I/O
class Serializable {
public:
    virtual ~Serializable() = default;

    // Produce a pipe-delimited string representing this object's complete state.
    // The returned string must not contain newline characters.
    [[nodiscard]] virtual std::string serialize() const = 0;

    // Populate this object's state from a pipe-delimited string.
    // Returns false if the input is malformed or missing required fields.
    virtual bool deserialize(const std::string& line) = 0;
};
