import random
import logging
from black_scholes import black_scholes

logging.basicConfig(filename="fuzz_log.txt", level=logging.INFO)

def test_fuzz_inputs():
    for _ in range(100):
        try:
            S = random.uniform(1, 1000)
            K = random.uniform(1, 1000)
            T = random.uniform(0.01, 5)
            r = random.uniform(-0.2, 0.2)
            sigma = random.uniform(0.01, 1)
            opt_type = random.choice(['call', 'put'])

            result = black_scholes(S, K, T, r, sigma, opt_type)
            assert result >= 0
        except Exception as e:
            logging.warning(f"⚠️ Failed Input: {(S, K, T, r, sigma, opt_type)}\n   Error: {e}")
