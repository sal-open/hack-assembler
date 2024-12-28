#ifndef PARSER_H
#define PARSER_H

// Function to convert to binary.
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
    char *dest = strstr(buffer, "//");
    if (dest != NULL) {
        *dest = '\n';
        *dest++;
        *dest = '\0';
    }
}

// Function to remove spaces.
void remove_spaces(char buffer[]) {
    char *src = buffer, *dest = buffer;
    while (*src != '\0') {
        if (*src != ' ') *dest++ = *src;
        src++;
    }
    *dest = '\0';
}

// Function to remove newlines and returns.
void remove_newlines(char buffer[]) {
    char *newlines = strchr(buffer, '\n');
    char *returns = strchr(buffer, '\r');
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
    char *start_label = strchr(buffer, '(');
    char *end_label = strchr(buffer, ')');
    start_label++;
    *end_label = '\0';
    strcpy(labels[*label_number].symbol_name, start_label);
    sprintf(labels[*label_number].memory_address, "%d", (*cycle - *label_number));
    (*label_number)++;
}

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

int add_variable(char variable[], int variable_number) {
    char *start_variable = strchr(variable, '@') + 1;
    int length = strlen(start_variable);
    int index = 0;
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

int comparaStringhe(const char *str1, const char *str2) {
    if (strchr(str1, '\n') != NULL) { *(strchr(str1, '\n')) = '\0'; }
    if (strchr(str2, '\n') != NULL) { *(strchr(str2, '\n')) = '\0'; }
    if (strlen(str1) != strlen(str2)) return -1;
    for (int i = 0; i < strlen(str1); i++) {
        if (str1[i] != str2[i]) return -1;
    }
    return 0;
}

#endif
