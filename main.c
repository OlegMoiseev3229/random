#include<stdio.h>
#include "oler_random.c"

int main() {
	oler_random_seed(42);
	for (int i = 0; i < 100000; i++) {
		printf("%f\n", oler_random_double());
	}
}
