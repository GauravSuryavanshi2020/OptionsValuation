// OptionValuationTests.cpp
#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <algorithm>
#include <random>

const double PI = 3.14159265358979323846;

// Normal CDF
inline double norm_cdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2));
}

// Black-Scholes Formula
double black_scholes(double S, double K, double T, double r, double sigma, char option_type) {
    if (T <= 0 || sigma <= 0) throw std::invalid_argument("Invalid T or sigma");
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    if (option_type == 'C') {
        return S * norm_cdf(d1) - K * std::exp(-r * T) * norm_cdf(d2);
    } else if (option_type == 'P') {
        return K * std::exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
    } else {
        throw std::invalid_argument("Invalid option type");
    }
}

// Property-based tests
void property_tests() {
    double K = 100, T = 1, r = 0.05, sigma = 0.2;
    for (double S = 50; S <= 150; S += 10) {
        double call1 = black_scholes(S, K, T, r, sigma, 'C');
        double call2 = black_scholes(S + 1, K, T, r, sigma, 'C');
        assert(call2 >= call1); // Monotonicity with S
    }

    for (double Tval = 0.01; Tval < 1; Tval += 0.1) {
        double price = black_scholes(100, 100, Tval, r, sigma, 'C');
        assert(price >= 0);
    }

    std::cout << "[PASS] Property-based tests passed.\n";
}

// Test case struct
struct TestCase {
    double S, K, T, r, sigma;
    char type;
    double diversity_score = 0;
};

TestCase compute_average(const std::vector<TestCase>& tests) {
    TestCase avg = {0, 0, 0, 0, 0, 'C'};
    for (const auto& t : tests) {
        avg.S += t.S;
        avg.K += t.K;
        avg.T += t.T;
        avg.r += t.r;
        avg.sigma += t.sigma;
    }
    int n = tests.size();
    avg.S /= n; avg.K /= n; avg.T /= n; avg.r /= n; avg.sigma /= n;
    return avg;
}

double score(const TestCase& t, const TestCase& avg) {
    return fabs(t.S - avg.S) + fabs(t.K - avg.K) + fabs(t.T - avg.T) + fabs(t.r - avg.r) + fabs(t.sigma - avg.sigma);
}

std::vector<TestCase> select_diverse_tests(std::vector<TestCase>& tests, int N) {
    TestCase avg = compute_average(tests);
    for (auto& t : tests) {
        t.diversity_score = score(t, avg);
    }
    std::sort(tests.begin(), tests.end(), [](const TestCase& a, const TestCase& b) {
        return a.diversity_score > b.diversity_score;
    });
    return std::vector<TestCase>(tests.begin(), tests.begin() + std::min(N, (int)tests.size()));
}

void test_diverse_set() {
    std::vector<TestCase> cases = {
        {90, 100, 1, 0.03, 0.2, 'C'}, {110, 100, 0.5, 0.05, 0.25, 'P'},
        {120, 100, 1.5, 0.02, 0.3, 'C'}, {80, 100, 0.75, 0.01, 0.15, 'P'},
        {100, 100, 1, 0.05, 0.2, 'C'}, {95, 100, 2, 0.04, 0.3, 'P'}
    };
    auto selected = select_diverse_tests(cases, 3);
    for (const auto& c : selected) {
        try {
            double result = black_scholes(c.S, c.K, c.T, c.r, c.sigma, c.type);
            std::cout << "Test Case [" << c.S << ", " << c.K << ", " << c.T << ", " << c.r << ", " << c.sigma << "]: " << result << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    std::cout << "[PASS] Diverse test cases executed.\n";
}

void randomized_tests(int count = 100) {
    std::default_random_engine gen;
    std::uniform_real_distribution<double> dist_S(50, 150);
    std::uniform_real_distribution<double> dist_K(50, 150);
    std::uniform_real_distribution<double> dist_T(0.01, 2);
    std::uniform_real_distribution<double> dist_r(0.0, 0.1);
    std::uniform_real_distribution<double> dist_sigma(0.1, 0.5);
    std::uniform_int_distribution<int> dist_type(0, 1);

    for (int i = 0; i < count; ++i) {
        double S = dist_S(gen);
        double K = dist_K(gen);
        double T = dist_T(gen);
        double r = dist_r(gen);
        double sigma = dist_sigma(gen);
        char type = dist_type(gen) == 0 ? 'C' : 'P';

        try {
            double price = black_scholes(S, K, T, r, sigma, type);
            assert(price >= 0);
        } catch (...) {
            std::cerr << "[FAIL] Error with input: (" << S << ", " << K << ", " << T << ", " << r << ", " << sigma << ", " << type << ")\n";
        }
    }
    std::cout << "[PASS] Randomized tests executed.\n";
}

int main() {
    property_tests();
    test_diverse_set();
    randomized_tests();
    return 0;
}
