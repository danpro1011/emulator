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
    
    reg.fill(0);
    memory.fill(0);
    stack.fill(0);
    input.fill(0);
};
