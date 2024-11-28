#include <assert.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <unistd.h>

#include "hidden_functions/hidden_functions.h"

int copy_file(const char * in, const char * out) {
    FILE * fds[2] = {NULL, NULL};

    fds[0] = fopen(in, "r");
    fds[1] = fopen(out, "w");

    int running = 1;

    while (running) {
        unsigned char buffer[2048];
        const size_t  count = fread(buffer, sizeof(unsigned char), 2048, fds[0]);
        assert(count > 0);
        const size_t writn = fwrite(buffer, sizeof(unsigned char), count, fds[1]);

        running = !feof(fds[0]) && !ferror(fds[0]) && (writn == count);
    }

    fclose(fds[0]);
    fclose(fds[1]);

    return EXIT_SUCCESS;
}

int wait_confirmation(const char * in, const char * out) {
    printf("You are about to copy file %s in %s. Are you sure ? (y/N)\n", in,
           out);

    struct pollfd fds = {.fd = STDIN_FILENO, POLLIN, 0};

    const int test = poll(&fds, 1, (int) 1e3 * 3);
    if (test < 0) {
        perror("poll");
        return -1;
    } else if (test == 0) {
        fprintf(stderr, "Timeout.\n");
        return 3;
    }

    const unsigned char uc   = (unsigned char) getchar();
    const unsigned char valy = (uc ^ 'y') + (uc ^ 'Y');

    const int ok = (valy == 32);

    return (int) (ok != 1);
}
