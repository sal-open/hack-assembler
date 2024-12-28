#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

typedef struct {
    char memory_address[16]; // Numerical value as a string
    char symbol_name[64];    // Symbol name
} symbol;

// Default symbols
symbol default_symbols[23] = {
    {"0", "SP"},  {"1", "LCL"},  {"2", "ARG"},
    {"3", "THIS"}, {"4", "THAT"}, {"0", "R0"},
    {"1", "R1"},   {"2", "R2"},   {"3", "R3"},
    {"4", "R4"},   {"5", "R5"},   {"6", "R6"},
    {"7", "R7"},   {"8", "R8"},   {"9", "R9"},
    {"10", "R10"}, {"11", "R11"}, {"12", "R12"},
    {"13", "R13"}, {"14", "R14"}, {"15", "R15"},
    {"16384", "SCREEN"}, {"24576", "KBD"}
};

// Labels found in the .asm
symbol labels[16384];

// Variables found in the .asm
symbol variables[16384];

// Computation table (C-instruction "comp" mnemonics)
const symbol comp_table[] = {
    {"0101010", "0"},   {"0111111", "1"},   {"0111010", "-1"},
    {"0001100", "D"},   {"0110000", "A"},   {"1110000", "M"},
    {"0001101", "!D"},  {"0110001", "!A"},  {"1110001", "!M"},
    {"0001111", "-D"},  {"0110011", "-A"},  {"1110011", "-M"},
    {"0011111", "D+1"}, {"0110111", "A+1"}, {"1110111", "M+1"},
    {"0001110", "D-1"}, {"0110010", "A-1"}, {"1110010", "M-1"},
    {"0000010", "D+A"}, {"1000010", "D+M"}, {"0010011", "D-A"},
    {"1010011", "D-M"}, {"0000111", "A-D"}, {"1000111", "M-D"},
    {"0000000", "D&A"},  {"1000000", "D&M"}, {"0010101", "D|A"},
    {"1010101", "D|M"}
};

// Jump table (C-instruction "jump" mnemonics)
const symbol jump_table[7] = {
    {"001", "JGT"}, {"010", "JEQ"}, {"011", "JGE"},
    {"100", "JLT"}, {"101", "JNE"}, {"110", "JLE"},
    {"111", "JMP"}
};

// Destination table (C-instruction "dest" mnemonics)
const symbol dest_table[7] = {
    {"001", "M"},   {"010", "D"},   {"011", "MD"},
    {"100", "A"},   {"101", "AM"},  {"110", "AD"},
    {"111", "AMD"}
};

#endif
