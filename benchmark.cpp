#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

static int seed = 0;

int myrand() {
	// rand formula from FreeBSD rand.c
	seed = seed * 1103515245 + 12345;
	return seed % ((unsigned long)RAND_MAX + 1);
}

static inline uint64_t count_empty_old( uint64_t x ) {
    x |= (x >> 2) & 0x3333333333333333ULL;
    x |= (x >> 1);
    x = ~x & 0x1111111111111111ULL;
    // At this point each nibble is:
    //  0 if the original nibble was non-zero
    //  1 if the original nibble was zero
    // Next sum them all
    x += x >> 32;
    x += x >> 16;
    x += x >>  8;
    x += x >>  4; // this can overflow to the next nibble if there were 16 empty positions
    return x & 0xf;
}

static inline uint64_t count_empty_new( uint64_t x ) {
	x = ~x;
	x &= x >> 2;
	x &= x >> 1;
	x &= 0x1111111111111111ull;
	return __builtin_popcountll( x );
}

static inline uint64_t transpose_new( uint64_t x ) {
	uint64_t t = (x ^ (x >> 12)) & 0x0000f0f00000f0f0ull;
	x ^= t ^ (t << 12);
	t = (x ^ (x >> 24)) & 0x00000000ff00ff00ull;
	x ^= t ^ (t << 24);
	return x;
}

uint64_t (*funcs[])(uint64_t) = {
	count_empty_old,
	count_empty_new,
	transpose_new,
	NULL
};

int main(void) {

	for (int fi = 0; funcs[fi]; fi++) {
		uint64_t (*func)(uint64_t) = funcs[fi];

		seed = 0;
		uint64_t s = 0;
		double t0 = (double)clock() / (double)CLOCKS_PER_SEC;
		for( int i = 0; i < 100000; i++ ) {
			uint64_t b;
			for( int j = 0; j < 10001; j++ ) {
				b = ((uint64_t)myrand()) << 40 ^ ((uint64_t)myrand() << 16) ^ myrand();
				b = func(b);
				s += b;
			}
		}
		double t1 = (double)clock() / (double)CLOCKS_PER_SEC;

		printf("%d %lx %f seconds\n", fi, s, t1-t0);
	}
	return 0;
}

