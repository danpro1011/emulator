#include "chip8.hpp"
#include <fstream>

Chip8::Chip8() {
    PC = 0x200;
};

bool Chip8::loadRom(const std::string &filename) {

};

void Chip8::Cycle() {
    // Fetch

    // Decode

    // Execute
};

/*
 * We will reset the emulator to initial (safe) conditions 
 */
void Chip8::CPUReset() {
    indexReg = 0;
    PC = 0x200;
    sp = 0;
    delayTimer = 0;
    soundTimer = 0;
    
    memset(reg, 0, sizeof(reg));
    memset(memory, 0, sizeof(memory));
    memset(stack, 0, sizeof(stack));
    memset(input, 0, sizeof(input));
};
