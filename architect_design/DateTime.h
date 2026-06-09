#pragma once

#include <string>
#include <ctime>

// Lightweight date/time value type.
//
// Stores wall-clock components (y/m/d h:m:s) rather than epoch seconds so
// callers can reason about dates without conversions.  All times are local.
//
// Designed as a value type — cheap to copy, trivially comparable.
// Used as a timestamp throughout the system: Logger entries, Booking dates,
// Invoice generation times, IDGenerator persistence.
class DateTime {
public:
    // ── Construction ─────────────────────────────────────────────────────────

    // Construct with explicit components.  No validation — callers are
    // responsible for passing sane values (enforced in fromString).
    DateTime(int year, int month, int day,
             int hour = 0, int minute = 0, int second = 0) noexcept;

    // Capture current local time.
    static DateTime now() noexcept;

    // Parse an ISO 8601 string ("YYYY-MM-DDTHH:MM:SS").
    // Returns a zeroed DateTime on malformed input rather than throwing —
    // consistent with the project-wide no-exception-from-parsing policy.
    static DateTime fromString(const std::string& iso) noexcept;

    // ── Formatting ───────────────────────────────────────────────────────────

    std::string toISOString()   const;   // "2024-09-15T14:30:00"
    std::string toDisplayDate() const;   // "15 Sep 2024"
    std::string toDisplayTime() const;   // "14:30"
    std::string toLogStamp()    const;   // "[2024-09-15 14:30:00]"
    std::string toFileSafe()    const;   // "20240915_143000"

    // ── Accessors ────────────────────────────────────────────────────────────

    int year()   const noexcept { return year_;   }
    int month()  const noexcept { return month_;  }
    int day()    const noexcept { return day_;     }
    int hour()   const noexcept { return hour_;    }
    int minute() const noexcept { return minute_;  }
    int second() const noexcept { return second_;  }

    // ── Arithmetic ───────────────────────────────────────────────────────────

    // Number of whole calendar days between this and other.
    // Always non-negative — order of operands doesn't matter.
    int daysBetween(const DateTime& other)  const noexcept;

    // Absolute difference in whole hours.
    int hoursBetween(const DateTime& other) const noexcept;

    // ── Comparison ───────────────────────────────────────────────────────────

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

    // Returns epoch seconds via mktime.  Used for arithmetic.
    // mktime handles DST transitions correctly; naive subtraction doesn't.
    std::time_t toEpoch() const noexcept;
};
