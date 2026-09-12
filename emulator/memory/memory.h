#include <stdint.h>
#include <stdlib.h>

// NOTE: Echo RAM doesn't mean copy of RAM it means inaccessible DO NOT COPY just mask bit

struct Memory {
	uint8_t mem_array[65536];
};
