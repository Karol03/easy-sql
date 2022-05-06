/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <array>
#include <string>
#include <vector>


template <std::size_t Size>
using Char = std::array<char, Size>;
template <std::size_t Size>
using Varchar = std::array<char, Size>;
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



namespace epsql::db
{

template <typename T> inline const char* nameOf() { return "UNKNOWN"; }
template <> inline const char* nameOf<Text>() { return "Text"; }

template <> inline const char* nameOf<Smallint>() { return "Smallint"; }
template <> inline const char* nameOf<Int>() { return "Int"; }
template <> inline const char* nameOf<Bigint>() { return "Bigint"; }

template <> inline const char* nameOf<Boolean>() { return "Boolean"; }

template <> inline const char* nameOf<Float8>() { return "Double precision"; }
template <> inline const char* nameOf<Real>() { return "Real"; }

}  // namespace epsql::db
