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
            parse_a(hackfile, buffer, position);
        }
        else if (strchr(buffer, '@') != NULL && !(*(position) >= '0' && *(position) <= '9')) {
            int default_symbol = search_default_symbol(position);
            if (default_symbol >= 0) {
                strcpy(position, default_symbols[default_symbol].memory_address);
                parse_a(hackfile, buffer, position);
            }
            else fprintf(hackfile, "%s\n", buffer);
        }
        else parse_c(hackfile, buffer);
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
            char *temp = strchr(buffer, '@') + 1;
            while (found < label_number && str_length_compare(temp, labels[found].symbol_name) != 0) {
                found++;
            }
            if (found < label_number) strcpy(temp, labels[found].memory_address);

            while (found_variable < variable_number && str_length_compare(temp, variables[found_variable].symbol_name) != 0) {
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
