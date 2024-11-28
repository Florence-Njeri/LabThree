#define _XOPEN_SOURCE 1
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "functions.h"
#include "hidden_functions/hidden_functions.h"

int parse_options(int            argc,
                  char * const * argv,
                  char ** __restrict in,
                  char ** __restrict out) {
    for (int32_t i = 0; i < argc; i++) {
        const int option = getopt(argc, argv, "i:o:");
        switch (option) {
            case -1:
                /* No more options */
                i = INT32_MAX - 1;
                break;
            case (int) 'i':
                /* Input file */
                *in = (char *) malloc(sizeof(char) * strlen(optarg));
                (void) strcpy(*in, optarg);
                i++;
                break;
            case (int) 'o':
                /* Output file */
                *out = (char *) malloc(sizeof(char) * strlen(optarg));
                (void) strcpy(*out, optarg);
                i++;
                break;
            case (int) '?':
                /* Ambiguous or unknown */
                (void) fprintf(stderr, "Unknown or ambiguous value.\n");
                return EXIT_FAILURE;

            default:
                /* Unexpected error */
                (void) fprintf(stderr, "An unexpected error occured.\n");
                return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int secure_copy_file(const char * in, const char * out) {
    int error = 0;
    error     = access(in, R_OK);
    if (!error) {
        error = access(out, W_OK);
        if (!error) {
            error = wait_confirmation(in, out);
            copy_file(in, out);
        } else {
            fprintf(stderr, "File %s cannot be written.\n", out);
        }
    } else {
        fprintf(stderr, "File %s cannot be read.\n", in);
    }

    return error;
}