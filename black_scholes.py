import math
from scipy.stats import norm

def black_scholes(S, K, T, r, sigma, option_type='call'):
    """
    Black-Scholes option pricing formula.
    S: Spot price
    K: Strike price
    T: Time to maturity (in years)
    r: Annual risk-free interest rate
    sigma: Annual volatility
    option_type: 'call' or 'put'
    """

    if T <= 0 or sigma <= 0 or S <= 0 or K <= 0:
        print("Invalid input parameters.")
        return float("nan")

    d1_numerator = math.log(S / K) + (r + 0.5 * sigma ** 2) * T
    d1_denominator = sigma * math.sqrt(T)
    d1 = d1_numerator / d1_denominator
    d2 = d1 - sigma * math.sqrt(T)

    print(f"Inputs -> S: {S}, K: {K}, T: {T}, r: {r}, sigma: {sigma}, type: {option_type}")
    print(f"Step 1: d1 numerator = ln(S/K) + (r + 0.5σ²)T = {d1_numerator:.6f}")
    print(f"Step 2: d1 denominator = σ√T = {d1_denominator:.6f}")
    print(f"Step 3: d1 = {d1:.6f}")
    print(f"Step 4: d2 = d1 - σ√T = {d2:.6f}")

    if option_type == 'call':
        call_price = S * norm.cdf(d1) - K * math.exp(-r * T) * norm.cdf(d2)
        print(f"Step 5: Call Price = S*N(d1) - K*e^(-rT)*N(d2) = {call_price:.6f}")
        return call_price
    elif option_type == 'put':
        put_price = K * math.exp(-r * T) * norm.cdf(-d2) - S * norm.cdf(-d1)
        print(f"Step 5: Put Price = K*e^(-rT)*N(-d2) - S*N(-d1) = {put_price:.6f}")
        return put_price
    else:
        raise ValueError("option_type must be 'call' or 'put'")

# Example usage:
if __name__ == "__main__":
    S = 100       # Spot price
    K = 100       # Strike price
    T = 1         # Time to maturity in years
    r = 0.05      # Risk-free interest rate
    sigma = 0.2   # Volatility

    option_type = 'call'  # Change to 'put' if needed
    price = black_scholes(S, K, T, r, sigma, option_type)
    print(f"\n➡️ Final {option_type.capitalize()} Option Price: {price:.4f}")
