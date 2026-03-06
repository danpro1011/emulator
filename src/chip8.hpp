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
    uint16_t PC; // start at 0x200 below is reserved
    
    //stack
    uint8_t sp{}; //pointer
    std::array<uint16_t, 32> stack{}; // 48 bytes upscaled to 64

    uint8_t delayTimer{};
    uint8_t soundTimer{};

    //0 to F "8", "4", "6", "2" directional input. array stores 0/1, 0 = not pressed, 1 = pressed
    std::array<uint8_t, 16> input{};

    // display
    std::array<uint32_t, (64 * 32)> video{};

    // opcode handlers
    void OP_0NNN_CallMachineCode();     // usually ignore
    void OP_00E0_ClearScreen();
    void OP_00EE_ReturnFromSubroutine();

    void OP_1NNN_Jump();
    void OP_2NNN_CallSubroutine();

    void OP_3XNN_SkipIfVxEqualsNN();
    void OP_4XNN_SkipIfVxNotEqualsNN();
    void OP_5XY0_SkipIfVxEqualsVy();

    void OP_6XNN_SetVx();
    void OP_7XNN_AddNNToVx();

    void OP_8XY0_SetVxToVy();
    void OP_8XY1_SetVxOrVy();
    void OP_8XY2_SetVxAndVy();
    void OP_8XY3_SetVxXorVy();
    void OP_8XY4_AddVyToVx();
    void OP_8XY5_SubtractVyFromVx();
    void OP_8XY6_ShiftVxRight();
    void OP_8XY7_SetVxToVyMinusVx();
    void OP_8XYE_ShiftVxLeft();

    void OP_9XY0_SkipIfVxNotEqualsVy();

    void OP_ANNN_SetI();
    void OP_BNNN_JumpToNNNPlusV0();
    void OP_CXNN_SetVxToRandomAndNN();
    void OP_DXYN_DrawSprite();

    void OP_EX9E_SkipIfKeyInVxPressed();
    void OP_EXA1_SkipIfKeyInVxNotPressed();

    void OP_FX07_SetVxToDelayTimer();
    void OP_FX0A_WaitForKeyPress();
    void OP_FX15_SetDelayTimerToVx();
    void OP_FX18_SetSoundTimerToVx();
    void OP_FX1E_AddVxToI();
    void OP_FX29_SetIToFontSpriteAddress();
    void OP_FX33_StoreBCDOfVx();
    void OP_FX55_StoreV0ThroughVxInMemory();
    void OP_FX65_LoadV0ThroughVxFromMemory();
    

};