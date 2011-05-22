// Math Instructions ----------------------------------------------------------
// ----------------------------------------------------------------------------
#include "../cpu.h"
extern CPU_8080 *CPU;


// Add (REG) to A
#define ADD(REG) static void ADD_##REG() { \
    uint16_t i = *CPU->A + *CPU->REG; \
    *CPU->A = i & 0xff; \
    cpu_flag_szap(CPU->A); \
    *CPU->F |= i > 0xff; \
}
ADD(B); ADD(C); ADD(D); ADD(E); ADD(H); ADD(L); ADD(A);

// Add contents of (HL) to A
static void ADD_M() { \
    uint16_t i = *CPU->A + read8(*CPU->HL);
    *CPU->A = i & 0xff; // mask to 255
    cpu_flag_szap(CPU->A); // check flags
    *CPU->F |= i > 0xff; // set carry flag
}


// Add (REG + carry) to A
#define ADC(REG) static void ADC_##REG() { \
    uint16_t i = (*CPU->A + *CPU->REG) + (*CPU->F & 1); \
    *CPU->A = i & 0xff; \
    cpu_flag_szap(CPU->A); \
    *CPU->F |= i > 0xff; \
}
ADC(B); ADC(C); ADC(D); ADC(E); ADC(H); ADC(L); ADC(A);

// Add (contents of (HL) + carry) to A
static void ADC_M() {
    uint16_t i = (*CPU->A + read8(*CPU->HL)) + (*CPU->F & 1);
    *CPU->A = i & 0xff; // mask to 255
    cpu_flag_szap(CPU->A); // check flags
    *CPU->F |= i > 0xff; // set carry flag
}


// Subtract (REG) from A
#define SUB(REG) static void SUB_##REG() { \
    int16_t i = *CPU->A - *CPU->REG; \
    *CPU->A = i & 0xff; \
    cpu_flag_szp(CPU->A); \
    *CPU->F |= i < 0; \
}
SUB(B); SUB(C); SUB(D); SUB(E); SUB(H); SUB(L); SUB(A);

// Subtract contents of (HL) from A
static void SUB_M() { \
    int16_t i = *CPU->A - read8(*CPU->HL);
    *CPU->A = i & 0xff; // mask to 255
    cpu_flag_szp(CPU->A); // check flags
    *CPU->F |= i < 0; // set carry flag
}


// Subtract (REG + carry) from A
#define SBB(REG) static void SBB_##REG() { \
    int16_t i = (*CPU->A - *CPU->REG) - (*CPU->F & 1); \
    *CPU->A = i & 0xff; \
    cpu_flag_szp(CPU->A); \
    *CPU->F |= i < 0; \
}
SBB(B); SBB(C); SBB(D); SBB(E); SBB(H); SBB(L); SBB(A);

// Subtract (contents of (HL) + carry) from A
static void SBB_M() {
    int16_t i = (*CPU->A - read8(*CPU->HL)) - (*CPU->F & 1);
    *CPU->A = i & 0xff; // mask to 255
    cpu_flag_szp(CPU->A); // check flags
    *CPU->F |= i < 0; // set carry flag
}


// 16 BIT ---------------------------------------------------------------------
// ----------------------------------------------------------------------------

// Add BC to HL
static void DAD_B() {
    uint32_t i = *CPU->HL + *CPU->BC;
    *CPU->F = i > 0xffff; // Carry is bit 1, so this clears and sets correctly
    *CPU->HL = (i & 0xffff);
}

// Add DE to HL
static void DAD_D() {
    uint32_t i = *CPU->HL + *CPU->DE;
    *CPU->F = i > 0xffff;
    *CPU->HL = (i & 0xffff);
}

// Add HL to HL
static void DAD_H() {
    uint32_t i = *CPU->HL + *CPU->HL;
    *CPU->F = i > 0xffff;
    *CPU->HL = (i & 0xffff);
}

// Add SP to HL
static void DAD_SP() {
    uint32_t i = *CPU->HL + *CPU->SP;
    *CPU->F = i > 0xffff;
    *CPU->HL = (i & 0xffff);
}
