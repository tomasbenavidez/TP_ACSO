#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"

void ADDis(uint32_t Rd, uint32_t Rn, uint32_t imm12, uint32_t shift);
void ADD(uint32_t Rd, uint32_t Rn, uint32_t Rm);        
void SUB(uint32_t Rd, uint32_t Rn, uint32_t Rm);
void SUBis(uint32_t Rd, uint32_t Rn, uint32_t imm12, uint32_t shift);
// void ANDis(uint32_t Rd, uint32_t Rn, uint32_t imm12, uint32_t shift);
// void EORis(uint32_t Rd, uint32_t Rn, uint32_t imm12, uint32_t shift);


void execute_add(uint32_t instr);
void execute_adds(uint32_t instr);
void execute_sub(uint32_t instr);
void execute_subs(uint32_t instr);
void execute_and(uint32_t instr);
void execute_ands(uint32_t instr);
void execute_eor(uint32_t instr);
void execute_orr(uint32_t instr);
void execute_mul(uint32_t instr);
void execute_addi(uint32_t instr);
void execute_addis(uint32_t instr);
void execute_subi(uint32_t instr);
void execute_subis(uint32_t instr);
void execute_ldur(uint32_t instr);
void execute_stur(uint32_t instr);
void execute_sturb(uint32_t instr);
void execute_sturh(uint32_t instr);
void execute_ldurb(uint32_t instr);
void execute_ldurh(uint32_t instr);
void execute_b(uint32_t instr);
void execute_br(uint32_t instr);
void execute_bcond(uint32_t instr);
void execute_movz(uint32_t instr);
void execute_cbz(uint32_t instr);
void execute_cbnz(uint32_t instr);
void execute_hlt(uint32_t instr);
void execute_lsl_lsr(uint32_t instr);



void decode_instruction(uint32_t instr) {

    uint32_t op26 = (instr >> 26) & 0x3F;     // bits [31:26]
    uint32_t op24 = (instr >> 24) & 0xFF;     // bits [31:24]
    uint32_t op21 = (instr >> 21) & 0x7FF;    // bits [31:21]
    uint32_t op10 = (instr >> 10) & 0x3FFFFF; // bits [31:10]
    uint32_t op  = (instr >> 22) & 0x3FF;     // bits [31:22] (p/ instrucciones I)
    
    switch (op21) {
        // 🟩 R-type (Register)
        case 0b10001011000: execute_add(instr); break;     // ADD Xd, Xn, Xm
        case 0b10101011000: execute_adds(instr); break;    // ADDS Xd, Xn, Xm
        case 0b11001011000: execute_sub(instr); break;     // SUB
        case 0b11101011000: execute_subs(instr); break;    // SUBS
        case 0b10001010000: execute_and(instr); break;     // AND
        case 0b11101010000: execute_ands(instr); break;    // ANDS
        case 0b11001010000: execute_eor(instr); break;     // EOR
        case 0b10101010000: execute_orr(instr); break;     // ORR
        case 0b10011011000: execute_mul(instr); break;     // MUL

        // 🟨 I-type (Immediate)
        case 0b1001000100:  execute_addi(instr); break;    // ADDI
        case 0b1011000100:  execute_addis(instr); break;   // ADDIS (ADDS with imm)
        case 0b1101000100:  execute_subi(instr); break;    // SUBI
        case 0b1111000100:  execute_subis(instr); break;   // SUBIS (SUBS with imm)

        // 🟦 D-type (Load/Store)
        case 0b11111000010: execute_ldur(instr); break;    // LDUR
        case 0b11111000000: execute_stur(instr); break;    // STUR
        case 0b00111000000: execute_sturb(instr); break;   // STURB
        case 0b01111000000: execute_sturh(instr); break;   // STURH
        case 0b00111000010: execute_ldurb(instr); break;   // LDURB
        case 0b01111000010: execute_ldurh(instr); break;   // LDURH

        // 🟥 B-type (Unconditional Branch)
        default:
            if (op26 == 0b000101) {
                execute_b(instr); // B label
            } else if (op10 == 0b1101011000011111000000) {
                execute_br(instr); // BR Xn
            }

            // 🟪 CB-type (Conditional Branch)
            else if (op24 == 0b01010100) {
                execute_bcond(instr); // BEQ, BNE, BGT, etc.
            }

            // 🟫 IW-type (MOVZ)
            else if ((instr >> 23) == 0b110100101) {
                execute_movz(instr); // MOVZ
            }

            // 🟥 CBZ/CBNZ
            else if ((instr >> 24) == 0b10110100) {
                execute_cbz(instr); // CBZ
            } else if ((instr >> 24) == 0b10110101) {
                execute_cbnz(instr); // CBNZ
            }

            // 🟥 HLT
            else if ((instr >> 5) == 0b1101010001010000000000) {
                execute_hlt(instr); // HLT
            }

            // 🟩 Shift (Immediate) - LSL/LSR (aliases de UBFM)
            else if ((instr >> 22) == 0b110100101) {
                execute_lsl_lsr(instr); // LSL, LSR
            }

            else {
                printf("Instrucción desconocida: 0x%08X\n", instr);
                exit(1);
            }

            break;
    }
}



