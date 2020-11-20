#pragma once

#include <cmath>
#include <concepts>
#include <numbers>

namespace pl {

template <std::floating_point T>
constexpr T mod2pi(T t) noexcept {
    constexpr T full_turn = static_cast<T>(2) * std::numbers::pi_v<T>;
    T signed_mod = std::fmod(t, full_turn);
    return signed_mod < static_cast<T>(0.0) ? full_turn + signed_mod
                                            : signed_mod;
}

template <std::floating_point T>
constexpr T ValueBetweenMinusPiAndPi(T t) noexcept {
    return mod2pi(t + std::numbers::pi_v<T>) - std::numbers::pi_v<T>;
}

// Angle's value in ( -pi , pi )
template <std::floating_point T>
class Angle {
  public:
    using value_type = T;

    Angle() = default;
    explicit Angle(T alpha) noexcept
        : value_{ValueBetweenMinusPiAndPi(alpha)} {}

    T value() const noexcept { return value_; }

  private:
    T value_{};
};

template <std::floating_point T>
Angle<T> AngleFromDegrees(T deg) {
    return Angle<T>(std::numbers::pi_v<T> * deg / static_cast<T>(180.0));
}

template <std::floating_point T>
T DegreesFromAngle(Angle<T> angle) {
    return static_cast<T>(180.0) * angle.value() / std::numbers::pi_v<T>;
}

template <std::floating_point T>
Angle<T> operator+(Angle<T> const& alpha, Angle<T> const& beta) {
    return Angle<T>(alpha.value() + beta.value());
}

template <std::floating_point T>
Angle<T> operator-(Angle<T> alpha, Angle<T> beta) {
    return Angle<T>(alpha.value() - beta.value());
}

template <std::floating_point T>
T Distance(Angle<T> alpha, Angle<T> beta) {
    auto const diff = beta - alpha;
    return std::abs(diff.value());
}

template <typename From, typename Into>
using ConstantTypeConversion = Into;

template <std::floating_point T,
          typename Operator,
          std::convertible_to<T>... Ts>
T OperateAsAngle(Operator op, Ts... values) noexcept
    requires std::regular_invocable<Operator,
                                    ConstantTypeConversion<Ts, Angle<T>>...> {
    Angle<T> result = op(Angle<T>(static_cast<T>(values))...);
    return result.value();
}

}  // namespace pl
