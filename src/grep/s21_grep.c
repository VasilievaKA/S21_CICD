#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    s21_grep(argc, argv);
  } else {
    printf("Not enough arguments\n");
  }
  return 0;
}

void s21_grep(int argc, char **argv) {
  char patterns[4096] = {'\0'};
  char filenames[4096] = {'\0'};
  flags o;
  initOptionBash(&o);
  int fileCounter = 0;
  fileCounter = parserFlag(argc, argv, &o, patterns, filenames);
  int isPrint = 0;
  isPrint = changeFlags(&o);
  regex_t regex;
  int cflags = 0;
  int reti = 0;
  char *filename = NULL;
  if (o.i) {
    cflags = REG_EXTENDED | REG_ICASE;
  } else {
    cflags = REG_EXTENDED;
  }
  reti = regcomp(&regex, patterns, cflags);
  filename = strtok(filenames, "|");
  while (filename != NULL) {
    FILE *fp;
    regmatch_t match[1] = {0};
    char cc[4097] = {'\0'};
    int count = 0;
    if ((fp = fopen(filename, "r")) != NULL) {
      for (size_t i = 0; fgets(cc, 4096, fp); i++) {
        if (strchr(cc, '\n') == NULL)
          strcat(cc, "\n");
        if (o.o) {
          reti = regexec(&regex, cc, 1, match, 0);
        } else {
          reti = regexec(&regex, cc, 0, NULL, 0);
        }
        if (!reti) {
          if ((o.e || o.i == 1 || o.n || o.f || o.o) && o.v == 0 &&
              isPrint == 0) {
            printStr(fileCounter, filename, &o, i);
            if (o.o) {
              char *pstr = cc;
              int error = 0;
              while (!error) {
                if (match[0].rm_eo == match[0].rm_so)
                  break;
                printf("%.*s\n", (int)(match[0].rm_eo - match[0].rm_so),
                       pstr + match[0].rm_so);
                pstr += match[0].rm_eo;
                error = regexec(&regex, pstr, 1, match, REG_NOTBOL);
              }
            } else {
              fputs(cc, stdout);
            }
          }
          if (isPrint == 1)
            count++;
        } else if (reti == REG_NOMATCH) {
          if (o.v) {
            printStr(fileCounter, filename, &o, i);
            fputs(cc, stdout);
          }
          if (isPrint == -1)
            count++;
        }
        memset(cc, '\0', 4096);
      }
      fclose(fp);
      if (o.c) {
        if (fileCounter > 1 && !o.h)
          printFile(filename);
        if (o.l && count > 0)
          count = 1;
        printf("%d\n", count);
      }
      if ((o.l) && count > 0)
        printf("%s\n", filename);
    }
    filename = strtok(NULL, "|");
  }
  regfree(&regex);
}

void printStr(int fileCounter, char *filename, flags *o, int i) {
  if (fileCounter > 1 && !o->h)
    printFile(filename);
  if (o->n)
    flagN(i);
}

void flagN(int i) {
  char css[10];
  sprintf(css, "%d:", i + 1);
  fputs(css, stdout);
}

void printFile(char *filename) {
  fputs(filename, stdout);
  fputc(':', stdout);
}

int changeFlags(flags *o) {
  int isPrint = 0;
  if (o->v && o->i)
    o->i = 2;
  if (o->v && o->e)
    o->e = 0;
  if (o->v && o->o)
    o->o = 0;
  if (o->c || o->l) {
    if (o->v) {
      isPrint = -1;
      o->v = 0;
    } else {
      isPrint = 1;
      o->n = 0;
      if (o->i)
        o->i = 2;
      o->e = 0;
    }
  }
  if (o->o == 2) {
    o->o = 0;
    o->e = 1;
  }
  if (o->c + o->e + o->f + o->i + o->l + o->n + o->o + o->v == 0)
    o->e = 1;
  if (o->e && o->o)
    o->e = 0;
  if (o->i + o->h + o->s + o->n == 1 && o->o) {
    o->o = 1;
    o->e = 0;
  }
  return isPrint;
}

