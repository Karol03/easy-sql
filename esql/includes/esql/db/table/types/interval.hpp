/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <functional>
#include <inttypes.h>
#include <sstream>
#include <string>
#include <vector>


namespace esql::db::table::types
{

struct Days { Days(uint64_t value) : value{value} {} uint64_t value{}; };
struct Seconds { Seconds(uint64_t value) : value{value} {} uint64_t value{}; };
struct Microseconds { Microseconds(uint64_t value) : value{value} {} uint64_t value{}; };

inline Days operator""_days(long double value) { return Days{static_cast<uint64_t>(value)}; }
inline Days operator""_days(const char* value) { return Days{static_cast<uint64_t>(atoll(value))}; }
inline Seconds operator""_seconds(long double value) { return Seconds{static_cast<uint64_t>(value)}; }
inline Seconds operator""_seconds(const char* value) { return Seconds{static_cast<uint64_t>(atoll(value))}; }
inline Microseconds operator""_microseconds(long double value) { return Microseconds{static_cast<uint64_t>(value)}; }
inline Microseconds operator""_microseconds(const char* value) { return Microseconds{static_cast<uint64_t>(atoll(value))}; }


struct Interval
{
public:
    Interval()
        : m_days{}
        , m_seconds{}
        , m_microseconds{}
    {}

