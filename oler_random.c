#include<stdint.h>
#include<assert.h>
#include<math.h>

// algorithm taken from Wikipedia Xorshift64 Xorshiro256**
#define oler_XORSHIFT_MAX_VALUE (~0ull)
typedef struct {
	uint64_t n;
} oler_XorShiftState;

typedef struct {
	uint64_t n[4];
} oler_XoshiroState;

uint64_t oler_xorshift_next(oler_XorShiftState* state) {
	uint64_t x = state->n;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	return state->n = x;
}

uint64_t oler_xoshiro_rol64(uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

uint64_t oler_xoshiro_next(oler_XoshiroState* state) {
	uint64_t *s = state->n;
	uint64_t result = oler_xoshiro_rol64(s[1] * 5, 7) * 9;
	uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = oler_xoshiro_rol64(s[3], 45);

	return result;
}

oler_XoshiroState global_random_state;

void oler_random_seed(uint64_t seed) {
	global_random_state.n[0] = seed;
	global_random_state.n[1] = seed + 0x3458FFADEE98ull;
	global_random_state.n[2] = ~(83192ull*seed + 0xF9765498ull);
	global_random_state.n[3] = 808346589364589360ull*seed + 7;
}

uint64_t oler_randuint(uint64_t a, uint64_t b) {
	assert(b > a);
	uint64_t x = oler_xoshiro_next(&global_random_state);
	x %= b-a;
	return a + x;
}

int64_t oler_randint(int64_t a, int64_t b) {
	assert(b > a);
	uint64_t x = oler_xoshiro_next(&global_random_state);
	int64_t x_signed = *(int64_t *)&x;
	x_signed %= b-a;
	return a + x_signed;
}

double oler_random_double() { // between 0. and 1.
	uint64_t x = oler_xoshiro_next(&global_random_state);
	return (x-1)/(double)oler_XORSHIFT_MAX_VALUE;
}

double oler_random_bernoulli(double p) {
	return (oler_random_double() < p) ? 1. : 0.;
}

double oler_random_pareto(double alpha, double x_min) { 
	double p = oler_random_double();
	return x_min * pow(1. - p, -1./alpha);
}

double oler_random_exponential(double lambda) { 
	double p = oler_random_double();
	return -((log(1 - p))/(lambda));
}

double oler_random_normal(double mu, double sigma) { 
	double u = oler_random_double();
	double v = oler_random_double();

	double x = sqrt(-2 * log(u)) * cos(2*M_PI * v);
	return mu + sigma*x;
}

void oler_random_bytes(size_t len, uint8_t buffer[len]) {
	for (size_t bytes_written = 0; bytes_written < len;) {
		uint64_t x =  oler_xoshiro_next(&global_random_state);
		switch (len - bytes_written) {
		case 3:
			buffer[bytes_written++] = x & 0xFF;
			x >>= 8;
		case 2:
			buffer[bytes_written++] = x & 0xFF;
			x >>= 8;
		case 1:
			buffer[bytes_written++] = x & 0xFF;
			x >>= 8;
			break;
		default:
			buffer[bytes_written++] = x & 0xFF;
			x >>= 8;
			buffer[bytes_written++] = x & 0xFF;
			x >>= 8;
			buffer[bytes_written++] = x & 0xFF;
			x >>= 8;
			buffer[bytes_written++] = x & 0xFF;
			x >>= 8;
		}
	}
}

