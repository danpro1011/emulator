#include <string>
#include <array>
#include <cstdint>
#pragma once

class Chip8 {
    public: 
    Chip8();

    bool loadRom(const std::string &filename); // want address of string file

    void Cycle(); // fetch -> decode -> execute opcode

    void CPUReset();
    
    
    private:
    // memory
    std::array<uint8_t, 4096> memory{};
    //registers
    std::array<uint8_t, 16> reg{};
    //12 bit index register 
    uint16_t indexReg{}; // ignore 4 bits in this Register
    //program counter
    uint16_t PC = 0x200; // start at 0x200 below is reserved
    
    //stack
    uint8_t sp{}; //pointer
    std::array<uint16_t, 32> stack{}; // 48 bytes upscaled to 64

    uint8_t delayTimer{};
    uint8_t soundTimer{};

    //0 to F "8", "4", "6", "2" directional input. array stores 0/1, 0 = not pressed, 1 = pressed
    std::array<uint8_t, 16> input{};

};