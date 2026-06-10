#pragma once

#include <string>


class Config {
public:
    static Config& instance();

    
    const std::string& hotelName() const noexcept;

    double taxRate() const noexcept;

    const std::string& currencySymbol() const noexcept;

    
    double lateCheckoutFee() const noexcept;

    
    const std::string& checkoutTime() const noexcept;

    
    void reload();

    Config(const Config&)            = delete;
    Config& operator=(const Config&) = delete;

private:
    Config();
    ~Config() = default;

    void        load();
    void        applyDefaults();
    static bool parseLine(const std::string& line,
                          std::string& outKey, std::string& outValue);
    static std::string trim(const std::string& s);

    std::string hotelName_       { "Hotel"  };
    double      taxRate_         { 0.10     };
    std::string currencySymbol_  { "$"      };
    double      lateCheckoutFee_ { 0.0      };
    std::string checkoutTime_    { "11:00"  };
};
