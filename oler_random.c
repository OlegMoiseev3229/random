#include<stdint.h>

// algorithm taken from Wikipedia, Xorshift64
#define oler_XORSHIFT_MAX_VALUE (~0ull)
typedef struct {
	uint64_t n;
} oler_XorShiftState;

uint64_t oler_xorshift_next(oler_XorShiftState* state) {
	uint64_t x = state->n;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	return state->n = x;
}

double oler_xorshift_next_double(oler_XorShiftState* state) {
	uint64_t x = oler_xorshift_next(state);
	return (x-1)/oler_XORSHIFT_MAX_VALUE;
}
