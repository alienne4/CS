#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef BUF_SIZE
#define BUF_SIZE 12
#endif

int bof(FILE *badfile) {
    char buffer[BUF_SIZE];
    // Buffer overflow vulnerability: no boundary check
    fread(buffer, sizeof(char), 300, badfile);
    return 1;
}

int main(int argc, char **argv) {
    FILE *badfile;
    char dummy[BUF_SIZE*5]; 
    memset(dummy, 0, BUF_SIZE*5);

    badfile = fopen("badfile", "r");
    bof(badfile);

    printf("Returned Properly\n");
    fclose(badfile);
    return 1;
}
