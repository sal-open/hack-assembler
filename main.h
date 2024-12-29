#ifndef MAIN_H
#define MAIN_H

void parser(FILE *asmfile, FILE *tempfile, char *hack_name);

// Function to check if argument is a file path. If not, it returns the original file name.
char *is_file_path(char *file_name) {
    char *f_slash = strrchr(file_name, '/');
    char *b_slash = strrchr(file_name, '\\');

    if (f_slash != NULL) return f_slash + 1;
    if (b_slash != NULL) return b_slash + 1;
    return file_name;
}

// Function to convert original '.asm' extension to '.hack'.
char *hack_file(char *file_name, char hack_name[]) {
    char *original = is_file_path(file_name);

    int i = 0;
    while (*original != '.' && *original != '\0') {
        hack_name[i] = *original;
        i++;
        original++;
    }

    hack_name[i] = '\0';
    strcat(hack_name, ".hack");
    return hack_name;
}

#endif