    Interval(uint64_t seconds)
        : m_days{seconds / 86'400ull}
        , m_seconds{seconds % 86'400ull}
        , m_microseconds{}
    {}

    Interval(Days days)
        : m_days{days.value}
        , m_seconds{}
        , m_microseconds{}
    {}

    Interval(Seconds seconds)
        : m_days{seconds.value / 86'400ull}
        , m_seconds{seconds.value % 86'400ull}
        , m_microseconds{}
    {}

    Interval(Microseconds microseconds)
        : m_days{}
        , m_seconds{}
        , m_microseconds{microseconds.value % 1'000'000ull}
    {
        microseconds.value /= 1'000'000ull;
        m_days = microseconds.value / 86'400ull;
        m_seconds = microseconds.value % 86'400ull;
    }

    Interval(Days days, Seconds seconds)
        : m_days{days.value + seconds.value / 86'400ull}
        , m_seconds{seconds.value % 86'400ull}
        , m_microseconds{}
    {}

    Interval(Seconds seconds, Microseconds microseconds)
        : m_days{seconds.value / 86'400ull}
        , m_seconds{seconds.value % 86'400ull}
        , m_microseconds{microseconds.value % 1'000'000ull}
    {
        microseconds.value /= 1'000'000ull;
        m_seconds += (microseconds.value % 86'400ull);
        m_days += ((microseconds.value / 86'400ull) + (m_seconds / 86'400ull));
        m_seconds %= 86'400ull;
    }

    Interval(Days days, Seconds seconds, Microseconds microseconds)
        : m_days{days.value + seconds.value / 86'400ull}
        , m_seconds{seconds.value % 86'400ull}
        , m_microseconds{microseconds.value % 1'000'000ull}
    {
        microseconds.value /= 1'000'000ull;
        m_seconds += (microseconds.value % 86'400ull);
        m_days += ((microseconds.value / 86'400ull) + (m_seconds / 86'400ull));
        m_seconds %= 86'400ull;
    }

    Interval(std::string valueStr)
        : m_days{}
        , m_seconds{}
        , m_microseconds{}
    {
        if (valueStr.empty())
            return;

        const auto yearWords = std::vector<std::string>{"y", "year", "years"};
        const auto monthWords = std::vector<std::string>{"m", "month", "months"};
        const auto dayWords = std::vector<std::string>{"d", "day", "days"};
        const auto hourWords = std::vector<std::string>{"h", "hh", "hour", "hours"};
        const auto minuteWords = std::vector<std::string>{"mm", "min", "minute", "minutes"};
        const auto secondWords = std::vector<std::string>{"s", "sec", "second", "seconds"};
        const auto microsecondWords = std::vector<std::string>{"us", "micro", "microsecond", "microseconds"};

        toLower(valueStr);
        const auto values = split(valueStr);
        for (auto i = 1u; i < values.size(); i += 2)
        {
            const auto value = std::atoi(values[i - 1].c_str());
            const auto name = values[i];

            if (any_of(yearWords, name)) years(value);
            else if (any_of(monthWords, name)) months(value);
            else if (any_of(dayWords, name)) days(value);
            else if (any_of(hourWords, name)) hours(value);
            else if (any_of(minuteWords, name)) minutes(value);
            else if (any_of(secondWords, name)) seconds(value);
            else if (any_of(microsecondWords, name)) microseconds(value);
        }
    }

    Interval(const Interval&) = default;
    Interval& operator=(const Interval&) = default;
    Interval(Interval&&) = default;
    Interval& operator=(Interval&&) = default;

    std::string to_string() const
    {
        if (!(*this))
            return "0 seconds";

        auto result = std::stringstream{};
        if (years() == 1u)
            result << "1 year ";
        else if (years() > 1)
            result << years() << " years ";

        if (months() == 1)
            result << "1 month ";
        else if (months() > 1)
            result << months() << " months ";

        if (days() == 1)
            result << "1 day ";
        else if (days() > 1)
            result << days() << " days ";

        if (hours() == 1)
            result << "1 hour ";
        else if (hours() > 1)
            result << hours() << " hours ";

        if (minutes() == 1)
            result << "1 minute ";
        else if (minutes() > 1)
            result << minutes() << " minutes ";

        if (seconds() == 1)
            result << "1 second ";
        else if (seconds() > 1)
            result << seconds() << " seconds ";

        if (microseconds() == 1)
            result << "1 microsecond ";
        else if (microseconds() > 1)
            result << microseconds() << " microseconds ";

        auto resultStr = result.str();
        resultStr.pop_back();
        return resultStr;
    }

    inline uint32_t days() const { return (m_days % 365ull) % 30ull; }
    inline uint32_t hours() const { return m_seconds / 3'600ull; }
    inline uint32_t minutes() const { return (m_seconds % 3600ull) / 60ull; }
    inline uint32_t months() const { return (m_days % 365ull) / 30ull; }
    inline uint32_t seconds() const { return (m_seconds % 3600ull) % 60ull; }
    inline uint32_t years() const { return m_days / 365ull; }
    inline uint64_t microseconds() const { return m_microseconds; }

    inline void days(uint64_t value) { m_days = value; }
    inline void hours(uint32_t value) { seconds(uint64_t{value} * 3'600ull); }
    inline void microseconds(uint64_t value)
    {
        if (value >= 1'000'000ull)
            seconds(value / 1'000'000ull);
        m_microseconds = value % 1'000'000ull;
    }
    inline void minutes(uint32_t value) { seconds(uint64_t{value} * 60ull); }
    inline void months(uint32_t value) { days(uint64_t{value} * 30); }
    inline void seconds(uint64_t value)
    {
        if (value >= 86'400ull)
            days(value / 86'400ull);
        m_seconds = value % 86'400ull;
    }
    inline void years(uint32_t value) { days(uint64_t{value} * 365); }

    inline void reset() { m_days = 0; m_seconds = 0; m_microseconds = 0; }
    inline operator bool() const { return m_days || m_seconds || m_microseconds; }

    inline Interval& operator+=(const Interval& rhs)
    {
        m_microseconds += rhs.m_microseconds;
        m_seconds += (m_microseconds / 1'000'000ull) + rhs.m_seconds;
        m_days += (m_seconds / 86'400ull) + rhs.m_days;

        m_microseconds %= 1'000'000ull;
        m_seconds %= 86'400ull;
        return *this;
    }

    inline Interval& operator-=(const Interval& rhs)
    {
        if (rhs > *this)
        {
            m_days = 0;
            m_seconds = 0;
            m_microseconds = 0;
            return *this;
        }

        m_days -= rhs.m_days;
        m_seconds = m_seconds + 86'400ull - rhs.m_seconds;
        m_microseconds = m_microseconds + 1'000'000ull - rhs.m_microseconds;

        if (m_microseconds >= 1'000'000ull)
            m_microseconds -= 1'000'000ull;
        else
            m_seconds -= 1;

        if (m_seconds >= 86'400ull)
            m_seconds -= 86'400ull;
        else
            m_days -= 1;
        return *this;
    }

    inline Interval& operator+=(const Days& rhs) { return (*this += Interval{rhs}); }
    inline Interval& operator-=(const Days& rhs) { return (*this -= Interval{rhs}); }
    inline Interval& operator+=(const Seconds& rhs) { return (*this += Interval{rhs}); }
    inline Interval& operator-=(const Seconds& rhs) { return (*this -= Interval{rhs}); }
    inline Interval& operator+=(const Microseconds& rhs) { return (*this += Interval{rhs}); }
    inline Interval& operator-=(const Microseconds& rhs) { return (*this -= Interval{rhs}); }
    inline Interval& operator+=(const uint64_t& rhs) { return (*this += Interval{rhs}); }
    inline Interval& operator-=(const uint64_t& rhs) { return (*this -= Interval{rhs}); }
    inline Interval& operator*=(double rhs)
    {
        if (rhs < 0.0L)
            rhs *= (-1.0L);

        auto micros = static_cast<double>(m_microseconds) * rhs;
        auto seconds = static_cast<double>(m_seconds) * rhs;
        auto days = static_cast<double>(m_days) * rhs;

        seconds += (days - static_cast<double>(static_cast<uint64_t>(days))) * 86'400.0L;
        micros += (seconds - static_cast<double>(static_cast<uint64_t>(seconds))) * 1'000'000.0L;

        m_microseconds = static_cast<uint64_t>(micros);
        m_seconds = static_cast<uint64_t>(seconds);
        m_days = static_cast<uint64_t>(days);

        m_seconds += m_microseconds / 1'000'000ull;
        m_microseconds %= 1'000'000ull;

        m_days += m_seconds / 86'400ull;
        m_seconds %= 86'400ull;
        return *this;
    }
    inline Interval& operator/=(double rhs) { return operator*=(1.0L/rhs); }

    friend Interval operator+(const Interval& lhs, const Interval& rhs);
    friend Interval operator-(const Interval& lhs, const Interval& rhs);
    friend Interval operator+(const Interval& lhs, const uint64_t& rhs);
    friend Interval operator-(const Interval& lhs, const uint64_t& rhs);
    friend Interval operator+(const uint64_t& lhs, const Interval& rhs);
    friend Interval operator-(const uint64_t& lhs, const Interval& rhs);
    friend Interval operator+(const Interval& lhs, const Seconds& rhs);
    friend Interval operator-(const Interval& lhs, const Seconds& rhs);
    friend Interval operator+(const Seconds& lhs, const Interval& rhs);
    friend Interval operator-(const Seconds& lhs, const Interval& rhs);
    friend Interval operator+(const Interval& lhs, const Microseconds& rhs);
    friend Interval operator-(const Interval& lhs, const Microseconds& rhs);
    friend Interval operator+(const Microseconds& lhs, const Interval& rhs);
    friend Interval operator-(const Microseconds& lhs, const Interval& rhs);
    friend Interval operator+(const Interval& lhs, const Days& rhs);
    friend Interval operator-(const Interval& lhs, const Days& rhs);
    friend Interval operator+(const Days& lhs, const Interval& rhs);
    friend Interval operator-(const Days& lhs, const Interval& rhs);

    friend Interval operator*(const Interval& lhs, const double& rhs);
    friend Interval operator/(const Interval& lhs, const double& rhs);

    friend bool operator==(const Interval& lhs, const Interval& rhs);
    friend bool operator!=(const Interval& lhs, const Interval& rhs);
    friend bool operator<(const Interval& lhs, const Interval& rhs);
    friend bool operator<=(const Interval& lhs, const Interval& rhs);
    friend bool operator>(const Interval& lhs, const Interval& rhs);
    friend bool operator>=(const Interval& lhs, const Interval& rhs);

private:
    template <typename Out>
    inline void split(const std::string& str, char delim, Out result) {
        std::istringstream iss(str);
        std::string item;
        while (std::getline(iss, item, delim))
        {
            if (!item.empty())
                *result++ = item;
        }
    }

    inline std::vector<std::string> split(const std::string& str, char delim = ' ')
    {
        auto result = std::vector<std::string>{};
        if (!str.empty())
            split(str, delim, std::back_inserter(result));
        return result;
    }

    inline void toLower(std::string& str)
    {
        for (auto& l : str)
            l = tolower(l);
    }

    template <typename Container, typename Value>
    inline bool any_of(const Container& container, const Value& value)
    {
        return std::any_of(container.begin(), container.end(), [&value](const auto& v) { return value == v; });
    }

private:
    uint64_t m_days;
    uint64_t m_seconds;
    uint64_t m_microseconds;
};


inline Interval operator+(const Interval& lhs, const Interval& rhs) { return (Interval{lhs} += rhs); }
inline Interval operator-(const Interval& lhs, const Interval& rhs) { return (Interval{lhs} -= rhs); }
inline Interval operator+(const Interval& lhs, const uint64_t& rhs) { return (Interval{lhs} += rhs); }
inline Interval operator-(const Interval& lhs, const uint64_t& rhs) { return (Interval{lhs} -= rhs); }
inline Interval operator+(const uint64_t& lhs, const Interval& rhs) { return (Interval{lhs} += rhs); }
inline Interval operator-(const uint64_t& lhs, const Interval& rhs) { return (Interval{lhs} -= rhs); }
inline Interval operator+(const Interval& lhs, const Seconds& rhs) { return (Interval{lhs} += rhs); }
inline Interval operator-(const Interval& lhs, const Seconds& rhs) { return (Interval{lhs} -= rhs); }
inline Interval operator+(const Seconds& lhs, const Interval& rhs) { return (Interval{lhs} += rhs); }
inline Interval operator-(const Seconds& lhs, const Interval& rhs) { return (Interval{lhs} -= rhs); }
inline Interval operator+(const Interval& lhs, const Microseconds& rhs) { return (Interval{lhs} += rhs); }
inline Interval operator-(const Interval& lhs, const Microseconds& rhs) { return (Interval{lhs} -= rhs); }
inline Interval operator+(const Microseconds& lhs, const Interval& rhs) { return (Interval{lhs} += rhs); }
inline Interval operator-(const Microseconds& lhs, const Interval& rhs) { return (Interval{lhs} -= rhs); }
inline Interval operator+(const Interval& lhs, const Days& rhs) { return (Interval{lhs} += rhs); }
inline Interval operator-(const Interval& lhs, const Days& rhs) { return (Interval{lhs} -= rhs); }
inline Interval operator+(const Days& lhs, const Interval& rhs) { return (Interval{lhs} += rhs); }
inline Interval operator-(const Days& lhs, const Interval& rhs) { return (Interval{lhs} -= rhs); }

inline Interval operator*(const Interval& lhs, const double& rhs) { return (Interval{lhs} *= rhs); }
inline Interval operator/(const Interval& lhs, const double& rhs) { return (Interval{lhs} /= rhs); }

inline bool operator==(const Interval& lhs, const Interval& rhs) { return lhs.m_days == rhs.m_days && lhs.m_seconds == rhs.m_seconds && lhs.m_microseconds == rhs.m_microseconds; }
inline bool operator!=(const Interval& lhs, const Interval& rhs) { return !operator==(lhs, rhs); }
inline bool operator<(const Interval& lhs, const Interval& rhs) { return (lhs.m_days < rhs.m_days) || (lhs.m_days == rhs.m_days && ((lhs.m_seconds < rhs.m_seconds) && (lhs.m_seconds == rhs.m_seconds && (lhs.m_days < rhs.m_days)))); }
inline bool operator<=(const Interval& lhs, const Interval& rhs) { return !operator>(lhs, rhs); }
inline bool operator>(const Interval& lhs, const Interval& rhs) { return (lhs.m_days > rhs.m_days) || (lhs.m_days == rhs.m_days && ((lhs.m_seconds > rhs.m_seconds) && (lhs.m_seconds == rhs.m_seconds && (lhs.m_days > rhs.m_days)))); }
inline bool operator>=(const Interval& lhs, const Interval& rhs) { return !operator<(lhs, rhs); }

}  // namespace esql::db::table::types
