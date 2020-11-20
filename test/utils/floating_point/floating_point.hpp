#pragma once

#include <cmath>
#include <concepts>

namespace pl {

template <typename T>
inline constexpr T kZeroFloatingPointThreshold;
template <>
inline constexpr float kZeroFloatingPointThreshold<float> = 1.0e-5;
template <>
inline constexpr double kZeroFloatingPointThreshold<double> = 1.0e-12;
template <>
inline constexpr long double kZeroFloatingPointThreshold<long double> =
    1.0e-16L;

template <typename T>
inline constexpr T kFloatingPointEqualityThreshold;
template <>
inline constexpr float kFloatingPointEqualityThreshold<float> = 1.0e-4;
template <>
inline constexpr double kFloatingPointEqualityThreshold<double> = 1.0e-5;
template <>
inline constexpr long double kFloatingPointEqualityThreshold<long double> =
    1.0e-8L;

template <std::floating_point T>
constexpr bool IsFloatingPointZero(T x) {
    return std::abs(x) < kZeroFloatingPointThreshold<T>;
}

// TODO: Take ULPs into consideration
template <std::floating_point T>
constexpr bool AreCloseEnough(T first,
                              T second,
                              T epsilon = kFloatingPointEqualityThreshold<T>) {
    return IsFloatingPointZero(std::abs(first - second)) or
           not(std::abs(first - second) >
               epsilon * std::max(std::abs(first), std::abs(second)));
}

}  // namespace pl