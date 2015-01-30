#include <stdio.h>
#include <stdlib.h>
#include "alu_model.h"

/*
 alu_model.c
 
 Programmer: George Mobus
 Programmer: Nabil Sujipto, Nhat Tran
 Date: 1-22-2015
 Description:
 This program provides example code for creating register objects and a registe r file. These twoobjects
 can be instantiated either on the stack ("raw") or on the heap (i.e. constructed). Several methods (OO-style
 functions) are given to show how to write them in object-oriented style in C. Note that when an object is
 instantiated in the heap the first argument passed in a method is always the pointer to the specific object.
 That pointer is actually the 'this' reference in Java, where it is implied in Java it is explicit in C.
 The main() test driver also shows how data conversions from unsigned to signed work.
 */

//Register method
Register_p createRegister (void) {
	Register_p r = (Register_p) malloc (sizeof(Register));
	return r;
}

int getSignedValue (Register_p r) {
	return (signed short) *r;
}

void putUnsignedValue (Register_p r, int v) {
	*r = (unsigned short) v & 0x00FF; 
}

// RegisterFile Methods

RegisterFile_p createRegisterFile (void) {
	RegisterFile_p rf = (RegisterFile_p) malloc (sizeof (RegisterFile));
	return rf;		// actually a pointer to a register - the first one
}

void setRegisterValue (RegisterFile_p rf, int which, Register value) {
		switch (which) {
		case R0: {rf->r0 = value; break;}
		case R1: {rf->r1 = value; break;}
		case R2: {rf->r2 = value; break;}
		case R3: {rf->r3 = value; break;}
		case R4: {rf->r4 = value; break;}
		case R5: {rf->r5 = value; break;}
		case R6: {rf->r6 = value; break;}
		case R7: {rf->r7 = value; break;}
	}
}

void clearRegisterFile (RegisterFile_p rf) {
	int i;
	for (i=0; i<REGISTER_FILE_SIZE; i++) setRegisterValue(rf, i, (Register) 0x0);	// clear registers
}

Register getRegisterValue (RegisterFile_p rf, int which) {
	Register r;
	switch (which) {
		case R0: {r = rf->r0; break;}
		case R1: {r = rf->r1; break;}
		case R2: {r = rf->r2; break;}
		case R3: {r = rf->r3; break;}
		case R4: {r = rf->r4; break;}
		case R5: {r = rf->r5; break;}
		case R6: {r = rf->r6; break;}
		case R7: {r = rf->r7; break;}
	}
    return r;
}

void printRegisterFile (RegisterFile_p rf) {
	int i;
	for (i=0; i<REGISTER_FILE_SIZE; i++) printf("Register R%d content: %04X\n", i,
		getRegisterValue(rf, i));
}



// ALU methods

ALU_p createALU (void) {
	ALU_p alu = (ALU_p) malloc (sizeof(ALU));
	clearALU(alu);
	return alu;
}

void clearALU (ALU_p alu) {
	alu->A = 0x0;
	alu->B = 0x0;
	alu->R = 0x0;
	alu->flags = 0x0;
}

void setALU_Registers (ALU_p alu, Register opnd1, Register opnd2) {
	alu->A = opnd1;
	alu->B = opnd2;
	alu->R = 0x0;
}

void performOperation (ALU_p alu, int op) {
    
	switch (op) {
		case ADD: {add(alu); break;}
		case SUB: {sub(alu); break;}
		case MUL: {mul(alu); break;}
		case DIV: {alu_div(alu); break;}
		case AND: {alu_and(alu); break;}
		case OR: {alu_or(alu); break;}
		case NOT: {alu_not(alu); break;}
		case XOR: {alu_xor(alu); break;}
		case SHL: {alu_shl(alu); break;}
		case SHR: {alu_shr(alu); break;}
		default: return;	// set machine exception
	}
    
}

