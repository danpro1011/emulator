#include "chip8.cpp"

int main(int argc, char **argv) {
    Chip8 chip8;

    chip8.CPUReset();

    // load rom
    chip8.loadRom("TODO");

    // loop (cycle)
    while (true) {
        chip8.Cycle();
    }
}