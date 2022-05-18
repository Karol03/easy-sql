/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <sstream>
#include <string>
#include <variant>
#include <vector>


struct Date { std::string to_string() const { return {}; } };
struct Interval { std::string to_string() const { return {}; } };
struct Time { std::string to_string() const { return {}; } };
struct Timestamp { std::string to_string() const { return {}; } };

using Text = std::string;

using Smallint = int16_t;
using Int = int32_t;
using Bigint = int64_t;

using Float8 = double;
using Double = double;
using Real = float;

using Boolean = bool;

template <typename T>
using Array = std::vector<T>;



namespace epsql::db::table
{

template <typename T> inline const char* nameOf() { return "UNKNOWN"; }
template <> inline const char* nameOf<Date>() { return "Date"; }
template <> inline const char* nameOf<Interval>() { return "Interval"; }
template <> inline const char* nameOf<Time>() { return "Time"; }
template <> inline const char* nameOf<Timestamp>() { return "Timestamp"; }
template <> inline const char* nameOf<Text>() { return "Text"; }
template <> inline const char* nameOf<Smallint>() { return "Smallint"; }
template <> inline const char* nameOf<Int>() { return "Int"; }
template <> inline const char* nameOf<Bigint>() { return "Bigint"; }
template <> inline const char* nameOf<Boolean>() { return "Boolean"; }
template <> inline const char* nameOf<Float8>() { return "Double precision"; }
template <> inline const char* nameOf<Real>() { return "Real"; }


template <typename T> inline std::string getValue(const T&) { return "NULL"; }
template <> inline std::string getValue(const Smallint& value) { return std::to_string(value); }
template <> inline std::string getValue(const Int& value) { return std::to_string(value); }
template <> inline std::string getValue(const Bigint& value) { return std::to_string(value); }
template <> inline std::string getValue(const Real& value) { return std::to_string(value); }
template <> inline std::string getValue(const Float8& value) { return std::to_string(value); }
template <> inline std::string getValue(const Boolean& value) { auto result = std::stringstream{}; result << std::boolalpha << value; return result.str(); }
template <> inline std::string getValue(const Text& value) { return std::string{"'"} + value + "'"; }
template <> inline std::string getValue(const Date& value) { return value.to_string(); }
template <> inline std::string getValue(const Interval& value) { return value.to_string(); }
template <> inline std::string getValue(const Time& value) { return value.to_string(); }
template <> inline std::string getValue(const Timestamp& value) { return value.to_string(); }

using TableVariant = std::variant<Smallint, Int, Bigint,
                                  Real, Float8,
                                  Boolean,
                                  Text,
                                  Date, Interval, Time, Timestamp,
                                  void*>;

}  // namespace epsql::db::table
