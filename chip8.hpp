#include <string>
#pragma once

class Chip8 {
    public: 
    Chip8();
    bool loadRom();
    void Cycle();
    
    
    private:
    // memory
    uint8_t memory[4096];
    //registers
    uint8_t reg[16];
    //12 bit index register 
    uint16_t addressRegister; // ignore 4 bits in this Register
    //program counter
    uint16_t PC;
    
    //stack
    uint8_t sp = 0; //pointer
    uint16_t stack[32]; // 48 bytes to 12 levels

    uint8_t delayTimer;
    uint8_t soundTimer;

    //0 to F "8", "4", "6", "2" directional input. array stores 0/1, 0 = not pressed, 1 = pressed
    uint8_t input[16];


}