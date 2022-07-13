#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void remove_char(char *str, char garbage)
{
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

char *remove_substring(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}

void process_file(FILE *input_file, FILE *output_file, char *buffer) {
    char *command;
    ssize_t read;
    size_t len = 0;

    if (input_file) {
        while ((read = getline(&buffer, &len, input_file)) != -1) {
            command = strstr(buffer, "imprimere(\"");
            if (command != NULL) {
                command += 11;
                command = remove_substring(command, "\")");
                remove_char(command, '\n');
                fprintf(output_file, "\tprintf(\"");
                fprintf(output_file, command);
                fprintf(output_file, "\\");
                fprintf(output_file, "n");
                fprintf(output_file, "\");\n");
            }
        }
    }
}

int main(int argc, char *argv[]) 
{
    FILE *input_file;
    FILE *output_file;
    FILE *fp;
    int status;
    char input_filename[100];
    char output_filename_aux[100];
    char output_filename[100];
    char extension[6];
    char *buffer = 0;
    char command[1024];

    if (argc < 2) {
        printf("Please specify a file to compile!\n");
        exit(EXIT_FAILURE);
    }
    if (argc < 3) {
        printf("Please specify the name of the binary!\n");
        exit(EXIT_FAILURE);
    }

    strncpy(input_filename, argv[1], strlen(argv[1]));
    input_filename[strlen(argv[1])] = '\0';

    strncpy(output_filename, argv[2], strlen(argv[2]));
    strncpy(output_filename_aux, argv[2], strlen(argv[2]));
    output_filename_aux[strlen(argv[2])] = '.';
    output_filename_aux[strlen(argv[2])+1] = 'c';
    output_filename_aux[strlen(argv[2])+2] = '\0';

    strncpy(extension, &input_filename[strlen(input_filename)-5], 5);

    if (strcmp(".spqr", extension) != 0) {
        printf("The file extension is invalid!\n");
        exit(EXIT_FAILURE);
    }

    input_file = fopen(input_filename, "r");

    if (input_file == NULL)
    {
        printf("Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    output_file = fopen(output_filename_aux, "w");
    if (output_file == NULL)
    {
        printf("Error creating output file\n");
        exit(EXIT_FAILURE);
    }

    fprintf(output_file, "#include <stdio.h>\n\n");
    fprintf(output_file, "int main(void) {\n");
    process_file(input_file, output_file, buffer);
    fprintf(output_file, "\treturn 0;\n");
    fprintf(output_file, "}\n");

    fclose(input_file);
    fclose(output_file);

    sprintf(command, "gcc %s.c -c && gcc %s.o -o %s && rm -rf %s.c %s.o", output_filename, output_filename, output_filename, output_filename, output_filename);
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("An error occurred while initiating pipe stream.\n");
        exit(EXIT_FAILURE);
    }
    
    status = pclose(fp);
    if (status == -1) {
        printf("An error occurred during compilation.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Compiled successfully!\n");
    }

    return 0;
}