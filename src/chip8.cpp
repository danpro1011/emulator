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
    switch(opcode & 0XF000) { // Check which condition and then subconditions
        case 0x0000: // 3 cases 
            switch (opcode & 0X00FF) {
                case 0x00EE:
                    OP_00EE_ReturnFromSubroutine();
                    break;
                case 0x00E0:
                    OP_00E0_ClearScreen();
                    break;
                default: // Most machines do nothing here
                    OP_0NNN_CallMachineCode();
                    break;
            }
            break;
        case 0x1000:
            OP_1NNN_Jump();
            break;
        case 0x2000:
            OP_2NNN_CallSubroutine();
            break;
        case 0x300:
            OP_3XNN_SkipIfVxEqualsNN();
            break;
        case 0x400:
            OP_4XNN_SkipIfVxNotEqualsNN();
            break;
        case 0x5000;
            OP_5XY0_SkipIfVxEqualsVy();
            break;
        case 0x6000:
            OP_6XNN_SetVx();
            break;
        case 0x7000:
            OP_7XNN_AddNNToVx();
            break;
        case 0x8000:
            break;
        case 0x9000:
            OP_9XY0_SkipIfVxNotEqualsVy();
            break;
        case 0xA000:
            OP_ANNN_SetI();
            break;
        case 0xB000:
            OP_BNNN_JumpToNNNPlusV0();
            break;
        case 0xC000:
            OP_CXNN_SetVxToRandomAndNN();
            break;
        case 0xE000:
            break;
        case 0xF000:
            break;
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
    uint8_t constant = opcode & 0x00FF;
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    if(reg[regXid] == constant){
        PC += 2;
    }
}

void Chip8::OP_4XNN_SkipIfVxNotEqualsNN() {
    uint8_t constant = opcode & 0x00FF;
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    if(reg[regXid] != constant){ 
        PC += 2;
    }
}

void Chip8::OP_5XY0_SkipIfVxEqualsVy() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t regYid = (opcode & 0x00F0) >> 4; 
    if (reg[regYid] == reg[regXid]) {
        PC += 2;
    }
}

void Chip8::OP_6XNN_SetVx() {
    uint8_t constant = opcode & 0x00FF;
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    reg[regXid] = constant;
}

void Chip8::OP_7XNN_AddNNToVx() {
    uint8_t constant = opcode & 0x00FF;
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    reg[regXid] += constant;
}

void Chip8::OP_8XY0_SetVxToVy() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t regYid = (opcode & 0x00F0) >> 4; 
    reg[regXid] = reg[regYid];
}

void Chip8::OP_8XY1_SetVxOrVy() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t regYid = (opcode & 0x00F0) >> 4; 
    reg[regXid] |= reg[regYid];
}

void Chip8::OP_8XY2_SetVxAndVy() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t regYid = (opcode & 0x00F0) >> 4; 
    reg[regXid] &= reg[regYid];
}

void Chip8::OP_8XY3_SetVxXorVy() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t regYid = (opcode & 0x00F0) >> 4; 
    reg[regXid] ^= reg[regYid];
}

void Chip8::OP_8XY4_AddVyToVx(){
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t regYid = (opcode & 0x00F0) >> 4;
    uint16_t sum = reg[regXid] + reg[regYid];

    reg[regXid] = sum & 0x00FF;

    reg[0x000F] = (sum > 0x00FF) ? 1 : 0;
}

void Chip8::OP_8XY5_SubtractVyFromVx() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t regYid = (opcode & 0x00F0) >> 4; 
    reg[0x000F] = (reg[regXid] >= reg[regYid]) ? 1 : 0;
    reg[regXid] -= reg[regYid];
}

void Chip8::OP_8XY6_ShiftVxRight() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    reg[0x000F] = reg[regXid] & 0x0001;
    reg[regXid] >>= 1;
}

