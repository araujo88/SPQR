#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int roman_to_integer(char c)
{
    switch(c) {
    case 'I':  
        return 1;  
    case 'V':  
        return 5;  
    case 'X':  
        return 10;  
    case 'L':  
        return 50;  
    case 'C':  
        return 100;  
    case 'D':  
        return 500;  
    case 'M':  
        return 1000;  
    default:
        return 0;
    }
}

int roman_to_int(char *s)
{
    int i, int_num = roman_to_integer(s[0]);

    for (i = 1; s[i] != '\0'; i++) {
        int prev_num = roman_to_integer(s[i - 1]);
        int cur_num = roman_to_integer(s[i]);
        if (prev_num < cur_num) {
            int_num = int_num - prev_num + (cur_num - prev_num);
        } else {
            int_num += cur_num;
        }
    }
    return int_num;
}

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

void str_replace(char *target, const char *needle, const char *replacement)
{
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1) {
        const char *p = strstr(tmp, needle);

        // walked past last occurrence of needle; copy remaining part
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }

        // copy part before needle
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        // copy replacement string
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        // adjust pointers, move on
        tmp = p + needle_len;
    }

    // write altered string back to target
    strcpy(target, buffer);
}

void process_file(FILE *input_file, FILE *output_file, char *buffer) {
    char *command;
    ssize_t read;
    size_t len = 0;

    if (input_file) {
        while ((read = getline(&buffer, &len, input_file)) != -1) {
            while(1) {

                // print string
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
                    memset(command, 0, strlen(command));
                    break;
                }

                // print integer
                command = strstr(buffer, "imprimere(");
                if (command != NULL) {
                    command += 10;
                    command = remove_substring(command, "\")");
                    remove_char(command, '\n');
                    fprintf(output_file, "\tprintf(\"%%lld\\n\", ");
                    fprintf(output_file, command);
                    fprintf(output_file, ";\n");
                    memset(command, 0, strlen(command));
                    break;
                }

                // declare integer
                command = strstr(buffer, "numerus ");
                if (command != NULL) {
                    remove_char(command, '\n');
                    fprintf(output_file, "\tlong long int ");
                    command += 8;
                    char number[100];
                    sprintf(number, "%d", roman_to_int(command));
                    remove_char(command, 'I');
                    remove_char(command, 'V');
                    remove_char(command, 'X');
                    remove_char(command, 'L');
                    remove_char(command, 'C');
                    remove_char(command, 'D');
                    remove_char(command, 'M');
                    fprintf(output_file, command);
                    fprintf(output_file, number);
                    fprintf(output_file, ";\n");
                    memset(command, 0, strlen(command));
                    break;
                }

                 // ignores newlines
                command = strstr(buffer, "\n");
                if (command != NULL) {
                    break;
                }
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

    //sprintf(command, "gcc %s.c -c && gcc %s.o -o %s", output_filename, output_filename, output_filename);
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