from black_scholes import black_scholes

def test_selected_cases():
    test_cases = [
        (50, 50, 0.5, 0.01, 0.1, 'call'),   # low volatility
        (200, 100, 2, 0.05, 0.3, 'call'),   # deep in-the-money
        (100, 100, 1, 0.05, 0.2, 'put'),    # at-the-money
    ]
    for S, K, T, r, sigma, opt_type in test_cases:
        price = black_scholes(S, K, T, r, sigma, opt_type)
        assert price >= 0
