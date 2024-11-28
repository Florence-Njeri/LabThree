#include "functions/functions.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {

    char * input  = NULL;
    char * output = NULL;

    const int error = parse_options(argc, argv, &input, &output);
    if (error) {
        if (input != NULL) {
            free(input);
        }
        if (output != NULL) {
            free(output);
        }
        return EXIT_FAILURE;
    }

    if (input == NULL || output == NULL) {
        fprintf(stderr, "'-o <out>' and '-i <in>' have to be provided.\n");
        return EXIT_FAILURE;
    }

    const int result = secure_hash_file(input, output);

    if (input != NULL) {
        free(input);
    }
    if (output != NULL) {
        free(output);
    }

    return result;
}