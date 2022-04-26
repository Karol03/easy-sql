/**
 * Created by Karol Dudzic @ 2022
 */
#pragma once

#include <array>
#include <string>
#include <vector>


namespace epsql::db::types
{

template <std::size_t Size>
using Char = std::array<char, Size>;
template <std::size_t Size>
using Varchar = std::array<char, Size>;
using Text = std::string;

using SmallInt = int16_t;
using Int = int32_t;

using Float = float;
using Float8 = double;
using Real = double;

template <typename T>
using Array = std::vector<T>;

}  // namespace epsql::db::types
