#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"
#include <stdlib.h>

// docker run --rm -it simulador

void execute_add(uint32_t instr);
void execute_subs(uint32_t instr);
void execute_ands(uint32_t instr);
void execute_eor(uint32_t instr);
void execute_orr(uint32_t instr);
void execute_mul(uint32_t instr);
void execute_addis(uint32_t instr);
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
void execute_lsl(uint32_t instr);
void execute_lsr(uint32_t instr);

void execute_cmp(uint32_t instr);



void decode_instruction(uint32_t instr) {

    uint32_t op26 = (instr >> 26) & 0x3F;     // bits [31:26]
    uint32_t op24 = (instr >> 24) & 0xFF;     // bits [31:24]
    uint32_t op21 = (instr >> 21) & 0x7FF;    // bits [31:21]
    uint32_t op10 = (instr >> 10) & 0x3FFFFF; // bits [31:10]
    uint32_t op  = (instr >> 22) & 0x3FF;     // bits [31:22] (p/ instrucciones I)

    
    switch (op21) {

        case 0x6A2: execute_hlt(instr); break;    // HLT
        // 🟩 R-type (Register)
        case 0x558: execute_add(instr); break;    // ADDS Xd, Xn, Xm
        case 0x758: execute_subs(instr); break;    // SUBS
        case 0x750: execute_ands(instr); break;    // ANDS
        case 0x650: execute_eor(instr); break;     // EOR
        case 0x550: execute_orr(instr); break;     // ORR
        case 0x698: execute_mul(instr); break;     // MUL
        case 0x3D2: execute_cmp(instr); break;     // CMP

        // 🟨 I-type (Immediate)
        case 0x588:  execute_addis(instr); break;   // ADDIS (ADDS with imm)
        case 0x788:  execute_subis(instr); break;   // SUBIS (SUBS with imm)

        // 🟦 D-type (Load/Store)
        case 0x7C2: execute_ldur(instr); break;    // LDUR
        case 0x7C0: execute_stur(instr); break;    // STUR
        case 0x1C0: execute_sturb(instr); break;   // STURB
        case 0x3C0: execute_sturh(instr); break;   // STURH
        case 0x1C2: execute_ldurb(instr); break;   // LDURB
        case 0x3C2: execute_ldurh(instr); break;   // LDURH

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
            else if (((instr >> 23) == 0b110100100) && ((instr & 0x3F) == 0x3F)) {
                execute_lsl(instr); // Detecta UBFM cuando actúa como LSL
            } else if (((instr >> 23) == 0b110100100) && ((instr & 0x3F) == 0x00)) {
                execute_lsr(instr); // Detecta UBFM cuando actúa como LSR
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
    uint32_t instr = mem_read_32(NEXT_STATE.PC);
    printf("Fetched instruction: 0x%X\n", instr);

    decode_instruction( instr );

}
//funciona
void execute_add(uint32_t instr) {
    uint32_t Rd = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    uint32_t Rm = (instr >> 16) & 0x1F;


    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t op2 = CURRENT_STATE.REGS[Rm];
    int64_t result = op1 + op2;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("ADD X%d, X%d, X%d\n", Rd, Rn, Rm);

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

//funciona
void execute_addis(uint32_t instr) {
    uint32_t Rd = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    uint32_t imm12 = (instr >> 10) & 0xFFF;
    uint32_t shift = (instr >> 22) & 0x1;
    
    if (shift == 1) {
        imm12 = imm12 << 12;
    }
    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t result = op1 + (int64_t)imm12;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("ADD X%d, X%d, #%d\n", Rd, Rn, imm12);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

//funciona
void execute_subs(uint32_t instr) {
    uint32_t Rd = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    uint32_t Rm = (instr >> 16) & 0x1F;

    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t op2 = CURRENT_STATE.REGS[Rm];
    int64_t result = op1 - op2;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("SUB X%d, X%d, X%d\n", Rd, Rn, Rm);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

//funciona
void execute_subis(uint32_t instr) {
    uint32_t Rd = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    uint32_t imm12 = (instr >> 10) & 0xFFF;
    uint32_t shift = (instr >> 22) & 0x1;

    if (shift == 1) {
        imm12 = imm12 << 12;
    }
    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t result = op1 - (int64_t)imm12;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("SUB X%d, X%d, #%d\n", Rd, Rn, imm12);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

//funciona
void execute_ands(uint32_t instr) {
    uint32_t Rd = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    uint32_t Rm = (instr >> 16) & 0x1F;

    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t op2 = CURRENT_STATE.REGS[Rm];
    int64_t result = op1 & op2;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("AND X%d, X%d, X%d\n", Rd, Rn, Rm);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

//funciona (a chequear igual)
void execute_eor(uint32_t instr) {

    uint32_t Rd = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    uint32_t Rm = (instr >> 16) & 0x1F;

    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t op2 = CURRENT_STATE.REGS[Rm];
    int64_t result = op1 ^ op2;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("EOR X%d, X%d, X%d\n", Rd, Rn, Rm);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

// funciona
void execute_orr(uint32_t instr) {

    uint32_t Rd = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    uint32_t Rm = (instr >> 16) & 0x1F;

    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t op2 = CURRENT_STATE.REGS[Rm];
    int64_t result = op1 | op2;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("ORR X%d, X%d, X%d\n", Rd, Rn, Rm);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

// Funciona
void execute_mul(uint32_t instr) {

    uint32_t Rd = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    uint32_t Rm = (instr >> 16) & 0x1F;

    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t op2 = CURRENT_STATE.REGS[Rm];
    int64_t result = op1 * op2;
    NEXT_STATE.REGS[Rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("MUL X%d, X%d, X%d\n", Rd, Rn, Rm);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

//funciona 
void execute_cmp(uint32_t instr) {
    uint32_t Rn = (instr >> 5) & 0x1F;
    uint32_t Rm = (instr >> 16) & 0x1F;

    int64_t op1 = CURRENT_STATE.REGS[Rn];
    int64_t op2 = CURRENT_STATE.REGS[Rm];
    int64_t result = op1 - op2;
    NEXT_STATE.FLAG_Z = (result == 0);
    NEXT_STATE.FLAG_N = (result < 0);
    printf("CMP X%d, X%d\n", Rn, Rm);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

void execute_ldur(uint32_t instr) {
    uint32_t Rt = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    int64_t offset = ((instr >> 10) & 0xFFF); // 12-bit signed offset

    // Sign extend offset if needed
    if (offset & 0x800) {
        offset |= 0xFFFFFFFFFFFFF000;
    }

    int64_t address = CURRENT_STATE.REGS[Rn] + offset;
    NEXT_STATE.REGS[Rt] = mem_read_32(address);
    
    printf("LDUR X%d, [X%d, #%ld]\n", Rt, Rn, offset);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

void execute_stur(uint32_t instr) {
    uint32_t Rt = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    int64_t offset = ((instr >> 10) & 0xFFF); // 12-bit signed offset

    // Sign extend offset if needed
    if (offset & 0x800) {
        offset |= 0xFFFFFFFFFFFFF000;
    }

    int64_t address = CURRENT_STATE.REGS[Rn] + offset;
    mem_write_32(address, CURRENT_STATE.REGS[Rt]);
    
    printf("STUR X%d, [X%d, #%ld]\n", Rt, Rn, offset);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

void execute_sturb(uint32_t instr) {
    uint32_t Rt = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    int64_t offset = ((instr >> 10) & 0xFFF); // 12-bit signed offset

    // Sign extend offset if needed
    if (offset & 0x800) {
        offset |= 0xFFFFFFFFFFFFF000;
    }

    int64_t address = CURRENT_STATE.REGS[Rn] + offset;
    uint8_t value = CURRENT_STATE.REGS[Rt] & 0xFF; // Extraer los primeros 8 bits

    mem_write_32(address, value); // Escribir solo 1 byte en memoria
    
    printf("STURB X%d, [X%d, #%ld]\n", Rt, Rn, offset);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}


void execute_sturh(uint32_t instr) {
    uint32_t Rt = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    int64_t offset = ((instr >> 10) & 0xFFF); // 12-bit signed offset

    // Sign extend offset if needed
    if (offset & 0x800) {
        offset |= 0xFFFFFFFFFFFFF000;
    }

    int64_t address = CURRENT_STATE.REGS[Rn] + offset;
    uint16_t value = CURRENT_STATE.REGS[Rt] & 0xFFFF; // Extraer los primeros 16 bits

    mem_write_32(address, value); // Escribir solo 2 bytes en memoria
    
    printf("STURH W%d, [X%d, #%ld]\n", Rt, Rn, offset);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

void execute_ldurb(uint32_t instr) {
    uint32_t Rt = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    int64_t offset = ((instr >> 10) & 0xFFF); // 12-bit signed offset

    // Sign extend offset if needed
    if (offset & 0x800) {
        offset |= 0xFFFFFFFFFFFFF000;
    }

    int64_t address = CURRENT_STATE.REGS[Rn] + offset;
    uint8_t value = mem_read_32(address); // Leer solo 1 byte de memoria

    NEXT_STATE.REGS[Rt] = (uint64_t)value; // Guardar en el registro con padding de ceros
    
    printf("LDURB W%d, [X%d, #%ld]\n", Rt, Rn, offset);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

void execute_ldurh(uint32_t instr) {
    uint32_t Rt = instr & 0x1F;
    uint32_t Rn = (instr >> 5) & 0x1F;
    int64_t offset = ((instr >> 10) & 0xFFF); // 12-bit signed offset

    // Sign extend offset if needed
    if (offset & 0x800) {
        offset |= 0xFFFFFFFFFFFFF000;
    }

    int64_t address = CURRENT_STATE.REGS[Rn] + offset;
    uint16_t value = mem_read_32(address); // Leer solo 2 bytes de memoria

    NEXT_STATE.REGS[Rt] = (uint64_t)value; // Guardar en el registro con padding de ceros
    
    printf("LDURH W%d, [X%d, #%ld]\n", Rt, Rn, offset);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

void execute_b(uint32_t instr) {
    int32_t imm26 = (instr & 0x03FFFFFF); // Los primeros 26 bits del inmediato
    if (imm26 & 0x02000000) { // Si el bit más significativo es 1, realizar una extensión de signo
        imm26 |= 0xFC000000;
    }
    
    // Desplazar 2 bits a la derecha (dividir por 4) para la dirección real de salto
    imm26 = imm26 << 2;

    int64_t target_address = CURRENT_STATE.PC + imm26;

    NEXT_STATE.PC = target_address;

    printf("B target\n");
}

void execute_br(uint32_t instr) {
    uint32_t Rn = (instr >> 5) & 0x1F; // Obtener el número de registro Xn

    int64_t target_address = CURRENT_STATE.REGS[Rn]; // Dirección de destino almacenada en el registro Xn

    NEXT_STATE.PC = target_address; // Actualizar el contador de programa con la dirección de salto

    printf("BR X%d\n", Rn);
}

// Funciona
void execute_bcond(uint32_t instr) {
    uint32_t cond = (instr >> 24) & 0xF; // Obtener el código de condición de 4 bits
    int32_t imm19 = (instr & 0x00FFFFE0); // Los primeros 19 bits del inmediato
    if (imm19 & 0x00080000) { // Si el bit más significativo es 1, realizar una extensión de signo
        imm19 |= 0xFFE00000;
    }
    
    // Desplazar 2 bits a la derecha (dividir por 4) para la dirección real de salto
    imm19 = imm19 << 2;

    int64_t target_address = CURRENT_STATE.PC + imm19;

    switch (cond) {
        case 0b0000: // BEQ
            if (CURRENT_STATE.FLAG_Z) {
                NEXT_STATE.PC = target_address;
            }
            break;
        case 0b0001: // BNE
            if (!CURRENT_STATE.FLAG_Z) {
                NEXT_STATE.PC = target_address;
            }
            break;
        case 0b0010: // BGT (Mayor que): Z == 0 y N == V
            if (!CURRENT_STATE.FLAG_Z && (CURRENT_STATE.FLAG_N == 0)) {
                NEXT_STATE.PC = target_address;
            }
            break;
        case 0b0011: // BLT (Menor que): N != V
            if (CURRENT_STATE.FLAG_N != 0) {
                NEXT_STATE.PC = target_address;
            }
            break;
        case 0b0100: // BGE (Mayor o igual que): Z == 1 o N == V
            if (CURRENT_STATE.FLAG_Z || (CURRENT_STATE.FLAG_N == 0)) {
                NEXT_STATE.PC = target_address;

            }
            break;
        case 0b0101: // BLE (Menor o igual que): Z == 1 o N != V
            if (CURRENT_STATE.FLAG_Z || (CURRENT_STATE.FLAG_N != 0)) {
                NEXT_STATE.PC = target_address;
            }
            break;

        default:
            printf("Condición desconocida: 0x%X\n", cond);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            exit(1);

    }

    printf("B%s #%d\n", cond == 0xE ? "" : cond == 0x0 ? "EQ" : cond == 0x1 ? "NE" : cond == 0xA ? "GT" : 
        cond == 0xB ? "GE" : cond == 0xC ? "LT" : cond == 0xD ? "LE" : "???", imm19);
}

//funciona
void execute_movz(uint32_t instr) {
    uint32_t Rd = instr & 0x1F;
    uint32_t imm16 = (instr >> 5) & 0xFFFF;
    uint32_t hw = (instr >> 21) & 0x3;
    uint32_t shift = (instr >> 22) & 0x1;

    if (shift == 0) {
        NEXT_STATE.REGS[Rd] = imm16;
    }
    printf("MOVZ X%d, #%d\n", Rd, imm16);

    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

// crear test
void execute_cbz(uint32_t instr) {
    
    uint32_t Rt = (instr >> 5) & 0x1F;
    uint32_t imm19 = (instr >> 5) & 0x7FFFF;

    printf("CBZ X%d, #%d\n", Rt, imm19);
    if (CURRENT_STATE.REGS[Rt] == 0) {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm19 << 2);
    }
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

// crear test
void execute_cbnz(uint32_t instr) {

    uint32_t Rt = (instr >> 5) & 0x1F;
    uint32_t imm19 = (instr >> 5) & 0x7FFFF;

    printf("CBNZ X%d, #%d\n", Rt, imm19);
    if (CURRENT_STATE.REGS[Rt] != 0) {
        NEXT_STATE.PC = CURRENT_STATE.PC + (imm19 << 2);
    }
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

void execute_hlt(uint32_t instr) {
    printf("HLT\n");
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    RUN_BIT = 0;
}

void execute_lsr(uint32_t instr) {
    uint32_t Rd = instr & 0x1F;           
    uint32_t Rn = (instr >> 5) & 0x1F;    
    uint32_t imm6 = (instr >> 10) & 0x3F; 

    int64_t op1 = CURRENT_STATE.REGS[Rn]; 
    int64_t result = op1 >> imm6;         

    NEXT_STATE.REGS[Rd] = result;         
    NEXT_STATE.FLAG_Z = (result == 0);    
    NEXT_STATE.FLAG_N = (result < 0);     

    printf("LSR X%d, X%d, #%d\n", Rd, Rn, imm6); 
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

void execute_lsl(uint32_t instr) {
    uint32_t Rd = instr & 0x1F;          
    uint32_t Rn = (instr >> 5) & 0x1F;    
    uint32_t imm6 = (instr >> 10) & 0x3F; 

    int64_t op1 = CURRENT_STATE.REGS[Rn]; 
    int64_t result = op1 << imm6;         

    NEXT_STATE.REGS[Rd] = result;         
    NEXT_STATE.FLAG_Z = (result == 0);    
    NEXT_STATE.FLAG_N = (result < 0);     

    printf("LSL X%d, X%d, #%d\n", Rd, Rn, imm6);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}