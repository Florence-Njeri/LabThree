#ifndef HIDDEN_FUNCTIONS_H
#define HIDDEN_FUNCTIONS_H

int write_file(unsigned char hash[32], const char * out);

int compute_confirmation(const char * in, unsigned char hash[32]);

#endif // HIDDEN_FUNCTIONS_H