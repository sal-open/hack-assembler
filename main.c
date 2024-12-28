#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parser(FILE *asmfile, FILE *tempfile, char *hack_name);

// Function to convert original '.asm' extension to '.hack' and return a pointer to
// the first character of the file name.
char *hack_file(char *filename, char converted[]) {
    // Check if the argument is a file path. If so, locate the last occurence
    // of '/' or '\', and update the pointer to the file name, otherwise leave it as filename.
    char *last_occurrence = strrchr(filename, '/');
    char *eventual_occurrence = strrchr(filename, '\\');
    if (last_occurrence != NULL && eventual_occurrence == NULL) last_occurrence++;
    else if (last_occurrence == NULL && eventual_occurrence != NULL) {
        eventual_occurrence++;
        last_occurrence = eventual_occurrence;
    }
    else last_occurrence = filename;

    // Copy the name into converted until you find a '.'
    int i = 0;
    while(*last_occurrence != '.' && *last_occurrence != '\0' && i < 64) {
        converted[i] = *last_occurrence;
        i++;
        last_occurrence++;
    }

    // i is now the the index of the position after the last letter, so we can
    // null-terminate it and use strcat to append '.hack' right after the last letter.
    converted[i] = '\0';
    strcat(converted, ".hack");

    return converted;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr,"Use: ./assembler [file name]\n");
        return 1;
    }
    char *asm_name = argv[1];
    char conversion[64];
    char *hack_name = hack_file(asm_name, conversion);
    FILE *asmfile = fopen(asm_name, "r");
    FILE *hackfile = fopen(hack_name, "w");

    if (asmfile == NULL) {
        fprintf(stderr, "Error opening file\n");
        return 2;
    }
    if (hackfile == NULL) {
        fprintf(stderr, "Error creating file\n");
        return 3;
    }

    parser(asmfile, hackfile, hack_name);

    fclose(asmfile);
    fclose(hackfile);

    return 0;
}