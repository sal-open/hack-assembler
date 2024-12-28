#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symboltable.h"
#include "parser.h"


void parser(FILE *asmfile, FILE *hackfile, char *hackname) {
    char buffer [256];
    int cycle = 0;
    int label_number = 0;
    int variable_number = 0;

    cycle = 0;
    while (fgets(buffer, sizeof(buffer), asmfile) != NULL) {
        remove_comments(buffer);
        remove_spaces(buffer);
        remove_newlines(buffer);
        cycle++;
        char *position = strchr(buffer, '@') + 1;
        if (strchr(buffer, '@') != NULL && (*(position) >= '0' && *(position) <= '9')) {
            // Check if '@' is in the buffer.
            char *a_instruction = strchr(buffer, '@');

            // Check if the value after '@' is a default symbol and swap it with its memory address.


            // Getting the numerical value of the A instruction into result.
            int result = atoi(position);

            // Converting result into a binary number and putting it into the binary_number array.
            char binary_number[16];
            binary(result, binary_number);

            // Substitute '@' with a '0'.
            char *translation = strchr(buffer, '@');
            *translation = '0';

            // Move to next location.
            translation++;
            strcpy(translation, binary_number);

            // Printing the modified buffer.
            fprintf(hackfile, "%s\n", buffer);
        }
        else if (strchr(buffer, '@') != NULL && !(*(position) >= '0' && *(position) <= '9')) {
            int default_symbol = search_default_symbol(position);
            if (default_symbol >= 0) {
                strcpy(position, default_symbols[default_symbol].memory_address);
                int result = atoi(position);

            // Converting result into a binary number and putting it into the binary_number array.
                char binary_number[16];
                binary(result, binary_number);

            // Substitute '@' with a '0'.
                char *translation = strchr(buffer, '@');
                *translation = '0';

            // Move to next location.
                translation++;
                strcpy(translation, binary_number);
                fprintf(hackfile, "%s\n", buffer);
            }
            else fprintf(hackfile, "%s\n", buffer);
        }
        else {
            char c_instruction[17] = "1110000000000000\0";

            int comp_t = 50;
            int dest_t = 50;
            int jump_t = 50;


            if (strchr(buffer, '=') != NULL) {
                int found = 0;
                int *found_ptr = &found;
                for (int i = 0; i < strlen(buffer); i++) {
                    if (buffer[i] == '=') {
                        *(found_ptr) = i;

                        dest_t = 0;
                        if (found > 0) {
                            while (dest_t < 7 && strncmp(buffer, dest_table[dest_t].symbol_name, found) != 0) {
                                dest_t++;
                            }
                        }
                        break;
                    }
                }
            }
            if (strchr(buffer, ';') != NULL) {
                jump_t = 0;
                while (jump_t < 7 && strncmp(strchr(buffer, ';')+1, jump_table[jump_t].symbol_name, strlen(jump_table[jump_t].symbol_name)) != 0) {jump_t++;}
            }
            if (dest_t < 7) {    // It means that there is a dest field.
                comp_t = 0;
                char temp[256];
                strcpy(temp, buffer);
                char *equal = strchr(temp, '=') +1  ;
                while (comp_t < 28 && strncmp(equal, comp_table[comp_t].symbol_name, strlen(temp)) != 0) {comp_t++;}
            }
            else if (jump_t < 7) {  // It means that there is a jump field.
                comp_t = 0;
                char temp[256];
                strcpy(temp, buffer);
                *strchr(temp, ';') = '\0';
                while (comp_t < 28 && strncmp(&temp[0], comp_table[comp_t].symbol_name, strlen(temp)) != 0) {comp_t++;}
            }

            if (comp_t < 28) {
                strncpy(c_instruction + 3, comp_table[comp_t].memory_address, 7);
            }
            if (dest_t < 7) {
                strncpy(c_instruction + 10, dest_table[dest_t].memory_address, 3);
            }
            if (jump_t < 7) {
                strncpy(c_instruction + 13, jump_table[jump_t].memory_address, 3);
            }
            if (comp_t < 28 || jump_t < 7 || dest_t < 7) fprintf(hackfile, "%s\n", c_instruction);
            else if (strchr(buffer, '(') != NULL) {
                fprintf(hackfile, "%s\n", buffer);
            }
        }
    }

    cycle = 0;
    memset(buffer, '\0', sizeof(buffer));

    fclose(hackfile);
    hackfile = fopen(hackname, "r+");
    FILE *tempfile = fopen("temp.hack", "w+");

    while (fgets(buffer, sizeof(buffer), hackfile) != NULL) {
        if (strchr(buffer, '(') != NULL) {
            add_labels(buffer, &cycle, &label_number);
            cycle++;
        }
        else {
            fprintf(tempfile, "%s", buffer);
            cycle++;
        }
    }

    rewind(hackfile);
    while (fgets(buffer, sizeof(buffer), hackfile) != NULL) {
        if (strchr(buffer, '@') == NULL) continue;
        char *start = strchr(buffer, '@') + 1;
        if (*(start) >= '0' && *(start) <= '9') continue;
        if (search_label(buffer, label_number) == 0) {
            if (add_variable(buffer, variable_number) == 1) variable_number++;
        }
    }

    remove(hackname);
    rename("temp.hack", hackname);

    hackfile = tempfile;
    rewind(hackfile);


    FILE *tempfile_two = fopen("temp.hack", "w+");
    while (fgets(buffer, sizeof(buffer), hackfile) != NULL) {
        int found = 0;
        int found_variable = 0;
        if (strchr(buffer, '@') != NULL) {
            // while (found < label_number && strncmp(strchr(buffer, '@') + 1 , labels[found].symbol_name, strlen(labels[found].symbol_name)) != 0) {
            //     found++;
            // }
            // if (found < label_number) strcpy(strchr(buffer, '@')+1, labels[found].memory_address);
            //
            // while (found_variable < variable_number && strncmp(strchr(buffer, '@') + 1 , variables[found_variable].symbol_name, strlen(variables[found_variable].symbol_name)) != 0) {
            //     found_variable++;
            // }
            char *temp = strchr(buffer, '@') + 1;
            while (found < label_number && comparaStringhe(temp, labels[found].symbol_name) != 0) {
                found++;
            }
            if (found < label_number) strcpy(temp, labels[found].memory_address);

            while (found_variable < variable_number && comparaStringhe(temp , variables[found_variable].symbol_name) != 0) {
                found_variable++;
            }
            if (found_variable < variable_number) strcpy(temp, variables[found_variable].memory_address);

            int result = atoi(strchr(buffer, '@') + 1);
            char binary_number[16];
            binary(result, binary_number);

            // Substitute '@' with a '0'.
            char *translation = strchr(buffer, '@');
            *translation = '0';

            // Move to next location.
            translation++;
            strcpy(translation, binary_number);
            fprintf(tempfile_two, "%s\n", buffer);
        }
        else if (strchr(buffer, '(') == NULL) {
            fprintf(tempfile_two, "%s", buffer);
        }
    }
    remove(hackname);
    rename("temp.hack", hackname);
}
