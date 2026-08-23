#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct instruction{
    FILE *ins_pt;
    size_t length;
};

typedef struct instruction Instr;

void decode(Instr *I, uint8_t *ins);
