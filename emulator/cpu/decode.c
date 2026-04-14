#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static const char* regs16[] = {"bc", "de", "hl", "sp"};
static const char* regs16mem[] = {"bc", "de", "hl+", "hl-"};
static const char* regs8[] = {"b", "c", "d", "e", "h", "l" , "[hl]", "a"};
static const char* conds[] = { "nz", "z", "nc", "c"};

struct instruction{
    FILE *ins_pt;
    size_t length;
};

typedef struct instruction Instr;

void ld8_imm(Instr *I, uint8_t *ins)
{
	uint8_t immaddr = 0;
	uint8_t register_index = ((*ins) >> 3) & 0x7;
	fread(&immaddr, I->length - 1, 1, I->ins_pt);
	printf("Instruction Binary: %08b\nImmediate Address: %02X\n", *ins, immaddr);
	printf("Assembly Conversion: ld %s, %02X\n", regs8[register_index], immaddr);
}

void ldhimm8_a(Instr *I, uint8_t *ins)
{
	uint8_t immaddr = 0;
	fread(&immaddr, I->length - 1, 1, I->ins_pt);
	printf("Instruction Binary: %08b\nImmediate Address: %02X\n", *ins, immaddr);
	printf("Assembly Conversion: ldh %02X, a\n", immaddr);
}

void ldr8_r8(Instr *I, uint8_t *ins)
{
	uint8_t immaddr = 0;
	uint8_t register_index1 = ((*ins) >> 3) & 0x7;
	uint8_t register_index2 = (*ins) & 0x7;
	printf("Assembly Conversion: ld %s, %s\n", regs8[register_index1], regs8[register_index2]);
}

void ld16m_a(Instr *I, uint8_t *ins)
{
	uint8_t register_index = ((*ins) >> 4) & 0x3;
	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: ld [%s], a\n", regs16mem[register_index]);
}

void ld16a_m(Instr *I, uint8_t *ins)
{
	uint8_t register_index = ((*ins) >> 4) & 0x3;
	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: ld a, [%s]\n", regs16mem[register_index]);
}

void ld16_imm(Instr *I, uint8_t *ins)
{
	uint16_t immaddr = 0;
	uint8_t register_index = ((*ins) >> 4) & 0x3;
	fread(&immaddr, I->length - 1, 1, I->ins_pt);
	printf("Instruction Binary: %08b\nImmediate Address: %04X\n", *ins, immaddr);
	printf("Assembly Conversion: ld %s, %04X\n", regs16[register_index], immaddr);
}

void call(Instr *I, uint8_t *ins)
{
	uint16_t immaddr = 0;
	fread(&immaddr, I->length - 1, 1, I->ins_pt);
	printf("Instruction Binary: %08b\nImmediate Address: %04X\n", *ins, immaddr);
	printf("Assembly Conversion: call %04X\n", immaddr);
}

void ldh_a(Instr *I, uint8_t *ins)
{
	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: ld [c], a\n");
}

