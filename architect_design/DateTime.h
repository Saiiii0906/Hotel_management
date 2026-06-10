#pragma once

#include <string>
#include <ctime>

class DateTime {
public:
    
    DateTime(int year, int month, int day,
             int hour = 0, int minute = 0, int second = 0) noexcept;

    
    static DateTime now() noexcept;

    
    static DateTime fromString(const std::string& iso) noexcept;

   

    std::string toISOString()   const;   
    std::string toDisplayDate() const;   
    std::string toDisplayTime() const;   
    std::string toLogStamp()    const;   
    std::string toFileSafe()    const;   

    

    int year()   const noexcept { return year_;   }
    int month()  const noexcept { return month_;  }
    int day()    const noexcept { return day_;     }
    int hour()   const noexcept { return hour_;    }
    int minute() const noexcept { return minute_;  }
    int second() const noexcept { return second_;  }


    int daysBetween(const DateTime& other)  const noexcept;


    int hoursBetween(const DateTime& other) const noexcept;

 
    bool operator==(const DateTime& rhs) const noexcept;
    bool operator!=(const DateTime& rhs) const noexcept;
    bool operator< (const DateTime& rhs) const noexcept;
    bool operator<=(const DateTime& rhs) const noexcept;
    bool operator> (const DateTime& rhs) const noexcept;
    bool operator>=(const DateTime& rhs) const noexcept;

private:
    int year_   = 0;
    int month_  = 0;
    int day_    = 0;
    int hour_   = 0;
    int minute_ = 0;
    int second_ = 0;

    std::tm toTm()                        const noexcept;
    static DateTime fromTm(const std::tm& t) noexcept;

    std::time_t toEpoch() const noexcept;
};