void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. 
     * */

    // // Fetch instruction
    // uint32_t instr = mem_read_32(NEXT_STATE.PC);
    // printf("Fetched instruction: 0x%X\n", instr);

    // // Decode instruction
    // uint32_t opcode = (instr >> 21) & 0x7FF;

    // //CHEQUEAR SI ESTAN BIEN LAS MASCARAS
    // uint32_t Rd = instr & 0x1F;
    // uint32_t Rn = (instr >> 5) & 0x1F;
    // uint32_t imm12 = (instr >> 10) & 0xFFF;
    // uint32_t shift = (instr >> 22) & 0x1;

    // printf("Decoded instruction - opcode: 0x%X, Rd: %d, Rn: %d, imm12: %d, shift: %d\n", opcode, Rd, Rn, imm12, shift);

    // NEXT_STATE.PC = CURRENT_STATE.PC + 4;

    // switch (opcode) {
    //     case 0x6A2: // HLT
    //         RUN_BIT = 0;
    //         return;
    //     case 0x558: // ADD Xd, Xn, Xm (Extended register)
    //         ADD(Rd, Rn, imm12);
    //         printf(" ADD X%d, X%d, X%d (Extended register)\n" , Rd, Rn, imm12);
    //         return;
    //     case 0x588: // ADDS Xd, Xn, #imm12 (Extended immediate)
    //         printf(" ADD immediate\n");
    //         ADDis(Rd, Rn, imm12, shift);
    //         return;
    //     case 0x758: // SUB Xd, Xn, Xm (Extended register)
    //         SUB(Rd, Rn, imm12);
    //         printf(" SUB Xd, Xn, Xm (Extended register)\n");
    //         return;
    //     case 0x788: // SUBS Xd, Xn, #imm12 (Extended immediate)
    //         SUBis(Rd, Rn, imm12, shift);
    //         printf(" SUBS Xd, Xn, #imm12 (Extended immediate)\n");
    //         return;
    //     case 0x750: // ANDS Xd, Xn, #imm12 (Extended immediate)
    //         printf(" ANDS Xd, Xn, #imm12 (Extended immediate)\n");
    //         return;
    //     case 0x650: // EOR Xd, Xn, #imm12 (Extended immediate)
    //         printf(" EOR Xd, Xn, #imm12 (Extended immediate)\n");
    //         return;
    //     //implementar caso donde no haya mas instruccionesyy
        
    //     default:
    //         printf("Instruction not implemented: 0x%X\n", opcode);
    //         RUN_BIT = 0;
    //         break;
    // }

    // Extraer campos de la instrucción
    uint32_t instr = mem_read_32(NEXT_STATE.PC);
    printf("Fetched instruction: 0x%X\n", instr);


    decode_instruction( instr );

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

