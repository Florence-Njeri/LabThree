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
    struct stat file_info;
    error = open(in, O_RDONLY | O_NOFOLLOW);

    if (error == -1) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    // Check if input file is a regular file
    if (fstat(error, &file_info) == -1 || !S_ISREG(file_info.st_mode)) {
        fprintf(stderr, "Error: Input file is not a regular file or could not be stat-ed.\n");
        close(error);
        return EXIT_FAILURE;
    }
    if (!error) {
        error = open(out, O_WRONLY | O_CREAT | O_EXCL);
        // Once you open a file, use fstat() on the file descriptor to ensure it is a regular file and not a symlink.
       if (fstat(error, &file_info) == -1 || S_ISLNK(file_info.st_mode)) {
            fprintf(stderr, "Error: File is a symlink, operation aborted.\n");
            close(error);
            return EXIT_FAILURE;
        }
        if (!error) {
            error = wait_confirmation(in, out);
            copy_file(in, out);
        }else {
            fprintf(stderr, "File %s cannot be written.\n", out);
        }
        
    } else {
        fprintf(stderr, "File %s cannot be read.\n", in);
    }

    return error;
}