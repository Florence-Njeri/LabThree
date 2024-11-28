#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int secure_copy_file(const char * in,
                     const char * out);

int parse_options(int            argc,
                  char * const * argv,
                  char ** __restrict in,
                  char ** __restrict out);

#endif // FUNCTIONS_H