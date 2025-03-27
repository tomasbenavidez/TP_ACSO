#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"

// Declaración de la función ADDis
void ADDis(uint32_t Rd, uint32_t Rn, uint32_t imm12, uint32_t shift);

// Declaración de la función ADD
void ADD(uint32_t Rd, uint32_t Rn, uint32_t Rm);        

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. 
     * */

    // Fetch instruction
    uint32_t instr = mem_read_32(NEXT_STATE.PC);
    printf("Fetched instruction: 0x%X\n", instr);

    // Decode instruction
    uint32_t opcode = (instr >> 21) & 0x7FF;

    //CHEQUEAR SI ESTAN BIEN LAS MASCARAS
    uint32_t Rd = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    uint32_t imm12 = (instr >> 10) & 0xFFF;
    uint32_t shift = (instr >> 22) & 0x1;

    printf("Decoded instruction - opcode: 0x%X, Rd: %d, Rn: %d, imm12: %d, shift: %d\n", opcode, Rd, Rn, imm12, shift);

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

    switch (opcode) {
        case 0x6A2: // HLT
            RUN_BIT = 0;
            return;
        case 0x558: // ADD Xd, Xn, Xm (Extended register)
            ADD(Rd, Rn, imm12);
            printf(" ADD X%d, X%d, X%d (Extended register)\n" , Rd, Rn, imm12);
            return;
        case 0x588: // ADDS Xd, Xn, #imm12 (Extended immediate)
            printf(" ADD immediate\n");
            ADDis(Rd, Rn, imm12, shift);
            return;
        case 0x758: // SUB Xd, Xn, Xm (Extended register)
            printf(" SUB Xd, Xn, Xm (Extended register)\n");
            return;
        case 0x788: // SUBS Xd, Xn, #imm12 (Extended immediate)
            printf(" SUBS Xd, Xn, #imm12 (Extended immediate)\n");
            return;
        case 0x750: // ANDS Xd, Xn, #imm12 (Extended immediate)
            printf(" ANDS Xd, Xn, #imm12 (Extended immediate)\n");
            return;
        case 0x650: // EOR Xd, Xn, #imm12 (Extended immediate)
            printf(" EOR Xd, Xn, #imm12 (Extended immediate)\n");
            return;
        //implementar caso donde no haya mas instruccionesyy
        
        default:
            printf("Instruction not implemented: 0x%X\n", opcode);
            RUN_BIT = 0;
            break;
    }
}

// Implementar la función ADDis
void ADDis(uint32_t Rd, uint32_t Rn, uint32_t imm12, uint32_t shift) {
    if (shift == 1) {
        imm12 = imm12 << 12;
    }
    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t result = op1 + (int64_t)imm12;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("ADD X%d, X%d, #%d\n", Rd, Rn, imm12);
    // printf("X%d = %d\n", Rd, result);
}

void ADD(uint32_t Rd, uint32_t Rn, uint32_t Rm) {
    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t op2 = CURRENT_STATE.REGS[Rm];
    int64_t result = op1 + op2;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("ADD X%d, X%d, X%d\n", Rd, Rn, Rm);
}