void setALU_Flags (ALU_p alu, unsigned result) {
	int sign_A, sign_B, sign_R;
	
	sign_A = alu->A & SIGN_BIT_TEST;
	sign_B = alu->B & SIGN_BIT_TEST;
	sign_R = alu->R & SIGN_BIT_TEST;
	
	if (result > 65535) alu->flags |= OVERFLOW_SET;			// set overflow flag
	else if (sign_R) alu->flags |= NEGATIVE_SET;		// set negative flag
	else if (alu->R == 0) alu->flags |= ZERO_SET;
	else if (result > 32767) alu->flags |= CARRY_SET;
    
}
/**
 *add(ALU_p alu)
 *add two registers A, B
 *return result to register R
 */
void add(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 + (unsigned) opnd2;	// add two integers to check overflow, etc.
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}
/**
 *sub(ALU_p alu)
 *subtract two registers A, B
 *return result to register R
 */

void sub(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 - (unsigned) opnd2;	// sub two integers to check overflow, etc.
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}
/**
 *mul(ALU_p alu)
 *multiple two registers A, B
 *return result to register R
 */

void mul(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 * (unsigned) opnd2;	// multiple two integers to check overflow, etc.
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

/**
 *alu_div(ALU_p alu)
 *divide two registers A, B
 *return result to register R
 */
void alu_div(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 / (unsigned) opnd2;	// divivde two integers to check overflow, etc.
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

/**
 *alu_and(ALU_p alu)
 *performs the following operation A and B
 *
 */
void alu_and(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 & (unsigned) opnd2;	// and operation two integers to check overflow, etc.
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

/**
* alu_or(ALU_p alu)
* performs the following operation alu->A | alu->B
* return the result
*/
void alu_or(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 | (unsigned) opnd2;
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

/**
* alu_onot(ALU_p alu)
* performs the following operation ~alu->A
* return the result
*/
void alu_not(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned result = ~(unsigned) opnd1;
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

/**
* alu_xor(ALU_p alu)
* performs the following operation alu->A ^ alu->B
* return the result
*/
void alu_xor(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 ^ (unsigned) opnd2;
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

/**
* alu_shl(ALU_p alu)
* performs the following operation alu->A <<< 16
* return the result
*/
void alu_shl(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned result = (unsigned) opnd1 << 16;
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

/**
* alu_shr(ALU_p alu)
* performs the following operation alu->A >>> 16
* return the result
*/
void alu_shr(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned result = (unsigned) opnd1 >> 16;
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}


int main () {
	int dest, op1, op2, sel;			// register addresses
	
	RegisterFile_p rf = createRegisterFile();

	clearRegisterFile(rf);
	
	
	ALU_p alu_p = createALU();
	
	int resp;
	printf("Enter destination register number: ");
	scanf("%d", &dest);
	printf("\nEnter operand1 register number: ");
	scanf("%d", &op1);
	printf("\nEnter value for operand 1 [up to 32768]: ");
	scanf("%d", &resp);
	setRegisterValue(rf, op1, (Register) resp);
	printf("\nEnter operand2 register number: ");
	scanf("%d", &op2);
	printf("\nEnter value for operand 2 [up to 32768]: ");
	scanf("%d", &resp);
	setRegisterValue(rf, op2, (Register) resp);
	printf("\nEnter the number of the operation\n");
	printf("0 ADD, 1 SUB, 2 MUL, 3 DIV, 4 AND, 5 OR, 6 NOT, 7 XOR, 8 SHL, 9 SHR: ");
	scanf("%d", &sel);
	printf("\n");
	alu_p->A = getRegisterValue(rf, op1);
	alu_p->B = getRegisterValue(rf, op2);
	performOperation (alu_p, sel);
    printf("Registers in ALU:\n");
	printf("Register A = 0x%04X\n", alu_p->A);
	printf("Register B = 0x%04X\n", alu_p->B);
	printf("Register R = 0x%04X\n", alu_p->R);
    printf("\n");
	setRegisterValue(rf, dest, alu_p->R);
	printRegisterFile(rf);

	getchar();
}

