/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include "date.hpp"
#include "time.hpp"


namespace esql::db::table::types
{

struct Timestamp
{
public:
    Timestamp()
        : m_date{}
        , m_time{}
    {}

    Timestamp(Date date, Time time)
        : m_date{std::move(date)}
        , m_time{std::move(time)}
    {}

    Timestamp(Time time, Date date)
        : m_date{std::move(date)}
        , m_time{std::move(time)}
    {}

    Timestamp(std::string datetime)
    {
        if (datetime.empty())
            return;

        auto delimiter = datetime.find(' ');
        if (delimiter != std::string::npos)
        {
            auto date = datetime.substr(0, delimiter);
            auto time = datetime.substr(delimiter + 1);
            m_date = Date{std::move(date)};
            m_time = Time{std::move(time)};
        }
    }

    Timestamp(const Timestamp&) = default;
    Timestamp& operator=(const Timestamp&) = default;
    Timestamp(Timestamp&&) = default;
    Timestamp& operator=(Timestamp&&) = default;

    std::string to_string() const
    {
        auto result = std::string{};
        result = m_date.to_string() + ' ' + m_time.to_string();
        return result;
    }

    inline uint16_t hour() const { return m_time.hour(); }
    inline uint16_t minute() const { return m_time.minute(); }
    inline uint16_t second() const { return m_time.second(); }
    inline uint16_t day() const { return m_date.day(); }
    inline uint16_t month() const { return m_date.month(); }
    inline uint16_t year() const { return m_date.year(); }

    inline void hour(uint16_t value) { m_time.hour(value); }
    inline void minute(uint16_t value) { m_time.minute(value); }
    inline void second(uint16_t value) { m_time.second(value); }
    inline void day(uint16_t value) { m_date.day(value); }
    inline void month(uint16_t value) { m_date.month(value); }
    inline void year(uint16_t value) { m_date.year(value); }

    inline Timestamp& advance(const Time& time)
    {
        constexpr auto SINGLE_DAY = 1u;
        m_time += time;
        if (m_time < time)
            advance(SINGLE_DAY);
        return *this;
    }
    inline Timestamp& advance(const Date& date)
    {
        m_date += date;
        return *this;
    }
    inline Timestamp& advance(uint32_t days)
    {
        m_date += days;
        return *this;
    }
    inline Timestamp& back(const Time& time)
    {
        constexpr auto SINGLE_DAY = 1u;
        m_time -= time;
        if (m_time > time)
            back(SINGLE_DAY);
        return *this;
    }
    inline Timestamp& back(const Date& date)
    {
        m_date -= date;
        return *this;
    }
    inline Timestamp& back(uint32_t days)
    {
        m_date -= days;
        return *this;
    }

    inline Timestamp& operator+=(const Timestamp& timestamp)
    {
        advance(timestamp.m_time);
        return advance(timestamp.m_date);
    }
    inline Timestamp& operator-=(const Timestamp& timestamp)
    {
        back(timestamp.m_time);
        return back(timestamp.m_date);
    }

    inline Timestamp& operator+=(const Date& date) { return advance(date); }
    inline Timestamp& operator-=(const Date& date) { return back(date); }
    inline Timestamp& operator+=(const uint32_t& days) { return advance(days); }
    inline Timestamp& operator-=(const uint32_t& days) { return back(days); }
    inline Timestamp& operator+=(const Time& time) { return advance(time); }
    inline Timestamp& operator-=(const Time& time) { return back(time); }

    friend Timestamp operator+(const Timestamp& lhs, const Timestamp& rhs);
    friend Timestamp operator-(const Timestamp& lhs, const Timestamp& rhs);
    friend Timestamp operator+(const Timestamp& lhs, const uint32_t& rhs);
    friend Timestamp operator-(const Timestamp& lhs, const uint32_t& rhs);
    friend Timestamp operator+(const Timestamp& lhs, const Time& rhs);
    friend Timestamp operator-(const Timestamp& lhs, const Time& rhs);
    friend Timestamp operator+(const Time& lhs, const Timestamp& rhs);
    friend Timestamp operator+(const Timestamp& lhs, const Date& rhs);
    friend Timestamp operator-(const Timestamp& lhs, const Date& rhs);
    friend Timestamp operator+(const Date& lhs, const Timestamp& rhs);

    friend bool operator==(const Timestamp& lhs, const Timestamp& rhs);
    friend bool operator!=(const Timestamp& lhs, const Timestamp& rhs);
    friend bool operator<(const Timestamp& lhs, const Timestamp& rhs);
    friend bool operator<=(const Timestamp& lhs, const Timestamp& rhs);
    friend bool operator>(const Timestamp& lhs, const Timestamp& rhs);
    friend bool operator>=(const Timestamp& lhs, const Timestamp& rhs);

private:
    Date m_date;
    Time m_time;
};

inline Timestamp operator+(const Timestamp& lhs, const Timestamp& rhs) { return (Timestamp{lhs} += rhs); }
inline Timestamp operator-(const Timestamp& lhs, const Timestamp& rhs) { return (Timestamp{lhs} -= rhs); }
inline Timestamp operator+(const Timestamp& lhs, const uint32_t& rhs) { return (Timestamp{lhs} += rhs); }
inline Timestamp operator-(const Timestamp& lhs, const uint32_t& rhs) { return (Timestamp{lhs} -= rhs); }
inline Timestamp operator+(const Timestamp& lhs, const Time& rhs) { return (Timestamp{lhs} += rhs); }
inline Timestamp operator-(const Timestamp& lhs, const Time& rhs) { return (Timestamp{lhs} -= rhs); }
inline Timestamp operator+(const Time& lhs, const Timestamp& rhs) { return (Timestamp{rhs} += lhs); }
inline Timestamp operator+(const Timestamp& lhs, const Date& rhs) { return (Timestamp{lhs} += rhs); }
inline Timestamp operator-(const Timestamp& lhs, const Date& rhs) { return (Timestamp{lhs} -= rhs); }
inline Timestamp operator+(const Date& lhs, const Timestamp& rhs) { return (Timestamp{rhs} += lhs); }

inline bool operator==(const Timestamp& lhs, const Timestamp& rhs) { return lhs.m_time == rhs.m_time && lhs.m_date == rhs.m_date; }
inline bool operator!=(const Timestamp& lhs, const Timestamp& rhs) { return !operator==(lhs, rhs); }
inline bool operator<(const Timestamp& lhs, const Timestamp& rhs) { return lhs.m_date < rhs.m_date || (lhs.m_date == rhs.m_date && lhs.m_time < rhs.m_time); }
inline bool operator<=(const Timestamp& lhs, const Timestamp& rhs) { return !operator>(lhs, rhs); }
inline bool operator>(const Timestamp& lhs, const Timestamp& rhs) { return lhs.m_date > rhs.m_date || (lhs.m_date == rhs.m_date && lhs.m_time > rhs.m_time); }
inline bool operator>=(const Timestamp& lhs, const Timestamp& rhs) { return !operator<(lhs, rhs); }

}  // namespace esql::db::table::types
