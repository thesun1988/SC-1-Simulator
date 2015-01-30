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

// Register methods
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
	return rf;
}

Register getContentRegister (RegisterFile_p rf, int which) {
	Register r = *rf[which];
	return r;
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
		case NOT:
		case XOR:
		case SHL:
		case SHR:
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

void add(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 + (unsigned) opnd2;	// add two integers to check overflow, etc.
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void sub(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 - (unsigned) opnd2;	// sub two integers to check overflow, etc.
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

void mul(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 * (unsigned) opnd2;	// multiple two integers to check overflow, etc.
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}
void alu_div(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 / (unsigned) opnd2;	// divivde two integers to check overflow, etc.
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}
void alu_and(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 & (unsigned) opnd2;	// and operation two integers to check overflow, etc.
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

<<<<<<< HEAD
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
void alu_not(ALU_p) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = ~(unsigned) opnd1;
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

/**
* alu_xor(ALU_p alu)
* performs the following operation alu->A ^ alu->B
* return the result
*/
void alu_xor(ALU_p) {
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
void alu_shl(ALU_P) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned result = (unsigned) opnd1 <<< 16;
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}

/**
* alu_shr(ALU_p alu)
* performs the following operation alu->A >>> 16
* return the result
*/
void alu_shr(ALU_p) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned result = (unsigned) opnd1 >>> 16;
=======
void alu_or(ALU_p alu) {
	unsigned opnd1 = (unsigned) alu->A;
	unsigned opnd2 = (unsigned) alu->B;
	unsigned result = (unsigned) opnd1 | (unsigned) opnd2;	// and operation two integers to check overflow, etc.
>>>>>>> origin/master
	setALU_Flags(alu, result);
	alu->R = result & LOW_ORDER_WORD_MASK;
}


int main () {
	int dest, op1, op2, sel;			// register addresses
	
	RegisterFile_p rf = createRegisterFile();
	int i;
	for (i=0; i<REGISTER_FILE_SIZE; i++) *rf[i] = 0x0;	// clear registers
	ALU_p alu_p = createALU();
	
	printf("Enter destination register number: ");
	scanf("%d", &dest);
	printf("\nEnter operand1 register number: ");
	scanf("%d", &op1);
	printf("\nEnter value for operand 1 [up to 32768]: ");
	scanf("%d", rf[op1]);
	printf("\nEnter operand2 register number: ");
	scanf("%d", &op2);
	printf("\nEnter value for operand 2 [up to 32768]: ");
	scanf("%d", rf[op2]);
	printf("Enter the number of the operation\n");
	printf("0 ADD, 1 SUB, 2 MUL, 3 DIV, 4 AND, 5 OR, 6 NOT, 7 XOR, 8 SHL, 9 SHR: "); //picks operation
	scanf("%d", &sel);
	printf("\n");
	alu_p->A = *rf[op1];
	//printf("Register R%d contains value: 0x%04X\n", op1, getContentRegister (rf, op1));
	alu_p->B = *rf[op2];
	performOperation (alu_p, sel);
	printf("Register A = 0x%04X\n", alu_p->A);
	printf("Register B = 0x%04X\n", alu_p->B);
	printf("Register R = 0x%04X\n", alu_p->R);
	getchar();
}

