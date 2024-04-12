#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

/* This is a project to create a simple Unix Shell that closely,
   follows Project 2a from my old Operating Systems class at 
   University of Wisconsin. */

char error_message[30] = "An error has occurred\n";
char allocation_fail[30] = "Memory allocation failed\n";
/* write(STDERR_FILENO, error_message, strlen(error_message)); 
   Use this line of code to write the only error message */

void clean_whitespace(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    int length = strlen(str);
    int start_index = 0;
    int end_index = length - 1;

    // Move start_index forward until a non-whitespace character is found
    while (isspace(str[start_index])) {
        start_index++;
    }

    // Move end_index backwards until a non-whitespace character is found
    while (end_index >= start_index && isspace(str[end_index])) {
        end_index--;
    }

    // Move non-whitespace characters to the beginning of the string
    int i, j;
    for (i = start_index, j = 0; i <= end_index; i++) {
        if (!isspace(str[i]) || (i > start_index && !isspace(str[i - 1]))) {
            str[j++] = str[i];
        }
    }

    // Terminate the string at the last non-whitespace character
    str[j] = '\0';
    return;
}

int main(int argc, char *argv[]) {
    FILE *file;

    /* Getting input on the command line with
       input_size and input */
    size_t input_size = 0;
    char *input = NULL;

    if (argc == 2) {
        /* we are in batch mode and need to read from a file */
        file = freopen(argv[1], "r", stdin);
        /* stdin is redirected to get input from our batch file */

        if (file == NULL) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }

        /* last error check with our file is to see if it is empty or not */
        fseek(file, 0, SEEK_END);
        if (ftell(file) == 0) {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        fseek(file, 0, SEEK_SET);
        /* resets us back to the start of the file*/
    }

    while (1) {
        if (argc == 1) {
            /* we are not in batch mode,
               only birkbash > when in
               interactive mode */
            write(STDIN_FILENO, "birkbash > ", strlen("birkbash > "));
        } else if (argc > 2) {
            /* our program only additionally accepts a file name 
               for batch processing, anymore than one extra argument 
               is not applicable to this shell */
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }

        if (getline(&input, &input_size, stdin) == -1) {
            /* IF we hit the EOF then we display an error,
               and quit the shell gracefully with exit */
            write(STDERR_FILENO, error_message, strlen(error_message));

            /* Close the file if we are running batch mode */
            if (argc == 2) { fclose(file); }
            exit(0);
        }

        if (strlen(input) == 1) {
            /* User entered an empty command,
               just print the shell prompt again */
            input = NULL;
        } else {
            /* Not an empty command, call clean_whitespace to
               sanitize the input */
            clean_whitespace(input);
            printf("%s\n", input);
        }
    }

    return 0;
}