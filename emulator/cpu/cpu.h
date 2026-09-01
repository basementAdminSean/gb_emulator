#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct Register {
	union {
		uint16_t fullw;
		struct { uint8_t low; uint8_t high; };
	};
};

typedef struct Register Reg;

struct Register16 {
	uint16_t fullw;
};

typedef struct Register16 Reg16;

struct CPU {
	Reg Reg_AF; //A -> high  |  F -> low
	Reg Reg_BC; //B -> high  |  C -> low
	Reg Reg_DE; //D -> high  |  E -> low
	Reg Reg_HL; //H -> high  |  L -> low
	Reg16 Reg_SP;
	Reg16 Reg_PC;
};

typedef struct CPU CPU_inst;
