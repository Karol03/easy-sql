/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <algorithm>
#include <inttypes.h>
#include <string>


namespace esql::db::table::types
{

struct Date
{
public:
    Date()
        : m_day{}
        , m_month{}
        , m_year{}
    {}

    Date(uint32_t days)
        : m_day{}
        , m_month{}
        , m_year{}
    {
        if (days > 0)
            advance(days - 1);
    }

    Date(uint32_t year, uint32_t month, uint32_t day)
        : m_day{}
        , m_month{}
        , m_year{}
    {
        if (month > 0)
            --month;
        if (day > 0)
            --day;
        m_year = year + month / 12;
        m_month = month % 12;
        advance(day);
    }

    Date(std::string value)
        : m_day{}
        , m_month{}
        , m_year{}
    {
        if (value.empty())
            return;

        auto day = uint32_t{};
        auto month = uint32_t{};
        auto year = uint32_t{};

        auto i = 0u;
        while (i < value.size() && isdigit(value[i]))
            ++i;
        day = atol(value.substr(0, i).c_str());

        ++i;
        if (i < value.size())
        {
            auto j = i;
            while (i < value.size() && isdigit(value[i]))
                ++i;
            month = atol(value.substr(j, i).c_str());

            ++i;
            if (i < value.size())
            {
                j = i;
                while (i < value.size() && isdigit(value[i]))
                    ++i;
                year = atol(value.substr(j, i).c_str());
            }
        }

        *this = Date(year, month, day);
    }

    Date(const Date&) = default;
    Date& operator=(const Date&) = default;
    Date(Date&&) = default;
    Date& operator=(Date&&) = default;

    std::string to_string(bool isStartFromYear = true, bool isSlashDelimiter = false) const
    {
        char buffer[32] = {};
        if (isStartFromYear)
        {
            if (isSlashDelimiter)
                sprintf(buffer, "%d/%02d/%02d", m_year, m_month + 1, m_day + 1);
            else
                sprintf(buffer, "%d-%02d-%02d", m_year, m_month + 1, m_day + 1);
        }
        else
        {
            if (isSlashDelimiter)
                sprintf(buffer, "%02d/%02d/%d", m_day + 1, m_month + 1, m_year);
            else
                sprintf(buffer, "%02d-%02d-%d", m_day + 1, m_month + 1, m_year);
        }
        return buffer;
    }

    inline uint16_t day() const { return m_day + 1; }
    inline uint16_t month() const { return m_month + 1; }
    inline uint16_t year() const { return m_year; }

    inline void day(uint16_t value)
    {
        if (value > 0)
            --value;

        constexpr auto T30_DAYS_MONTHS = 0b010100101000;
        constexpr auto MIN_DAY = uint16_t{0};
        auto maxDay = uint16_t{31 - 1};

        if (m_month == 1)
            maxDay = 27 + !(m_year%4);
        else if ((T30_DAYS_MONTHS >> m_month) & 0b1)
            maxDay = 29;

        m_day = std::clamp(value, MIN_DAY, maxDay);
    }
    inline void month(uint16_t value)
    {
        if (value > 0)
            --value;

        constexpr auto JANUARY = uint16_t{0};
        constexpr auto DECEMBER = uint16_t{11};
        m_month = std::clamp(value, JANUARY, DECEMBER);
    }
    inline void year(uint16_t value) { m_year = value; }

    inline Date& operator+=(const Date& date)
    {
        m_month += (date.m_month + 1);
        m_year += date.m_year + m_month / 12;
        m_month %= 12;
        advance(date.m_day + 1);
        return *this;
    }

    inline Date& operator-=(const Date& date)
    {
        if (date > *this)
        {
            m_year = 0;
            m_month = 0;
            m_day = 0;
            return *this;
        }

        back(date.m_day + 1);
        m_month = 12 + m_month - (date.m_month + 1);
        m_year = m_year - date.m_year - (m_month < 12);
        m_month %= 12;
        return *this;
    }

    inline Date& operator+=(const uint32_t& days) { advance(days); return *this; }
    inline Date& operator-=(const uint32_t& days) { back(days); return *this; }

