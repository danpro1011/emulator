#include "chip8.hpp"
#include <fstream>

const uint16_t START_ADDRESS = 0x200;

Chip8::Chip8() {
    PC = START_ADDRESS;
};

bool Chip8::loadRom(const std::string &filename) {
    // Open the file as a stream of binary and move pointer to end
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    
    if (file.is_open()) {
        // give size of the file
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        // fill the buffer
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        //  load the rom into chip's memory 
        for (uint16_t i = 0; i < size; i++) {
            memory[PC + i] = buffer[i];
        }

        delete[] buffer;
        return true;
    }
    
    return false;
};

void Chip8::Cycle() {
    // Fetch 
    opcode = memory[PC];
    opcode <<= 8; 
    opcode |= memory[PC + 1];
    PC += 2;

    // Decode
    switch(opcode & 0XF000) {
        
    }

    // Execute
};


/*
 * We will reset the emulator to initial (safe) conditions 
 */
void Chip8::CPUReset() {
    indexReg = 0;
    PC = START_ADDRESS;
    sp = 0;
    delayTimer = 0;
    soundTimer = 0;
    
    reg.fill(0);
    memory.fill(0);
    stack.fill(0);
    input.fill(0);
};

// Opcode functions

void Chip8::OP_0NNN_CallMachineCode() {

}

void Chip8::OP_00E0_ClearScreen() {
    video.fill(0);
}

void Chip8::OP_00EE_ReturnFromSubroutine() {
    sp--;
    PC = stack[sp];
};

void Chip8::OP_1NNN_Jump() {
    uint16_t address = opcode & 0x0FFFu;
    PC = address;
}

void Chip8::OP_2NNN_CallSubroutine() {
    uint16_t address = opcode & 0x0FFFu;
    
    stack[sp] = PC;
    sp++;
    PC = address;
}

void Chip8::OP_3XNN_SkipIfVxEqualsNN() {

}

void Chip8::OP_4XNN_SkipIfVxNotEqualsNN() {

}

void Chip8::OP_5XY0_SkipIfVxEqualsVy() {

}

void Chip8::OP_6XNN_SetVx() {
    
}
