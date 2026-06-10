#include "core/Config.h"
#include "core/FileManager.h"
#include "core/Logger.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>



Config& Config::instance() {
    static Config inst;
    return inst;
}



Config::Config() {
    
    load();
}



const std::string& Config::hotelName()      const noexcept { return hotelName_;       }
double             Config::taxRate()         const noexcept { return taxRate_;         }
const std::string& Config::currencySymbol()  const noexcept { return currencySymbol_;  }
double             Config::lateCheckoutFee() const noexcept { return lateCheckoutFee_; }
const std::string& Config::checkoutTime()    const noexcept { return checkoutTime_;    }


void Config::reload() {
    applyDefaults();
    load();
}


void Config::load() {
    std::ifstream file(FileManager::CONFIG_FILE);

    if (!file.is_open()) {
        
        Logger::instance().warn(
            std::string("Config file not found: ") + FileManager::CONFIG_FILE
            + " — using defaults"
        );
        return;
    }

    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        ++lineNum;

        std::string key, value;
        if (!parseLine(line, key, value)) continue;   

        if (key == "hotel_name") {
            hotelName_ = value;

        } else if (key == "tax_rate") {
            try {
                double v = std::stod(value);
                taxRate_ = v;
            } catch (...) {
                Logger::instance().warn(
                    std::string("Config: invalid tax_rate '") + value
                    + "' on line " + std::to_string(lineNum)
                    + " — using default " + std::to_string(taxRate_)
                );
            }

        } else if (key == "currency_symbol") {
            currencySymbol_ = value;

        } else if (key == "late_checkout_fee") {
            try {
                double v = std::stod(value);
                lateCheckoutFee_ = v;
            } catch (...) {
                Logger::instance().warn(
                    std::string("Config: invalid late_checkout_fee '") + value
                    + "' on line " + std::to_string(lineNum)
                    + " — using default " + std::to_string(lateCheckoutFee_)
                );
            }

        } else if (key == "checkout_time") {
            checkoutTime_ = value;

        } else {
            
            Logger::instance().info(
                std::string("Config: unknown key '") + key
                + "' on line " + std::to_string(lineNum) + " — ignored"
            );
        }
    }

    Logger::instance().info(
        std::string("Config loaded from ") + FileManager::CONFIG_FILE
    );
}

void Config::applyDefaults() {
    hotelName_       = "Hotel";
    taxRate_         = 0.10;
    currencySymbol_  = "$";
    lateCheckoutFee_ = 0.0;
    checkoutTime_    = "11:00";
}


bool Config::parseLine(const std::string& line,
                       std::string& outKey, std::string& outValue) {
    std::string trimmed = trim(line);

    if (trimmed.empty() || trimmed[0] == '#') return false;

    auto sep = trimmed.find('=');
    if (sep == std::string::npos) return false;   

    outKey   = trim(trimmed.substr(0, sep));
    outValue = trim(trimmed.substr(sep + 1));

    return !outKey.empty();
}

std::string Config::trim(const std::string& s) {
    const char* ws = " \t\r\n";
    auto start = s.find_first_not_of(ws);
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}
