/*
 alu_model.h
 Programmer: George Mobus
 
 */

// Register ADT
typedef unsigned short Register;
typedef Register * Register_p;

#define LOW_ORDER_WORD_MASK 0x0000FFFF

Register_p createRegister (void);	// constructor
int getSignedValue (Register_p r);	// returns a signed value of what is in register r
void putUnsignedValue (Register_p r, int v);	// sets the value in register r to the unsigned value v

// Register file ADT
#define REGISTER_FILE_SIZE 8
#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7

typedef struct registerFile {
	Register r0,r1,r2,r3,r4,r5,r6,r7;
} RegisterFile;

typedef RegisterFile * RegisterFile_p;

RegisterFile_p createRegisterFile (void);	// constructor
Register getContentRegister (RegisterFile_p rf, int which);
void putContentRegister (RegisterFile_p rf, int which, int v);
int getContentLOB (RegisterFile_p rf, int which);	// returns LOB of register which
void putContentLOB (RegisterFile_p rf, int which, int v);	// puts an unsigned value from v into register which

// ALU ADT
// 0 ADD, 1 SUB, 2 MUL, 3 DIV, 4 AND, 5 OR, 6 NOT, 7 XOR, 8 SHL, 9 SHR
#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3
#define AND 4
#define OR  5
#define NOT 6
#define XOR 7
#define SHL 8
#define SHR 9

#define OVERFLOW_SET 0x1
#define NEGATIVE_SET 0x2
#define ZERO_SET 0x4
#define CARRY_SET 0x8

#define SIGN_BIT_TEST 0x8000


typedef struct alu {
	Register A, B, R;
	unsigned char flags;	// n, z, c, o - low order nybble
} ALU;

typedef ALU * ALU_p;

ALU_p createALU (void);
void clearALU (ALU_p);	// clears registers and flags
void performOperation (ALU_p, int);
void setALU_Flags (ALU_p, unsigned);
void add(ALU_p);
void sub(ALU_p);
void mul(ALU_p);
void alu_div(ALU_p);
void alu_and(ALU_p);
void alu_or(ALU_p);
void alu_not(ALU_p);
void alu_xor(ALU_p);
void alu_shl(ALU_p);
void alu_shr(ALU_p);
