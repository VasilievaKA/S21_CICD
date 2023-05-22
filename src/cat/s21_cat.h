#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_
#include <stdio.h>
#include <getopt.h>
typedef struct flags {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
} fl;
static struct option long_fl[] = {
        {"number-nonblank" , 0, 0, 'b'},
        {"number", 0, 0, 'n'},
        {"squeeze-blank", 0, 0, 's'},
        {"T", 0, 0, 't'},
        {"E", 0, 0, 'e'},
        {0, 0, 0, 0}
};
void parser(int argc, char *argv[], fl *flags);
void reader(char *argv[], fl *flags);
#endif  //  SRC_CAT_S21_CAT_H_
