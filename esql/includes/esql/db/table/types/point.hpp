/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <cmath>
#include <string>


namespace esql::db::table::types
{

struct Point
{
public:
    Point() : m_x{}, m_y{} {}
    Point(double x, double y) : m_x{x}, m_y{y} {}
    Point(const Point&) = default;
    Point& operator=(const Point&) = default;
    Point(Point&&) = default;
    Point& operator=(Point&&) = default;

    inline std::string to_string(unsigned precission = 5) const
    {
        precission = std::min(precission, 12u);
        char buffer[64] = {};
        sprintf(buffer, "(%.*lf, %.*lf)", precission, m_x, precission, m_y);
        return buffer;
    }

    inline double x() const { return m_x; }
    inline double y() const { return m_y; }
    inline void x(double value) { m_x = value; }
    inline void y(double value) { m_y = value; }
    inline double manhattanDistance() const { return m_x + m_y; }
    inline double euclideanDistance() const { return std::sqrt(m_x*m_x + m_y*m_y); }
    inline double length() const { return euclideanDistance(); }
    inline Point norm() const { const auto l = length(); return Point{m_x / l, m_y / l}; }
    inline Point versor() const { return norm(); }
    inline Point& reverse() { std::swap(m_x, m_y); return *this; }

    inline operator bool() { return m_x || m_y; }

    inline Point& operator+=(const Point& rhs) { m_x += rhs.m_x; m_y += rhs.m_y; return *this; }
    inline Point& operator-=(const Point& rhs) { m_x -= rhs.m_x; m_y -= rhs.m_y; return *this; }
    inline Point& operator*=(const Point& rhs) { m_x *= rhs.m_x; m_y *= rhs.m_y; return *this; }
    inline Point& operator/=(const Point& rhs) { m_x /= rhs.m_x; m_y /= rhs.m_y; return *this; }
    inline Point& operator+=(const double& value) { m_x += value; m_y += value; return *this; }
    inline Point& operator-=(const double& value) { m_x -= value; m_y -= value; return *this; }
    inline Point& operator*=(const double& value) { m_x *= value; m_y *= value; return *this; }
    inline Point& operator/=(const double& value) { m_x /= value; m_y /= value; return *this; }

    friend Point operator+(const Point& lhs, const Point& rhs);
    friend Point operator-(const Point& lhs, const Point& rhs);
    friend Point operator*(const Point& lhs, const Point& rhs);
    friend Point operator/(const Point& lhs, const Point& rhs);
    friend Point operator+(const Point& lhs, const double& rhs);
    friend Point operator-(const Point& lhs, const double& rhs);
    friend Point operator*(const Point& lhs, const double& rhs);
    friend Point operator/(const Point& lhs, const double& rhs);
    friend Point operator+(const double& lhs, const Point& rhs);
    friend Point operator-(const double& lhs, const Point& rhs);
    friend Point operator*(const double& lhs, const Point& rhs);
    friend Point operator/(const double& lhs, const Point& rhs);

    friend bool operator==(const Point& lhs, const Point& rhs);
    friend bool operator!=(const Point& lhs, const Point& rhs);
    friend bool operator<(const Point& lhs, const Point& rhs);
    friend bool operator<=(const Point& lhs, const Point& rhs);
    friend bool operator>(const Point& lhs, const Point& rhs);
    friend bool operator>=(const Point& lhs, const Point& rhs);

private:
    double m_x;
    double m_y;
};


inline Point operator+(const Point& lhs, const Point& rhs) { return Point{lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y}; }
inline Point operator-(const Point& lhs, const Point& rhs) { return Point{lhs.m_x - rhs.m_x, lhs.m_y - rhs.m_y}; }
inline Point operator*(const Point& lhs, const Point& rhs) { return Point{lhs.m_x * rhs.m_x, lhs.m_y * rhs.m_y}; }
inline Point operator/(const Point& lhs, const Point& rhs) { return Point{lhs.m_x / rhs.m_x, lhs.m_y / rhs.m_y}; }
inline Point operator+(const Point& lhs, const double& rhs) { return Point{lhs.m_x + rhs, lhs.m_y + rhs}; }
inline Point operator-(const Point& lhs, const double& rhs) { return Point{lhs.m_x - rhs, lhs.m_y - rhs}; }
inline Point operator*(const Point& lhs, const double& rhs) { return Point{lhs.m_x * rhs, lhs.m_y * rhs}; }
inline Point operator/(const Point& lhs, const double& rhs) { return Point{lhs.m_x / rhs, lhs.m_y / rhs}; }
inline Point operator+(const double& lhs, const Point& rhs) { return Point{lhs / rhs.m_x, lhs / rhs.m_y}; }
inline Point operator-(const double& lhs, const Point& rhs) { return Point{lhs - rhs.m_x, lhs - rhs.m_y}; }
inline Point operator*(const double& lhs, const Point& rhs) { return Point{lhs * rhs.m_x, lhs * rhs.m_y}; }
inline Point operator/(const double& lhs, const Point& rhs) { return Point{lhs / rhs.m_x, lhs / rhs.m_y}; }

inline bool operator==(const Point& lhs, const Point& rhs) { return lhs.m_x == rhs.m_x && lhs.m_y == lhs.m_y; }
inline bool operator!=(const Point& lhs, const Point& rhs) { return lhs.m_x != rhs.m_x || lhs.m_y != lhs.m_y; }
inline bool operator<(const Point& lhs, const Point& rhs) { return (lhs.m_x < rhs.m_x) || (lhs.m_x == rhs.m_x && lhs.m_y < rhs.m_y); }
inline bool operator<=(const Point& lhs, const Point& rhs){ return !operator>(lhs, rhs); }
inline bool operator>(const Point& lhs, const Point& rhs) { return (lhs.m_x > rhs.m_x) || (lhs.m_x == rhs.m_x && lhs.m_y > rhs.m_y); }
inline bool operator>=(const Point& lhs, const Point& rhs) { return !operator<(lhs, rhs); }

}  // namespace esql::db::table::types
