#pragma once

#include <concepts>
#include <sstream>

#include <catch2/catch.hpp>

#include "test/utils/floating_point/floating_point.hpp"

namespace pl {

template <std::floating_point T>
class PeriodicApprox : public Catch::MatcherBase<T> {
  public:
    explicit PeriodicApprox(T value, T lower_value, T period, T epsilon)
        : value_(value),
          lower_value_(lower_value),
          period_(period),
          tolerance_(epsilon) {}

    // Performs the test for this matcher
    [[nodiscard]] bool match(const T& other) const override {
        auto const value_to_compare =
            AreCloseEnough(value_, lower_value_, tolerance_) ? value_ + period_
                                                             : value_;
        auto const other_to_compare =
            AreCloseEnough(other, lower_value_, tolerance_) ? other + period_
                                                            : other;

        return AreCloseEnough(value_to_compare, other_to_compare, tolerance_);
    }

    // Produces a string describing what this matcher does.
    [[nodiscard]] std::string describe() const override {
        std::ostringstream ss;
        ss << " is approx to ";
        ss << value_ << " with a tolerance of " << tolerance_;
        return ss.str();
    }

  private:
    T value_;
    T lower_value_;
    T period_;
    T tolerance_;
};

// Builder function
template <std::floating_point T>
inline PeriodicApprox<T> IsPeriodicApprox(
    T value,
    T lower_bound,
    T period,
    T tolerance = kFloatingPointEqualityThreshold<T>) {
    return PeriodicApprox(value, lower_bound, period, tolerance);
}

}  // namespace pl
