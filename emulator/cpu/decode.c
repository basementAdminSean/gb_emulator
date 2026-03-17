#include <stdio.h>
#include <stdint.h>

static const char* regs16[] = {"bc", "de", "hl", "sp"};
static const char* regs16mem[] = {"bc", "de", "hl+", "hl-"};
static const char* regs8[] = {"b", "c", "d", "e", "h", "l" , "[hl]", "a"};

struct instruction{
    FILE *ins_pt;
    size_t length;
};

typedef struct instruction Instr;

void ld16m_a(Instr *I, uint8_t *ins)
{
	uint8_t register_index = ((*ins) >> 4) & 0x3;
	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: ld [%s], a\n", regs16mem[register_index]);
}

void ld16_imm(Instr *I, uint8_t *ins)
{
	uint16_t immaddr = 0;
	uint8_t register_index = ((*ins) >> 4) & 0x3;
	fread(&immaddr, I->length - 1, 1, I->ins_pt);
	printf("Instruction Binary: %08b\nImmediate Address: %04X\n", *ins, immaddr);
	printf("Assembly Conversion: ld %s, %04X\n", regs16[register_index], immaddr);
}

void bit8_xor(Instr *I, uint8_t *ins)
{
	uint8_t register_index = (*ins) & 0x7;
	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: xor %s\n", regs8[register_index]);
}

void bit16_la(Instr *I, uint8_t *ins)
{
	uint8_t switch_bit = ((*ins) >> 3) & 0x1;

	switch(switch_bit)
	{
		case 0x0:
			I->length = 3;
			ld16_imm(I, ins);			
			break;
		case 0x1:
			break;
	}
}


void block0(Instr *I, uint8_t *ins)
{
	uint8_t subcode = (*ins) & 0x7;

	switch(subcode)
	{
		case 0x0:
			break;
		case 0x1:
			bit16_la(I, ins);
			break;
		case 0x2:
			ld16m_a(I, ins);
			break;
		case 0x3:
			break;
		case 0x4:
			break;
		case 0x5:
			break;
		case 0x6:
			break;
		case 0x7:
			break;
	}
}

void block2(Instr *I, uint8_t *ins)
{
	uint8_t subcode = ((*ins) >> 3) & 0x7;

	I->length = 1;

	switch(subcode)
	{
		case 0x0:
			break;
		case 0x1:
			break;
		case 0x2:
			break;
		case 0x3:
			break;
		case 0x4:
			break;
		case 0x5:
			bit8_xor(I, ins);
			break;
		case 0x6:
			break;
		case 0x7:
			break;
	}
}

void decode(Instr *I, uint8_t *ins)
{
	//extract the opcode from the instruction
	uint8_t opcode = ((*ins) >> 6) & 0x3;

	switch(opcode)
	{
		case 0x0:
			block0(I, ins);
			break;
		case 0x1:
			break;
		case 0x2:
			block2(I, ins);
			break;
		case 0x3:
			break;
	}

	if((*ins) = 0xCB)
	{
		// TODO: Implement function extended for $CB instruction bit 7, h
		//extended_tbl(I, ins);
	}

}


int main()
{
	Instr I;
	uint8_t ins;

	FILE *fp = fopen("../../ROM/mgb_boot.bin", "rb");

	I.ins_pt = fp;

	if(!fp)
	{
		printf("File operation unsuccessfull");
	}
	else
	{
		printf("File opened...\n");

		for(int i = 0; i < 4; i++)
		{
			fread(&ins, 1, 1, fp);
			decode(&I, &ins);
		}
		
		fread(&ins, 1, 1, fp);
		decode(&I, &ins);
	}

	return 0;
}
