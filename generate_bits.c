#include <stdio.h>
#include <stddef.h>
#include "oler_random.c"

void generate_signal(oler_XorShiftState* random_state, size_t len, uint8_t buffer[len]) {
	for (size_t bytes_written = 0; bytes_written < len;) {
		uint64_t x =  oler_xorshift_next(random_state);
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

int main() {
	uint8_t random_bytes[127] = {0};
	oler_XorShiftState random_state = {42ull};
	generate_signal(&random_state, 127, random_bytes);
	for (int i = 0; i < 127; i++) {
		printf("%x\n", random_bytes[i]);
	}
}
