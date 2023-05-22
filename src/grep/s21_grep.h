#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
typedef struct flags {
    int i;
    int e;
    int v;
    int c;
    int l;
    int n;
    int h;
    int o;
    int s;
    int f;
}flags;
void s21_grep(int argc, char **argv);
void printStr(int fileCounter, char *filename, flags *o, int i);
void flagN(int i);
void printFile(char *filename);
int changeFlags(flags *o);
int parserFlag(int argc, char **argv, flags *o, char *pat, char *filenames);
int combinationOrFlag(char *str);
void messegeError(char *filename);
int findEFlag(int i, int argc, char **argv);
int findFFlag(int i, int argc, char **argv);
void initOptionBash(flags *buff);
#endif  //  SRC_GREP_S21_GREP_H_