    friend Date operator+(const Date& lhs, const Date& rhs);
    friend Date operator-(const Date& lhs, const Date& rhs);
    friend Date operator+(const Date& lhs, const uint32_t& rhs);
    friend Date operator-(const Date& lhs, const uint32_t& rhs);
    friend Date operator+(const uint32_t& lhs, const Date& rhs);
    friend Date operator-(const uint32_t& lhs, const Date& rhs);

    friend bool operator==(const Date& lhs, const Date& rhs);
    friend bool operator!=(const Date& lhs, const Date& rhs);
    friend bool operator<(const Date& lhs, const Date& rhs);
    friend bool operator<=(const Date& lhs, const Date& rhs);
    friend bool operator>(const Date& lhs, const Date& rhs);
    friend bool operator>=(const Date& lhs, const Date& rhs);

private:
    void back(uint32_t days)
    {
        constexpr uint8_t DAYS_PER_MONTH[] = { 30, 27, 30, 29, 30, 29, 30, 30, 29, 30, 29, 30 };

        while (days != 0)
        {
            if (m_day == 0)
            {
                if (m_month == 0)
                {
                    if (m_year == 0)
                        return;
                    m_month = 12;
                    --m_year;
                }
                --m_month;
                m_day = DAYS_PER_MONTH[(m_month + 1) % 12];
                if ((m_month == 1) && (m_year%4 == 0))
                    m_day += 1;
            }

            if (m_day < days)
            {
                days -= m_day;
                m_day = 0;
            }
            else
            {
                m_day -= days;
                days = 0;
            }
        }
    }

    void advance(uint32_t days)
    {
        constexpr auto MIN_DAY = uint32_t{0};
        constexpr uint8_t DAYS_PER_MONTH[] = { 30, 27, 30, 29, 30, 29, 30, 30, 29, 30, 29, 30 };
        constexpr auto T30_DAYS_MONTHS = 0b010100101000;

        while (days != 0)
        {
            auto maxDay = uint32_t{31 - 1};

            if (m_month == 1)
                maxDay = 27 + !(m_year%4);
            else if ((T30_DAYS_MONTHS >> m_month) & 0b1)
                maxDay = 29;

            maxDay -= m_day;
            const auto adv = std::clamp(days, MIN_DAY, maxDay);
            m_day += adv;
            days -= adv;

            if (m_day >= DAYS_PER_MONTH[m_month] && days)
            {
                --days;
                m_day = 0;
                ++m_month;
                if (m_month == 12)
                {
                    m_month = 0;
                    ++m_year;
                }
            }
        }
    }


public:
    uint8_t m_day;
    uint8_t m_month;
    uint16_t m_year;
};


inline Date operator+(const Date& lhs, const Date& rhs) { return (Date{lhs} += rhs); }
inline Date operator-(const Date& lhs, const Date& rhs) { return (Date{lhs} -= rhs); }
inline Date operator+(const Date& lhs, const uint32_t& rhs) { return (Date{lhs} += rhs); }
inline Date operator-(const Date& lhs, const uint32_t& rhs) { return (Date{lhs} -= rhs); }
inline Date operator+(const uint32_t& lhs, const Date& rhs) { return (Date{lhs} += rhs); }
inline Date operator-(const uint32_t& lhs, const Date& rhs) { return (Date{lhs} -= rhs); }

inline bool operator==(const Date& lhs, const Date& rhs) { return lhs.m_year == rhs.m_year && lhs.m_month == rhs.m_month && lhs.m_day == rhs.m_day; }
inline bool operator!=(const Date& lhs, const Date& rhs) { return !operator==(lhs, rhs); }
inline bool operator<(const Date& lhs, const Date& rhs) { return lhs.m_year < rhs.m_year || (lhs.m_year == rhs.m_year && (lhs.m_month < rhs.m_month || (lhs.m_month == rhs.m_month && lhs.m_day < rhs.m_day))); }
inline bool operator<=(const Date& lhs, const Date& rhs) { return !operator>(lhs, rhs); }
inline bool operator>(const Date& lhs, const Date& rhs){ return lhs.m_year > rhs.m_year || (lhs.m_year == rhs.m_year && (lhs.m_month > rhs.m_month || (lhs.m_month == rhs.m_month && lhs.m_day > rhs.m_day))); }
inline bool operator>=(const Date& lhs, const Date& rhs) { return !operator<(lhs, rhs); }

}  // namespace esql::db::table::types
