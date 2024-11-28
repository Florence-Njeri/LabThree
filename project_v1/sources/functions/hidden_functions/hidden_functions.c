#include <assert.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>

#include "hidden_functions/hidden_functions.h"

int write_file(unsigned char hash[SHA256_DIGEST_LENGTH], const char * out) {
    FILE * fdout = fopen(out, "w");

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        fprintf(fdout, "%02x", hash[i]);
    }
    fprintf(fdout, "\n");

    fclose(fdout);

    return EXIT_SUCCESS;
}

int compute_confirmation(const char * in, unsigned char hash[SHA256_DIGEST_LENGTH]) {
    FILE * fdin = fopen(in, "r");

    const int error = fseek(fdin, 0L, SEEK_END);
    if (error) {
        perror("fseek");
        return -1;
    }

    const size_t size = ftell(fdin);
    rewind(fdin);

    unsigned char * file_content = (unsigned char *) malloc(sizeof(unsigned char) * size);
    const size_t    byte_read    = fread(file_content, sizeof(unsigned char), size, fdin);
    if (byte_read != size) {
        perror("fread");
        return -1;
    }

    fclose(fdin);

    SHA256(file_content, size, hash);

    free(file_content);

    return 0;
}
