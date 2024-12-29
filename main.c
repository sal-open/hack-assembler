#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr,"Usage: %s <path>\n", argv[0]);
        return 1;
    }

    char *asm_name = argv[1];
    char hack_conversion[64];
    char *hack_name = hack_file(asm_name, hack_conversion);

    FILE *asmfile = fopen(asm_name, "r");
    FILE *hackfile = fopen(hack_name, "w");

    if (asmfile == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 2;
    }
    if (hackfile == NULL) {
        fprintf(stderr, "Error creating file.\n");
        remove(hack_name);
        return 3;
    }

    parser(asmfile, hackfile, hack_name);

    fclose(asmfile);
    fclose(hackfile);

    return 0;
}