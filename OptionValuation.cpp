#include <cmath>
#include <iostream>
#include <string>
#include <stdexcept>

// Portable approximation of CDF of standard normal distribution
double norm_cdf(double x) {
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

// Black-Scholes formula
double black_scholes(double S, double K, double T, double r, double sigma, const std::string& option_type) {
    if (T <= 0 || sigma <= 0 || S <= 0 || K <= 0) {
        throw std::invalid_argument("Invalid input: T, sigma, S, and K must be > 0");
    }

    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);

    if (option_type == "call") {
        return S * norm_cdf(d1) - K * std::exp(-r * T) * norm_cdf(d2);
    } else if (option_type == "put") {
        return K * std::exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
    } else {
        throw std::invalid_argument("Invalid option type. Use 'call' or 'put'");
    }
}

int main() {
    try {
        double S = 100.0;
        double K = 100.0;
        double T = 1.0;
        double r = 0.05;
        double sigma = 0.2;
        std::string option_type = "call";

        double price = black_scholes(S, K, T, r, sigma, option_type);
        std::cout << "Black-Scholes " << option_type << " option price: " << price << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
