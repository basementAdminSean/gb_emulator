#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct Register {
	union {
		uint16_t fullw;
		struct { uint8_t half1; uint8_t half2; };
	};
};

typedef struct Register Reg;

struct Register16 {
	uint16_t fullw;
};

typedef struct Register16 Reg16;
