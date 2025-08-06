from hypothesis import given, strategies as st
from black_scholes import black_scholes

@given(
    S=st.floats(min_value=1, max_value=1000),
    K=st.floats(min_value=1, max_value=1000),
    T=st.floats(min_value=0.01, max_value=5),
    r=st.floats(min_value=-0.1, max_value=0.3),
    sigma=st.floats(min_value=0.01, max_value=1.0),
    opt_type=st.sampled_from(['call', 'put'])
)
def test_option_value_non_negative(S, K, T, r, sigma, opt_type):
    price = black_scholes(S, K, T, r, sigma, opt_type)
    assert price >= 0
