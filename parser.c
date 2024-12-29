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

    while (fgets(buffer, sizeof(buffer), asmfile) != NULL) {
        // Clean the current line in 'buffer'.
        remove_comments(buffer);
        remove_spaces(buffer);
        remove_newlines(buffer);

        char *at_sign_pos = strchr(buffer, '@'), *at_value_pos = at_sign_pos + 1;
        // Is it a number?
        if (at_sign_pos != NULL && (*(at_value_pos) >= '0' && *(at_value_pos) <= '9')) {
            parse_a(hackfile, buffer, at_sign_pos);
        }
        // If not, Is it a default symbol?
        else if (at_sign_pos != NULL && !(*(at_value_pos) >= '0' && *(at_value_pos) <= '9')) {
            int symbol_index = search_default_symbol(at_value_pos);
            // If it is a default symbol, parse it, else it must be a label or a
            // variable, so leave it there for later.
            if (symbol_index >= 0) {
                strcpy(at_value_pos, default_symbols[symbol_index].memory_address);
                parse_a(hackfile, buffer, at_sign_pos);
            }
            else fprintf(hackfile, "%s\n", buffer);
        }
        // At this point it must be a C-instruction.
        else parse_c(hackfile, buffer);
    }

    fclose(hackfile);
    hackfile = fopen(hackname, "r+");               // To maintain what we already parsed.
    FILE *clean_file = fopen("temp.hack", "w+");    // New file without labels.

    while (fgets(buffer, sizeof(buffer), hackfile) != NULL) {
        if (strchr(buffer, '(') != NULL) {
            add_labels(buffer, &cycle, &label_number);
            cycle++;
        }
        else {
            fprintf(clean_file, "%s", buffer);
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

    hackfile = clean_file;
    rewind(hackfile);

    FILE *final = fopen("temp.hack", "w+");
    while (fgets(buffer, sizeof(buffer), hackfile) != NULL) {
        int found_label = 0;
        int found_variable = 0;
        char *at_sign_pos = strchr(buffer, '@');
        if (at_sign_pos != NULL) {
            char *at_value_pos = at_sign_pos + 1;
            // Check if the label exists and give me the index.
            while (found_label < label_number && str_length_compare(at_value_pos, labels[found_label].symbol_name) != 0) {
                found_label++;
            }
            if (found_label < label_number) strcpy(at_value_pos, labels[found_label].memory_address);

            // Check if the variable exists and give me the index.
            while (found_variable < variable_number && str_length_compare(at_value_pos, variables[found_variable].symbol_name) != 0) {
                found_variable++;
            }
            if (found_variable < variable_number) strcpy(at_value_pos, variables[found_variable].memory_address);

            parse_a(final, buffer, at_sign_pos);
        }
        else if (strchr(buffer, '(') == NULL) {
            fprintf(final, "%s", buffer);
        }
    }
    remove(hackname);
    rename("temp.hack", hackname);
}