int parserFlag(int argc, char **argv, flags *o, char *pat, char *filenames) {
  int i = 1;
  int e_counter = 0, file_counter = 0, combinationStatus = 0;
  while (i < argc) {
    combinationStatus = combinationOrFlag(argv[i]);
    if (combinationStatus) {
      char *arg = argv[i];
      arg++;
      while (*arg != 0) {
        if (*arg == 'e' && combinationStatus) {
          o->e = 1;
          if (e_counter > 0)
            strcat(pat, "|");
          if (!argv[i + 1])
            exit(1);
          i++;
          if (!strcmp(argv[i], "\0"))
            argv[i] = "\n";
          strcat(pat, argv[i]);
          e_counter++;
        } else if (*arg == 'l' && combinationStatus) {
          o->l = 1;
        } else if (*arg == 'n' && combinationStatus) {
          o->n = 1;
        } else if (*arg == 'c' && combinationStatus) {
          o->c = 1;
        } else if (*arg == 'i' && combinationStatus) {
          o->i = 1;
        } else if (*arg == 'v' && combinationStatus) {
          o->v = 1;
        } else if (*arg == 'f' && combinationStatus) {
          o->f = 1;
          char regfile[1024];
          char *file = regfile;
          file = argv[i + 1];
          FILE *f;
          char str[4097] = {'\0'};
          char *cc = str;
          if ((f = fopen(file, "r")) != NULL) {
            for (size_t j = 0; fgets(cc, 4096, f); j++) {
              if (e_counter > 0)
                strcat(pat, "|");
              if (strchr(cc, '\n'))
                *(cc + strlen(cc) - 1) = '\0';
              if (!strcmp(cc, "\0"))
                cc = "\n";
              strcat(pat, cc);
              e_counter++;
              memset(cc, '\0', 4096);
            }
            fclose(f);
          } else {
            messegeError(file);
            exit(1);
          }
          i++;
        } else if (*arg == 'h' && combinationStatus) {
          o->h = 1;
        } else if (*arg == 's' && combinationStatus) {
          o->s = 1;
        } else if (*arg == 'o' && combinationStatus) {
          o->o++;
        }
        arg++;
      }
    } else if (e_counter == 0 && !findEFlag(i, argc, argv) &&
               !findFFlag(i, argc, argv)) {
      if (!strcmp(argv[i], "\0"))
        argv[i] = "\n";
      strcat(pat, argv[i]);
      e_counter++;
    } else {
      FILE *f;
      if ((f = fopen(argv[i], "r")) != NULL) {
        fclose(f);
        if (file_counter > 0)
          strcat(filenames, "|");
        strcat(filenames, argv[i]);
        file_counter++;
      } else {
        if (!o->s)
          messegeError(argv[i]);
        file_counter++;
      }
    }
    i++;
  }
  return file_counter;
}

int combinationOrFlag(char *str) {
  int check = 0, len = strlen(str);
  if (*str == '-' && (len == 2 || len == 3)) {
    str++;
    len--;
    while (*str != 0) {
      switch (*str) {
      case 'e':
      case 'f':
      case 'n':
      case 'c':
      case 'l':
      case 'i':
      case 'h':
      case 's':
      case 'o':
      case 'v':
        check++;
        break;
      }
      str++;
    }
    if ((check == 2 && len == 2) || (check == 1 && len == 1)) {
      check = 1;
    } else {
      check = 0;
    }
  }
  return check;
}

void messegeError(char *filename) {
  fprintf(stderr, "grep: %s: No such file or directory\n", filename);
}

int findEFlag(int i, int argc, char **argv) {
  int countE = 0;
  while (++i < argc) {
    if (combinationOrFlag(argv[i])) {
      char *str = argv[i];
      while (*str != 0) {
        if (*str == 'e')
          countE++;
        str++;
      }
    }
  }
  return countE;
}

int findFFlag(int i, int argc, char **argv) {
  int countF = 0;
  while (++i < argc) {
    if (combinationOrFlag(argv[i])) {
      char *str = argv[i];
      while (*str != 0) {
        if (*str == 'f')
          countF++;
        str++;
      }
    }
  }
  return countF;
}

void initOptionBash(flags *buff) {
  buff->e = 0;
  buff->i = 0;
  buff->v = 0;
  buff->c = 0;
  buff->l = 0;
  buff->n = 0;
  buff->f = 0;
  buff->o = 0;
  buff->s = 0;
  buff->h = 0;
}
