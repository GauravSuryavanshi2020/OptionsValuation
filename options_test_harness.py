from black_scholes import black_scholes

def test_call_option_price():
    price = black_scholes(100, 100, 1, 0.05, 0.2, 'call')
    assert abs(price - 10.4506) < 1e-3

def test_put_option_price():
    price = black_scholes(100, 100, 1, 0.05, 0.2, 'put')
    assert abs(price - 5.5735) < 1e-3
