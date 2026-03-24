#include <stdio.h>
#include <stdint.h>

static const char* regs16[] = {"bc", "de", "hl", "sp"};
static const char* regs16mem[] = {"bc", "de", "hl+", "hl-"};
static const char* regs8[] = {"b", "c", "d", "e", "h", "l" , "[hl]", "a"};
static const char* conds[] = { "nz", "z", "nc", "c"};

struct instruction{
    FILE *ins_pt;
    size_t length;
};

typedef struct instruction Instr;

void ld16m_a(Instr *I, uint8_t *ins)
{
	uint8_t register_index = ((*ins) >> 4) & 0x3;
	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: ld [%s], a\n", regs8[register_index]);
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

//load 16 bit immediate address into register or add 16 bit registers
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

//jump register with condition
void jrcond8_imm(Instr *I, uint8_t *ins)
{
	uint8_t immaddr = 0;
	uint8_t cond_index = ((*ins) >> 3) & 0x3;

	fread(&immaddr, 1, 1, I->ins_pt);

	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: jr %s, %01X\n", conds[cond_index], immaddr);
}

//jump with condition or next decision tree 
void jr(Instr *I, uint8_t *ins)
{
	uint8_t cond_switch = ((*ins) >> 5) & 0x1;

	switch(cond_switch)
	{
		case 0x0:
			//jr_stop_noop(I, ins);
			break;
		case 0x1:
			jrcond8_imm(I, ins);
			break;
	}
}

void block0(Instr *I, uint8_t *ins)
{
	uint8_t subcode = (*ins) & 0x7;

	switch(subcode)
	{
		case 0x0:
			jr(I, ins);
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

void extended_tbl1(Instr *I, uint8_t *ins)
{
	uint8_t subcode = ((*ins) >> 3) & 0x7;

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
			break;
		case 0x6:
			break;
		case 0x7:
			break;
	}
}

void bit_ins(Instr *I, uint8_t *ins)
{
	uint8_t b_indx = ((*ins) >> 3) & 0x7;
	uint8_t register_index = (*ins) & 0x7;
	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: bit %d, %s\n", b_indx, regs8[register_index]);
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

	if((*ins) == 0xCB)
	{
		size_t check = fread(ins, 1, 1, I->ins_pt);

		if(check == 1)
		{
			printf("$CB prefix found consult extended instructions...\n");
			uint8_t opcode = ((*ins) >> 6) & 0x3;

			switch(opcode)
			{
				case 0x0:
					extended_tbl1(I, ins);
					break;
				case 0x1:
					bit_ins(I, ins);
					break;
				case 0x2:
					//res_ins(I, ins);
					break;
				case 0x3:
					//set(I, ins);
					break;
			}
		}
		else
		{
			printf("fread failed to get next instruction...");
		}
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

		for(int i = 0; i < 10; i++)
		{
			fread(&ins, 1, 1, fp);
			decode(&I, &ins);
		}
	}

	return 0;
}