void SUB(uint32_t Rd, uint32_t Rn, uint32_t Rm) {
    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t op2 = CURRENT_STATE.REGS[Rm];
    int64_t result = op1 - op2;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("SUB X%d, X%d, X%d\n", Rd, Rn, Rm);
}

void SUBis(uint32_t Rd, uint32_t Rn, uint32_t imm12, uint32_t shift) {
    if (shift == 1) {
        imm12 = imm12 << 12;
    }
    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t result = op1 - (int64_t)imm12;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("SUB X%d, X%d, #%d\n", Rd, Rn, imm12);
}

// void ANDis(uint32_t Rd, uint32_t Rn, uint32_t imm12, uint32_t shift) {
//     if (shift == 1) {
//         imm12 = imm12 << 12;
//     }
//     int64_t op1 = CURRENT_STATE.REGS[Rn];
//     int64_t result = op1 & (int64_t)imm12;
//     NEXT_STATE.REGS[Rd] = result;
//     NEXT_STATE.FLAG_Z = (result == 0);
//     NEXT_STATE.FLAG_N = (result < 0);
//     printf("AND X%d, X%d, #%d\n", Rd, Rn, imm12);
// }

// void EORis(uint32_t Rd, uint32_t Rn, uint32_t imm12, uint32_t shift) {
//     if (shift == 1) {
//         imm12 = imm12 << 12;
//     }
//     int64_t op1 = CURRENT_STATE.REGS[Rn];
//     int64_t result = op1 ^ (int64_t)imm12;
//     NEXT_STATE.REGS[Rd] = result;
//     NEXT_STATE.FLAG_Z = (result == 0);
//     NEXT_STATE.FLAG_N = (result < 0);
//     printf("EOR X%d, X%d, #%d\n", Rd, Rn, imm12);
// }


void execute_add(uint32_t instr) {
    printf("ADD\n");
}

void execute_adds(uint32_t instr) {
    printf("ADDS\n");
}

void execute_sub(uint32_t instr) {
    printf("SUB\n");
}

void execute_subs(uint32_t instr) {
    printf("SUBS\n");
}

void execute_and(uint32_t instr) {
    printf("AND\n");
}

void execute_ands(uint32_t instr) {
    printf("ANDS\n");
}

void execute_eor(uint32_t instr) {
    printf("EOR\n");
}

void execute_orr(uint32_t instr) {
    printf("ORR\n");
}

void execute_mul(uint32_t instr) {
    printf("MUL\n");
}

void execute_addi(uint32_t instr) {
    printf("ADDI\n");
}

void execute_addis(uint32_t instr) {
    printf("ADDIS\n");
}

void execute_subi(uint32_t instr) {
    printf("SUBI\n");
}

void execute_subis(uint32_t instr) {
    printf("SUBIS\n");
}

void execute_ldur(uint32_t instr) {
    printf("LDUR\n");
}

void execute_stur(uint32_t instr) {
    printf("STUR\n");
}

void execute_sturb(uint32_t instr) {
    printf("STURB\n");
}

void execute_sturh(uint32_t instr) {
    printf("STURH\n");
}

void execute_ldurb(uint32_t instr) {
    printf("LDURB\n");
}

void execute_ldurh(uint32_t instr) {
    printf("LDURH\n");
}

void execute_b(uint32_t instr) {
    printf("B\n");
}

void execute_br(uint32_t instr) {
    printf("BR\n");
}

void execute_bcond(uint32_t instr) {
    printf("Bcond\n");
}

void execute_movz(uint32_t instr) {
    printf("MOVZ\n");
}

void execute_cbz(uint32_t instr) {
    printf("CBZ\n");
}

void execute_cbnz(uint32_t instr) {
    printf("CBNZ\n");
}

void execute_hlt(uint32_t instr) {
    printf("HLT\n");
}

void execute_lsl_lsr(uint32_t instr) {
    printf("LSL/LSR\n");
}

