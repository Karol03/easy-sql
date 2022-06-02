/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <inttypes.h>
#include <string>


namespace esql::db::table::types
{

struct Time
{
public:
    Time()
        : m_hour{}
        , m_minute{}
        , m_second{}
    {}

    Time(uint32_t second)
    {
        uint32_t minute = second/60;
        uint32_t hour = minute/60;

        m_second = second % 60;
        m_minute = minute % 60;
        m_hour = hour % 24;
    }

    Time(uint32_t hour, uint32_t minute, uint32_t second)
    {
        minute += second/60;
        hour += minute/60;

        m_second = second % 60;
        m_minute = minute % 60;
        m_hour = hour % 24;
    }

    Time(std::string value)
    {
        if (value.empty())
        {
            m_hour = 0;
            m_minute = 0;
            m_second = 0;
            return;
        }

        auto second = uint32_t{};
        auto minute = uint32_t{};
        auto hour = uint32_t{};

        auto i = int(value.size() - 1);
        for (; i >= 0; --i)
        {
            if (!isdigit(value[i]))
            {
                second = atol(value.substr(i + 1).c_str());
                break;
            }
            else if (i == 0)
            {
                second = atol(value.substr(0).c_str());
            }
        }

        if (i > 0 && !isdigit(value[i]))
            --i;

        auto j = i;
        for (; j >= 0; --j)
        {
            if (!isdigit(value[j]))
            {
                minute = atol(value.substr(j + 1, i - j).c_str());
                break;
            }
            else if (j == 0)
            {
                minute = atol(value.substr(0, i - j + 1).c_str());
            }
        }

        if (j > 0 && !isdigit(value[j]))
            --j;

        for (i = j; i >= 0; --i)
        {
            if (!isdigit(value[i]))
            {
                hour = atol(value.substr(i + 1, j - i).c_str());
                break;
            }
            else if (i == 0)
            {
                hour = atol(value.substr(0, j - i + 1).c_str());
            }
        }

        minute += second/60;
        hour += minute/60;

        m_second = second % 60;
        m_minute = minute % 60;
        m_hour = hour % 24;
    }

    Time(const Time&) = default;
    Time& operator=(const Time&) = default;
    Time(Time&&) = default;
    Time& operator=(Time&&) = default;

    inline uint16_t hour() const { return m_hour; }
    inline uint16_t minute() const { return m_minute; }
    inline uint16_t second() const { return m_second; }
    inline void hour(uint16_t value)
    {
        if (value < 24) {
            m_hour = value;
            return;
        }
        m_hour = 23;
        m_minute = 59;
        m_second = 59;
    }
    inline void minute(uint16_t value) { m_minute = std::min(value, uint16_t{60}); }
    inline void second(uint16_t value) { m_second = std::min(value, uint16_t{60}); }

    std::string to_string(const char delimiter = ':') const
    {
        char buffer[8] = {};
        sprintf(buffer, "%02d%c%02d%c%02d", uint16_t{m_hour}, delimiter,
                                            uint16_t{m_minute}, delimiter,
                                            uint16_t{m_second});
        return buffer;
    }

    inline operator bool() { return m_hour || m_minute || m_second; }

    inline Time& operator+=(const Time& rhs)
    {
        m_second += rhs.m_second;
        m_minute += rhs.m_minute + m_second/60;
        m_hour += rhs.m_hour + m_minute/60;

        m_second %= 60;
        m_minute %= 60;
        m_hour %= 24;
        return *this;
    }

    inline Time& operator-=(const Time& rhs)
    {
        m_second = m_second - rhs.m_second + 60;
        m_minute = m_minute - 1 - rhs.m_minute + 60 + (m_second/60);
        m_hour = 23 + m_hour - rhs.m_hour + (m_minute/60);

        m_second %= 60;
        m_minute %= 60;
        m_hour %= 24;
        return *this;
    }

    inline Time& operator+=(const uint32_t& seconds) { return (*this += Time{seconds}); }
    inline Time& operator-=(const uint32_t& seconds) { return (*this -= Time{seconds}); }

    friend Time operator+(const Time& lhs, const Time& rhs);
    friend Time operator-(const Time& lhs, const Time& rhs);
    friend Time operator+(const Time& lhs, const uint32_t& rhs);
    friend Time operator-(const Time& lhs, const uint32_t& rhs);
    friend Time operator+(const uint32_t& lhs, const Time& rhs);
    friend Time operator-(const uint32_t& lhs, const Time& rhs);

    friend bool operator==(const Time& lhs, const Time& rhs);
    friend bool operator!=(const Time& lhs, const Time& rhs);
    friend bool operator<(const Time& lhs, const Time& rhs);
    friend bool operator<=(const Time& lhs, const Time& rhs);
    friend bool operator>(const Time& lhs, const Time& rhs);
    friend bool operator>=(const Time& lhs, const Time& rhs);

private:
    uint8_t m_hour;
    uint8_t m_minute;
    uint8_t m_second;
};


inline Time operator+(const Time& lhs, const Time& rhs) { return (Time{lhs} += rhs); }
inline Time operator-(const Time& lhs, const Time& rhs) { return (Time{lhs} -= rhs); }
inline Time operator+(const Time& lhs, const uint32_t& rhs) { return (Time{lhs} += rhs); }
inline Time operator-(const Time& lhs, const uint32_t& rhs) { return (Time{lhs} -= rhs); }
inline Time operator+(const uint32_t& lhs, const Time& rhs) { return (Time{lhs} += rhs); }
inline Time operator-(const uint32_t& lhs, const Time& rhs) { return (Time{lhs} -= rhs); }

inline bool operator==(const Time& lhs, const Time& rhs) { return lhs.m_hour == rhs.m_hour && lhs.m_minute == rhs.m_minute && lhs.m_second == rhs.m_second; }
inline bool operator!=(const Time& lhs, const Time& rhs) { return !operator==(lhs, rhs); }
inline bool operator<(const Time& lhs, const Time& rhs) { return lhs.m_hour < rhs.m_hour || (lhs.m_hour == rhs.m_hour && (lhs.m_minute < rhs.m_minute || (lhs.m_minute == rhs.m_minute && lhs.m_second < rhs.m_second))); }
inline bool operator<=(const Time& lhs, const Time& rhs) { return !operator>(lhs, rhs); }
inline bool operator>(const Time& lhs, const Time& rhs){ return lhs.m_hour > rhs.m_hour || (lhs.m_hour == rhs.m_hour && (lhs.m_minute > rhs.m_minute || (lhs.m_minute == rhs.m_minute && lhs.m_second > rhs.m_second))); }
inline bool operator>=(const Time& lhs, const Time& rhs) { return !operator<(lhs, rhs); }

}  // namespace esql::db::table::types
