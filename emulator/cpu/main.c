#include <stdio.h>
#include "cpu.h"

int main()
{
	Reg regbc;

	regbc.fullw = 0x5678;

	printf("16 bit register bc: %04X\n 8 bit register b: %02X\n 8 bit register c: %02X\n", 
		regbc.fullw, 
		regbc.half1, 
		regbc.half2);

	return 0;
}
