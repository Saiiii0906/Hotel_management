#include "core/DateTime.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <array>

// ── Construction ──────────────────────────────────────────────────────────────

DateTime::DateTime(int year, int month, int day,
                   int hour, int minute, int second) noexcept
    : year_(year), month_(month), day_(day),
      hour_(hour), minute_(minute), second_(second)
{}

DateTime DateTime::now() noexcept {
    auto tp  = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &t);
#else
    localtime_r(&t, &local);
#endif
    return fromTm(local);
}

DateTime DateTime::fromString(const std::string& iso) noexcept {
    // Expected: "YYYY-MM-DDTHH:MM:SS"
    // Any other format returns a zeroed DateTime — consistent with the
    // project-wide policy of no exceptions from parsing.
    if (iso.size() < 19) return DateTime(0, 0, 0);

    std::istringstream ss(iso);
    std::tm t{};
    ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) return DateTime(0, 0, 0);

    return fromTm(t);
}

// ── Formatting ────────────────────────────────────────────────────────────────

std::string DateTime::toISOString() const {
    char buf[20];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d",
                  year_, month_, day_, hour_, minute_, second_);
    return buf;
}

std::string DateTime::toDisplayDate() const {
    static constexpr std::array<const char*, 12> months = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    const char* mon = (month_ >= 1 && month_ <= 12)
                    ? months[static_cast<std::size_t>(month_ - 1)]
                    : "???";
    char buf[12];
    std::snprintf(buf, sizeof(buf), "%02d %s %04d", day_, mon, year_);
    return buf;
}

std::string DateTime::toDisplayTime() const {
    char buf[6];
    std::snprintf(buf, sizeof(buf), "%02d:%02d", hour_, minute_);
    return buf;
}

std::string DateTime::toLogStamp() const {
    char buf[22];
    std::snprintf(buf, sizeof(buf), "[%04d-%02d-%02d %02d:%02d:%02d]",
                  year_, month_, day_, hour_, minute_, second_);
    return buf;
}

std::string DateTime::toFileSafe() const {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%04d%02d%02d_%02d%02d%02d",
                  year_, month_, day_, hour_, minute_, second_);
    return buf;
}

// ── Arithmetic ────────────────────────────────────────────────────────────────

int DateTime::daysBetween(const DateTime& other) const noexcept {
    // mktime is DST-aware; naive (epoch_a - epoch_b) / 86400 can be off by 1
    // on days that include a DST transition.
    std::time_t a = toEpoch();
    std::time_t b = other.toEpoch();
    if (a == -1 || b == -1) return 0;

    double diff = std::difftime(a, b);
    return static_cast<int>(std::abs(diff) / 86400.0);
}

int DateTime::hoursBetween(const DateTime& other) const noexcept {
    std::time_t a = toEpoch();
    std::time_t b = other.toEpoch();
    if (a == -1 || b == -1) return 0;

    double diff = std::difftime(a, b);
    return static_cast<int>(std::abs(diff) / 3600.0);
}

// ── Comparison ────────────────────────────────────────────────────────────────

bool DateTime::operator==(const DateTime& rhs) const noexcept {
    return year_   == rhs.year_   && month_  == rhs.month_  &&
           day_    == rhs.day_    && hour_   == rhs.hour_   &&
           minute_ == rhs.minute_ && second_ == rhs.second_;
}

bool DateTime::operator!=(const DateTime& rhs) const noexcept { return !(*this == rhs); }
bool DateTime::operator< (const DateTime& rhs) const noexcept { return toEpoch() <  rhs.toEpoch(); }
bool DateTime::operator<=(const DateTime& rhs) const noexcept { return toEpoch() <= rhs.toEpoch(); }
bool DateTime::operator> (const DateTime& rhs) const noexcept { return toEpoch() >  rhs.toEpoch(); }
bool DateTime::operator>=(const DateTime& rhs) const noexcept { return toEpoch() >= rhs.toEpoch(); }

// ── Private helpers ───────────────────────────────────────────────────────────

std::tm DateTime::toTm() const noexcept {
    std::tm t{};
    t.tm_year = year_   - 1900;
    t.tm_mon  = month_  - 1;
    t.tm_mday = day_;
    t.tm_hour = hour_;
    t.tm_min  = minute_;
    t.tm_sec  = second_;
    t.tm_isdst = -1;   // let mktime determine DST
    return t;
}

DateTime DateTime::fromTm(const std::tm& t) noexcept {
    return DateTime(
        t.tm_year + 1900,
        t.tm_mon  + 1,
        t.tm_mday,
        t.tm_hour,
        t.tm_min,
        t.tm_sec
    );
}

std::time_t DateTime::toEpoch() const noexcept {
    std::tm t = toTm();
    return std::mktime(&t);   // normalises fields; handles DST
}