void bit8_xor(Instr *I, uint8_t *ins)
{
	uint8_t register_index = (*ins) & 0x7;
	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: xor %s\n", regs8[register_index]);
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

void inc_r8(Instr *I, uint8_t *ins)
{
	uint8_t reg_index = ((*ins) >> 3) & 0x7;

	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: inc %s\n", regs8[reg_index]);
}

void bit_ins(Instr *I, uint8_t *ins)
{
	uint8_t b_indx = ((*ins) >> 3) & 0x7;
	uint8_t register_index = (*ins) & 0x7;
	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: bit %d, %s\n", b_indx, regs8[register_index]);
}

void inc16(Instr *I, uint8_t *ins)
{
	uint8_t register_index = ((*ins) >> 4) & 0x3;
	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: inc %s\n", regs16mem[register_index]);
}

void dec16(Instr *I, uint8_t *ins)
{
	uint8_t register_index = ((*ins) >> 4) & 0x3;
	printf("Instruction Binary: %08b\n", *ins);
	printf("Assembly Conversion: dec %s\n", regs16mem[register_index]);
}

void three_switch(Instr *I, uint8_t *ins)
{
	uint8_t direction = ((*ins) >> 3) & 0x1;

	switch(direction)
	{
		case 0x0:
			ld16m_a(I, ins);
			break;
		case 0x1:
			ld16a_m(I, ins);
			break;
	}
}

void inc_dec(Instr *I, uint8_t *ins)
{
	uint8_t direction = ((*ins) >> 3) & 0x1;

	switch(direction)
	{
		case 0x0:
			inc16(I, ins);
			break;
		case 0x1:
			dec16(I, ins);
			break;
	}
}

void call_stack(Instr *I, uint8_t *ins)
{
	uint8_t direction = ((*ins) >> 3) & 0x1;

	switch(direction)
	{
		case 0x0:
			//ld16m_a(I, ins);
			break;
		case 0x1:
			I->length = 3;
			call(I, ins);
			break;
	}
}

void ld3_switch(Instr *I, uint8_t *ins)
{
	uint8_t direction = ((*ins) >> 4) & 0x1;

	switch(direction)
	{
		case 0x0:
			ldh_a(I, ins);
			break;
		case 0x1:
			break;
	}
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

void block3_imm8(Instr *I, uint8_t *ins)
{
	if(!(((*ins) >> 5) & 0x1))
	{
		I->length = 1;
		//ret(I, ins);
		return;
	}
	else
	{
		I->length = 2;
		uint8_t two_switch = ((*ins) >> 3) & 0x3;

		switch(two_switch)
		{
			case 0x0:
				//ldh [imm8], a
				ldhimm8_a(I, ins);
				break;
			case 0x1:
				//add sp, imm8
				break;
			case 0x2:
				//ldh a, [imm8]
				break;
			case 0x3:
				//ld hl, sp + imm8
				break;
		}
	}
}

void bit6_switch(Instr *I, uint8_t *ins)
{
	uint8_t six_switch = ((*ins) >> 5) & 0x1;

	switch (six_switch)
	{
		case 0x0:
			//jp cond, imm16
			break;
		case 0x1:
			ld3_switch(I, ins);
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
			three_switch(I, ins);
			break;
		case 0x3:
			inc_dec(I, ins);
			break;
		case 0x4:
			inc_r8(I, ins);
			break;
		case 0x5:
			break;
		case 0x6:
			I->length = 2;
			ld8_imm(I, ins);
			break;
		case 0x7:
			break;
	}
}

void block1(Instr *I, uint8_t *ins)
{
	switch(*ins)
	{
		case 0x76:
			printf("HALT!!\n");
			break;
		default:
			ldr8_r8(I, ins);
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

void block3(Instr *I, uint8_t *ins)
{
	uint8_t subcode = (*ins) & 0x7;

	switch(subcode)
	{
		case 0x0:
			block3_imm8(I, ins);
			break;
		case 0x1:
			break;
		case 0x2:
			bit6_switch(I, ins);
			break;
		case 0x3:
			break;
		case 0x4:
			break;
		case 0x5:
			call_stack(I, ins);
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
			block1(I, ins);
			break;
		case 0x2:
			block2(I, ins);
			break;
		case 0x3:
			block3(I, ins);
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


int main(int argc, char *argv[])
{
	Instr I;
	uint8_t ins;

	switch(argc)
	{
		case 1:
			printf("Program syntax: ./decode n \n\nn - amount of instructions to decode\n");
			return 0;
			break;
		case 2:
			break;
		default:
			printf("Please specify how many instructions to read as the only argument...\n");
			return 0;
			break;
	}


	FILE *fp = fopen("../../ROM/mgb_boot.bin", "rb");

	I.ins_pt = fp;

	if(!fp)
	{
		printf("File operation unsuccessfull");
	}
	else
	{
		printf("File opened...\n");
		
		for(int i = 0; i < atoi(argv[1]); i++)
		{
			fread(&ins, 1, 1, fp);
			printf("Instruction Hex: %02X\n", ins);
			decode(&I, &ins);
		}
		
	}

	return 0;
}
