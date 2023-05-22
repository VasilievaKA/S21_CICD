#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    fl flags = {0};
    parser(argc, &argv[0], &flags);
    if (flags.b) {
      flags.n = 0;
    }
    reader(argv, &flags);
  } else {
    printf("Something went wrong");
  }
  return 0;
}

void parser(int argc, char *argv[], fl *flags) {
  int opt;
  int index_fl = 0;
  while ((opt = getopt_long(argc, argv, "+benstvTE", long_fl, &index_fl)) !=
         -1) {
    switch (opt) {
    case 98:
      flags->b = 1;
      break;
    case 101:
      flags->e = 1;
      flags->v = 1;
      break;
    case 110:
      flags->n = 1;
      break;
    case 115:
      flags->s = 1;
      break;
    case 116:
      flags->t = 1;
      flags->v = 1;
      break;
    case 118:
      flags->v = 1;
      break;
    case 84:
      flags->t = 1;
      break;
    case 69:
      flags->e = 1;
      break;
    default:
      fprintf(stderr, "\nusage: benstvTE\n");
    }
  }
}

void reader(char *argv[], fl *flags) {
  FILE *fp;
  if ((fp = fopen(argv[optind], "r")) != NULL) {
    int current;
    int str_count = 0;
    while ((current = fgetc(fp)) != EOF) {
      if (flags->b) {
        if (current != '\n') {
          str_count += 1;
          printf("%6d\t", str_count);
        }
      }
      if (flags->n) {
        str_count += 1;
        printf("%6d\t", str_count);
      }
      int flag = 0;
      int for_print;
      while ((for_print = fgetc(fp)) != EOF) {
        if (flags->s) {
          if (current == '\n' && for_print == '\n') {
            while (for_print == '\n') {
              for_print = fgetc(fp);
              flag = 1;
            }
          }
        }
        if (flags->e || flags->t || flags->v) {
          if (current >= 0 && current < 32 && current != '\n') {
            if ((flags->e || flags->v) && (current + 64 != 'J') &&
                (current + 64 != 'I')) {
              printf("^%c", current + 64);
            } else if (flags->t) {
              printf("^%c", current + 64);
            } else {
              printf("%c", current);
            }
          } else {
            printf("%c", current);
          }
          if (flags->e) {
            if (for_print == '\n') {
              printf("$");
            }
          }
        } else {
          if (flag) {
            printf("\n%c", current);
            flag = 0;
          } else {
            printf("%c", current);
          }
        }
        if (flags->b) {
          if (current == '\n' && for_print != current) {
            str_count += 1;
            printf("%6d\t", str_count);
          }
        }
        if (flags->n) {
          if (current == '\n') {
            str_count += 1;
            printf("%6d\t", str_count);
          }
        }
        current = for_print;
      }
      if (current != EOF) {
        if (flags->e || flags->t) {
          if (current >= 0 && current < 32 && current != '\n') {
            if (flags->e && current + 64 != 'J' && current + 64 != 'I') {
              printf("^%c", current + 64);
            } else if (flags->t) {
              printf("^%c", current + 64);
            } else {
              printf("%c", current);
            }
          } else {
            printf("%c", current);
          }
          if (flags->e) {
            if (for_print == '\n') {
              printf("$");
            }
          }
        } else {
          if (flag) {
            printf("\n%c", current);
            flag = 0;
          } else {
            printf("%c", current);
          }
        }
      }
    }
    fclose(fp);
  } else {
    printf("No such file");
  }
}
