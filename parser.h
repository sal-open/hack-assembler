#ifndef PARSER_H
#define PARSER_H

// Function to convert a number into its fifteen long "binary" representation.
void binary(int number, char binary[]) {
    int s = 14;
    while (s >= 0) {
        binary[s] = (number % 2) + '0';
        number = number / 2;
        s--;
    }
    binary[15] = '\0';
}

// Function to remove comments.
void remove_comments(char buffer[]) {
    char *comment = strstr(buffer, "//");
    if (comment != NULL) {
        *comment = '\n';
    }
}

// Function to remove spaces.
void remove_spaces(char buffer[]) {
    char *with_spaces = buffer, *no_spaces = buffer;

    while (*with_spaces != '\0') {
        if (*with_spaces != ' ') *no_spaces++ = *with_spaces;
        with_spaces++;
    }
    *no_spaces = '\0';
}

// Function to remove newlines and returns.
void remove_newlines(char buffer[]) {
    char *newlines = strchr(buffer, '\n'), *returns = strchr(buffer, '\r');

    if (newlines != NULL) *newlines = '\0';
    if (returns != NULL) *returns = '\0';
}

// Function to remove labels.
void remove_labels(char buffer[]) {
    if (strchr(buffer, '(') != NULL) {
        *strchr(buffer, '(') = '\0';
    }
}

// Function to add labels to their array.
void add_labels(char buffer[], int *cycle, int *label_number) {
    char *start_label = strchr(buffer, '('), *end_label = strchr(buffer, ')');

    start_label++;
    *end_label = '\0';
    strcpy(labels[*label_number].symbol_name, start_label);
    sprintf(labels[*label_number].memory_address, "%d", (*cycle - *label_number));
    (*label_number)++;
}

// Function that checks if a label already exists or not. If so, it returns 1, otherwise 0.
int search_label(char label[], int label_number) {
    if (label_number == 0) return 1;

    char *start_label = strchr(label, '@') + 1;
    int index = 0;

    while (index < label_number) {
        if (strncmp(start_label, labels[index].symbol_name, strlen(labels[index].symbol_name)) == 0) {
            return 1;
        }
        index++;
    }
    return 0;
}

// Function to add a variable to variables array.
int add_variable(char variable[], int variable_number) {
    char *start_variable = strchr(variable, '@') + 1;
    int length = strlen(start_variable), index = 0;

    while (index <= variable_number) {
        if (strncmp(start_variable, variables[index].symbol_name, length) == 0) {
            return 0;
        }
        index++;
    }
    strcpy(variables[variable_number].symbol_name, start_variable);
    sprintf(variables[variable_number].memory_address, "%d", 16 + variable_number);
    return 1;
}

// Function that checks whether the instruction given is a default symbol. If so, it returns its index, otherwise -1.
int search_default_symbol(char *instruction) {
    int index = 0;
    while (index < 23) {
        if (strncmp(instruction, default_symbols[index].symbol_name, strlen(instruction)) == 0) {
            return index;
        }
        index++;
    }
    return -1;
}

// Function that checks if two strings are equally long. If so, it returns 0, otherwise -1.
int str_length_compare(const char *str1, const char *str2) {
    if (strchr(str1, '\n') != NULL) { *(strchr(str1, '\n')) = '\0'; }
    if (strchr(str2, '\n') != NULL) { *(strchr(str2, '\n')) = '\0'; }
    if (strlen(str1) != strlen(str2)) return -1;
    for (int i = 0; i < strlen(str1); i++) {
        if (str1[i] != str2[i]) return -1;
    }
    return 0;
}

void parse_a (FILE* hackfile, char buffer[], char *position) {
    int A_instr_value = atoi(position);

    // Converting result into a binary number and putting it into the binary_number array.
    char binary_number[16];
    binary(A_instr_value, binary_number);

    // Substitute '@' with a '0'.
    char *translation = strchr(buffer, '@');
    *translation = '0';

    // Move to the next location.
    translation++;
    strcpy(translation, binary_number);

    // Printing the modified buffer.
    fprintf(hackfile, "%s\n", buffer);
}

void parse_c (FILE* hackfile, char buffer[]) {
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
    else if (strchr(buffer, '(') != NULL) fprintf(hackfile, "%s\n", buffer);
}

#endif
