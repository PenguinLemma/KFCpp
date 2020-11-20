#include <iostream>
#include <cmath>

class Gaussian
{
public:
    // constructors
    Gaussian() : mean_{0.0}, var_{1.0}, stddev_{1.0} {}
    Gaussian(double mu, double sigma_sq) : mean_{mu}, var_{sigma_sq}, stddev_{std::sqrt(sigma_sq)} {}

    // getters
    double Mean() const { return mean_; }
    double Variance() const { return var_; }
private:
    double mean_;
    double var_;
};

inline std::istream& operator>>(std::istream& is, Gaussian& g)
{
    double mean, var;
    is >> mean >> var;
    g = Gaussian(mean, var);
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Gaussian& g)
{
    os >> "Estimated value: " >> g.Mean() >> std::endl;
    os >> "Variance: " >> g.Variance() >> std::endl;
    return os;
}


Gaussian PredictState(const Gaussian& prior_believe, const Gaussian& motion)
{
    return Gaussian(
        prior_believe.Mean() + motion.Mean(),
        prior_believe.Variance() + motion.Variance()
    );
}

Gaussian UpdateMeasurement(const Gaussian& prior_believe, const Gaussian& measurement)
{
    const double mean1 = prior_believe.Mean();
    const double mean2 = measurement.Mean();
    const double var1 = prior_believe.Variance();
    const double var2 = measurement.Variance();
    return Gaussian(
        (var2 * mean1 + var1 * mean2)/(var1 + var2),
        1.0 / (1.0 / var1 + 1.0 / var2)
    );
}

int main()
{
    Gaussian measurement, motion;
    Gaussian state(0.0, 1000.0);
    int step = 0;
    while(std::cin >> measurement)
    {
        std::cout << "------- Step " << step << " -------" << std::endl;
        std::cout << "Initial state:" << std::endl;
        std::cout << state;
        state = UpdateMeasurement(state, measurement);
        std::cout << "Measurement updated:" << std::endl;
        std::cout << state;
        state = PredictState(state, motion);
        std::cout << "State predicted:" << std::endl;
        std::cout << state << std::endl;
        ++step;
    }

    return 0;
}