void Chip8::OP_8XY7_SetVxToVyMinusVx() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t regYid = (opcode & 0x00F0) >> 4; 
    reg[0x000F] = (reg[regYid] >= reg[regXid]) ? 1 : 0;
    reg[regXid] = reg[regYid] - reg[regXid];
}

void Chip8::OP_8XYE_ShiftVxLeft() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    reg[0x000F] = reg[regXid] & 0x0080;
    reg[regXid] <<= 1;
}

void Chip8::OP_9XY0_SkipIfVxNotEqualsVy() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t regYid = (opcode & 0x0F00) >> 4;

    if(reg[regXid] != reg[regYid]) PC += 2;
}

void Chip8::OP_ANNN_SetI() {
    uint16_t address = (opcode & 0x0FFF);
    indexReg = address;
}

void Chip8::OP_BNNN_JumpToNNNPlusV0() {
    uint16_t address = (opcode & 0x0FFF);
    PC = reg[0] + address;
}

void Chip8::OP_CXNN_SetVxToRandomAndNN() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t constant = opcode & 0x00FF;
    reg[regXid] = (rand() % 256) & constant;
}

void Chip8::OP_DXYN_DrawSprite() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t regYid = (opcode & 0x00F0) >> 4;
    uint8_t height = (opcode & 0x000F); 

    uint8_t Vx = reg[regXid] % 64;
    uint8_t Vy = reg[regYid] % 32;

    reg[0x000F] = 0;

    for(uint8_t i = 0; i < height; i++) {
        uint8_t byte = memory[indexReg + i];
        for (uint8_t j = 0; j < 8; j ++) {
            uint8_t mask = 0x80 >> j;
            uint8_t byteCheck = mask & byte;

            if (byteCheck != 0) {
                uint8_t x = (Vx + j) % 64;
                uint8_t y = (Vy + i) % 32;
                uint16_t screenIndex = y * 64 + x;

                if (video[screenIndex] == 1) {
                    reg[0x000F] = 1;
                }

                video[screenIndex] ^= 1;
            }
        }
    }

}

void Chip8::OP_EX9E_SkipIfKeyInVxPressed() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    bool keyPressed = input[reg[regXid]];
    if (keyPressed){
        PC += 2;
    }
}

void Chip8::OP_EXA1_SkipIfKeyInVxNotPressed() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    bool keyPressed = input[reg[regXid]];
    if (!keyPressed){
        PC += 2;
    }
}

void Chip8::OP_FX07_SetVxToDelayTimer() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    reg[regXid] = delayTimer;
}

void Chip8::OP_FX0A_WaitForKeyPress() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    
    for(uint8_t key = 0; key < 16; key++) {
        if (input[key]) {
            reg[regXid] = key;
            return;
        }
    }

    PC -= 2;
}

void Chip8::OP_FX15_SetDelayTimerToVx() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    delayTimer = reg[regXid];
}

void Chip8::OP_FX18_SetSoundTimerToVx() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    soundTimer = reg[regXid];
}

void Chip8::OP_FX1E_AddVxToI() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    indexReg += reg[regXid];
}

void Chip8::OP_FX29_SetIToFontSpriteAddress() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t digit = reg[regXid] & 0x000F;
    indexReg = 0x50 + (digit * 5);
}

void Chip8::OP_FX33_StoreBCDOfVx() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    uint8_t value = reg[regXid];

    memory[indexReg] = value / 100;
    memory[indexReg + 1] = (value / 10) % 10;
    memory[indexReg + 2] = (value) % 10;
}


void Chip8::OP_FX55_StoreV0ThroughVxInMemory() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    for (uint8_t i = 0; i <=  regXid; i ++) {
        memory[indexReg + i] = reg[i];
    }

}

void Chip8::OP_FX65_LoadV0ThroughVxFromMemory() {
    uint8_t regXid = (opcode & 0x0F00) >> 8;
    for (uint8_t i = 0; i <=  regXid; i ++) {
        reg[i] = memory[indexReg + i];
    }
}







