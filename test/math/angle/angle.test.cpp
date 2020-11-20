#include <numbers>

#include <catch2/catch.hpp>

#include "src/math/angle/angle.hpp"
#include "test/utils/catch/approx_periodic.hpp"

namespace pl {

TEMPLATE_TEST_CASE("0-param constructor gives 0",
                   "[angle][constructor]",
                   float,
                   double,
                   long double) {
    Angle<TestType> zero;
    CHECK(zero.value() == static_cast<TestType>(0.0));
}

TEMPLATE_TEST_CASE("1-param constructor on values in (-pi, pi)",
                   "[angle][constructor]",
                   float,
                   double,
                   long double) {
    auto const offset = GENERATE(take(
        50,
        random(-std::numbers::pi_v<TestType>, std::numbers::pi_v<TestType>)));
    Angle const non_wrapped_angle(offset);
    CHECK(non_wrapped_angle.value() == offset);
}

TEMPLATE_TEST_CASE("1-param constructor on values outside (-pi, pi)",
                   "[angle][constructor]",
                   float,
                   double,
                   long double) {
    auto const offset = GENERATE(take(
        10,
        random(-std::numbers::pi_v<TestType>, std::numbers::pi_v<TestType>)));
    int const num_full_turns = GENERATE(1, 5, 7, -1, -3, -6);
    Angle const wrapped_angle(offset + static_cast<TestType>(num_full_turns) *
                                           std::numbers::pi_v<TestType> *
                                           static_cast<TestType>(2.0));

    CHECK_THAT(wrapped_angle.value(),
               IsPeriodicApprox(
                   offset,
                   -std::numbers::pi_v<TestType>,
                   std::numbers::pi_v<TestType> * static_cast<TestType>(2.0)));
}

TEMPLATE_TEST_CASE("Type correctness: Allowed in constructor",
                   "[angle][types]",
                   float,
                   double,
                   long double) {
    auto value = static_cast<TestType>(0.0);

    Angle angle(value);

    REQUIRE(
        std::is_same<typename decltype(angle)::value_type, TestType>::value);
}

TEMPLATE_TEST_CASE("operator+",
                   "[angle][operator]",
                   float,
                   double,
                   long double) {
    constexpr auto pi_halves =
        std::numbers::pi_v<TestType> / static_cast<TestType>(2.0);
    auto const alpha_raw =
        GENERATE(take(10, random(static_cast<TestType>(0.0), pi_halves)));
    auto const beta_raw =
        GENERATE(take(10, random(static_cast<TestType>(0.0), pi_halves)));

    constexpr auto margin_value = kFloatingPointEqualityThreshold<TestType>;

    SECTION("Both positive, no wrapping") {
        Angle const alpha(alpha_raw);
        Angle const beta(beta_raw);
        auto const expected_sum = alpha_raw + beta_raw;

        CHECK((alpha + beta).value() ==
              Approx(expected_sum).margin(margin_value));
    }
    SECTION("Both positive, wrapping") {
        Angle const alpha(alpha_raw + pi_halves);
        Angle const beta(beta_raw + pi_halves);
        auto const expected_sum =
            alpha_raw + beta_raw - std::numbers::pi_v<TestType>;

        CHECK((alpha + beta).value() ==
              Approx(expected_sum).margin(margin_value));
    }
    SECTION("Both negative, no wrapping") {
        Angle const alpha(-alpha_raw);
        Angle const beta(-beta_raw);
        auto const expected_sum = -alpha_raw - beta_raw;

        CHECK((alpha + beta).value() ==
              Approx(expected_sum).margin(margin_value));
    }
    SECTION("Both negative, wrapping") {
        Angle const alpha(-alpha_raw - pi_halves);
        Angle const beta(-beta_raw - pi_halves);
        auto const expected_sum =
            std::numbers::pi_v<TestType> - alpha_raw - beta_raw;

        CHECK((alpha + beta).value() ==
              Approx(expected_sum).margin(margin_value));
    }
    SECTION("Different signs") {
        Angle const alpha(alpha_raw);
        Angle const beta(-beta_raw);
        auto const expected_sum = alpha_raw - beta_raw;

        CHECK((alpha + beta).value() ==
              Approx(expected_sum).margin(margin_value));
    }
}

TEMPLATE_TEST_CASE("operator-",
                   "[angle][operator]",
                   float,
                   double,
                   long double) {
    auto const alpha_raw = GENERATE(take(
        10, random(static_cast<TestType>(0.0), std::numbers::pi_v<TestType>)));
    auto const beta_raw = GENERATE(take(
        10, random(static_cast<TestType>(0.0), std::numbers::pi_v<TestType>)));

    constexpr auto margin_value = kFloatingPointEqualityThreshold<TestType>;

    SECTION("Both positive") {
        Angle const alpha(alpha_raw);
        Angle const beta(beta_raw);
        auto const expected_diff = alpha_raw - beta_raw;

        CHECK((alpha - beta).value() ==
              Approx(expected_diff).margin(margin_value));
    }
    SECTION("Both negative") {
        Angle const alpha(-alpha_raw);
        Angle const beta(-beta_raw);
        auto const expected_diff = beta_raw - alpha_raw;

        CHECK((alpha - beta).value() ==
              Approx(expected_diff).margin(margin_value));
    }
    SECTION("Different signs, not wrapping") {
        auto const alpha_halves_raw = alpha_raw * static_cast<TestType>(0.5);
        auto const beta_halves_raw = beta_raw * static_cast<TestType>(0.5);
        Angle const alpha(alpha_halves_raw);
        Angle const beta(-beta_halves_raw);
        auto const expected_diff = alpha_halves_raw + beta_halves_raw;

        CHECK((alpha - beta).value() ==
              Approx(expected_diff).margin(margin_value));
    }
    SECTION("Different signs, wrapping") {
        constexpr auto pi_halves =
            std::numbers::pi_v<TestType> / static_cast<TestType>(2.0);
        auto const alpha_halves_raw = alpha_raw * static_cast<TestType>(0.5);
        auto const beta_halves_raw = beta_raw * static_cast<TestType>(0.5);
        Angle const alpha(alpha_halves_raw + pi_halves);
        Angle const beta(-beta_halves_raw - pi_halves);
        auto const expected_diff =
            alpha_halves_raw + beta_halves_raw - std::numbers::pi_v<TestType>;

        CHECK((alpha - beta).value() ==
              Approx(expected_diff).margin(margin_value));
    }
}

TEMPLATE_TEST_CASE("AngleFromDegrees",
                   "[angle][free_method]",
                   float,
                   double,
                   long double) {
    auto const [offset_deg, offset_rad] =
        GENERATE(as<std::pair<TestType, TestType> >{},
                 std::make_pair(0.0, 0.0),
                 std::make_pair(1.0, 0.0174532925199432957692369),
                 std::make_pair(15.0, 0.261799387799149436538554),
                 std::make_pair(51.0, 0.890117918517108084231082),
                 std::make_pair(128.0, 2.23402144255274185846232),
                 std::make_pair(177.0, 3.08923277602996335115493),
                 std::make_pair(-101.0, -1.76278254451427287269293),
                 std::make_pair(-56.0, -0.97738438111682456307727),
                 std::make_pair(-9.0, -0.15707963267948966192313));

    int const num_full_turns = GENERATE(1, 5, 7, -1, -3, -6);
    Angle wrapped_angle = AngleFromDegrees(
        offset_deg + static_cast<TestType>(num_full_turns * 360));

    CHECK_THAT(wrapped_angle.value(),
               IsPeriodicApprox(
                   offset_rad,
                   -std::numbers::pi_v<TestType>,
                   std::numbers::pi_v<TestType> * static_cast<TestType>(2.0)));
}

TEMPLATE_TEST_CASE("DegreesFromAngle",
                   "[angle][free_method]",
                   float,
                   double,
                   long double) {
    auto const [offset_deg, offset_rad] =
        GENERATE(as<std::pair<TestType, TestType> >{},
                 std::make_pair(0.0, 0.0),
                 std::make_pair(1.0, 0.0174532925199432957692369),
                 std::make_pair(15.0, 0.261799387799149436538554),
                 std::make_pair(51.0, 0.890117918517108084231082),
                 std::make_pair(128.0, 2.23402144255274185846232),
                 std::make_pair(177.0, 3.08923277602996335115493),
                 std::make_pair(-101.0, -1.76278254451427287269293),
                 std::make_pair(-56.0, -0.97738438111682456307727),
                 std::make_pair(-9.0, -0.15707963267948966192313));

    Angle const alpha(offset_rad);

    CHECK_THAT(DegreesFromAngle(alpha),
               IsPeriodicApprox(offset_deg,
                                static_cast<TestType>(-180.0),
                                static_cast<TestType>(360.0)));
}

TEMPLATE_TEST_CASE("Distance",
                   "[angle][free_method]",
                   float,
                   double,
                   long double) {
    auto const starting_angle_value = GENERATE(take(
        10,
        random(-std::numbers::pi_v<TestType>, std::numbers::pi_v<TestType>)));
    auto const distance_value = GENERATE(take(
        10, random(static_cast<TestType>(0.0), std::numbers::pi_v<TestType>)));
    Angle alpha(starting_angle_value);
    Angle beta(starting_angle_value + distance_value);
    Angle gamma(starting_angle_value - distance_value);
    auto constexpr margin_value = kFloatingPointEqualityThreshold<TestType>;
    CHECK(distance_value == Approx(Distance(alpha, beta)).margin(margin_value));
    CHECK(distance_value == Approx(Distance(beta, alpha)).margin(margin_value));
    CHECK(distance_value ==
          Approx(Distance(alpha, gamma)).margin(margin_value));
    CHECK(distance_value ==
          Approx(Distance(gamma, alpha)).margin(margin_value));
}

TEMPLATE_TEST_CASE("OperateAsAngle",
                   "[angle][free_method]",
                   float,
                   double,
                   long double) {
    using AngleT = Angle<TestType>;

    auto constexpr margin_value = kFloatingPointEqualityThreshold<TestType>;

    SECTION("Addition, arguments by const ref") {
        auto add3 = [](AngleT const& alpha,
                       AngleT const& beta,
                       AngleT const& gamma) -> AngleT {
            return alpha + beta + gamma;
        };

        auto constexpr pi_thirds =
            std::numbers::pi_v<TestType> / static_cast<TestType>(3.0);

        auto const alpha =
            GENERATE(take(10, random(static_cast<TestType>(0.0), pi_thirds)));
        auto const beta =
            GENERATE(take(10, random(static_cast<TestType>(0.0), pi_thirds)));
        auto const gamma =
            GENERATE(take(10, random(static_cast<TestType>(0.0), pi_thirds)));

        CHECK(OperateAsAngle<TestType>(add3, alpha, beta, gamma) ==
              Approx(alpha + beta + gamma).margin(margin_value));
        CHECK(
            OperateAsAngle<TestType>(
                add3, alpha + pi_thirds, beta + pi_thirds, gamma + pi_thirds) ==
            Approx(alpha + beta + gamma - std::numbers::pi_v<TestType>)
                .margin(margin_value));
        CHECK(OperateAsAngle<TestType>(add3,
                                       alpha - std::numbers::pi_v<TestType>,
                                       beta - std::numbers::pi_v<TestType>,
                                       gamma) ==
              Approx(alpha + beta + gamma).margin(margin_value));
    }

    SECTION("Substraction, arguments by value") {
        auto substract = [](AngleT alpha, AngleT beta) -> AngleT {
            return alpha - beta;
        };

        auto constexpr pi_halves =
            static_cast<TestType>(0.5) * std::numbers::pi_v<TestType>;

        auto const alpha =
            GENERATE(take(10, random(static_cast<TestType>(0.0), pi_halves)));
        auto const beta =
            GENERATE(take(10, random(static_cast<TestType>(0.0), pi_halves)));

        CHECK(OperateAsAngle<TestType>(substract, alpha, beta) ==
              Approx(alpha - beta).margin(margin_value));
        CHECK(OperateAsAngle<TestType>(
                  substract, alpha + pi_halves, -beta - pi_halves) ==
              Approx(alpha + beta - std::numbers::pi_v<TestType>)
                  .margin(margin_value));
    }

    SECTION("Addition, arguments' type deduced") {
        auto add3 = [](auto alpha, auto beta, auto gamma) {
            return alpha + beta + gamma;
        };

        auto constexpr pi_thirds =
            std::numbers::pi_v<TestType> / static_cast<TestType>(3.0);

        auto const alpha =
            GENERATE(take(10, random(static_cast<TestType>(0.0), pi_thirds)));
        auto const beta =
            GENERATE(take(10, random(static_cast<TestType>(0.0), pi_thirds)));
        auto const gamma =
            GENERATE(take(10, random(static_cast<TestType>(0.0), pi_thirds)));

        CHECK(OperateAsAngle<TestType>(add3, alpha, beta, gamma) ==
              Approx(alpha + beta + gamma).margin(margin_value));
        CHECK(
            OperateAsAngle<TestType>(
                add3, alpha + pi_thirds, beta + pi_thirds, gamma + pi_thirds) ==
            Approx(alpha + beta + gamma - std::numbers::pi_v<TestType>)
                .margin(margin_value));
        CHECK(OperateAsAngle<TestType>(add3,
                                       alpha - std::numbers::pi_v<TestType>,
                                       beta - std::numbers::pi_v<TestType>,
                                       gamma) ==
              Approx(alpha + beta + gamma).margin(margin_value));
    }
}

}  // namespace pl
