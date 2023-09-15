#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct all_flags {
  int flag_b;
  int flag_e;
  int flag_n;
  int flag_s;
  int flag_t;
  int flag_v;
  int flag_fail;
};

struct all_flags check_flags(int argc, char **argv);
void open_file(int argc, char **argv, struct all_flags value);
void read_file(int *flag_for_start_num, int *numeration, struct all_flags value,
               FILE *files);
void all_print(int *flag_for_start_num, int *numeration, struct all_flags value,
               char c, char *c2);
void print_numeration(int *numeration);

int main(int argc, char *argv[]) {
  struct all_flags value;
  value = check_flags(argc, argv);
  if (value.flag_fail == 1) {
    fprintf(stderr, "Error: comman no found");
  } else {
    if (value.flag_b == 1 && value.flag_n == 1) {
      value.flag_n = 0;
    }
    open_file(argc, argv, value);
  }
  return 0;
}

struct all_flags check_flags(int argc, char **argv) {
  struct all_flags num = {0};
  struct option long_flags[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                {"squeeze-blank", no_argument, NULL, 's'},
                                {"number", no_argument, NULL, 'n'},
                                {NULL, 0, NULL, 0}};
  int rez = 0;
  int option_index;
  while ((rez = getopt_long(argc, argv, "+beEnsvtT", long_flags,
                            &option_index)) != -1) {
    switch (rez) {
      case 'b':
        num.flag_b = 1;
        break;
      case 'e':
        num.flag_e = 1;
        num.flag_v = 1;
        break;
      case 'E':
        num.flag_e = 1;
        break;
      case 'n':
        num.flag_n = 1;
        break;
      case 's':
        num.flag_s = 1;
        break;
      case 't':
        num.flag_t = 1;
        num.flag_v = 1;
        break;
      case 'T':
        num.flag_t = 1;
        break;
      case 'v':
        num.flag_v = 1;
        break;
      case '?':
        num.flag_fail = 1;
        break;
    }
  }
  return num;
}

void open_file(int argc, char **argv, struct all_flags value) {
  FILE *files;
  for (int i = 1; i < argc; i++) {
    int flag_for_start_num = 0;
    int numeration = 1;
    if (argv[i][0] != '-') {
      if ((files = fopen(argv[i], "r")) == NULL) {
        fprintf(stderr, "%s: no such file\n", argv[i]);
      } else {
        read_file(&flag_for_start_num, &numeration, value, files);
      }
    }
  }
  fclose(files);
}

void read_file(int *flag_for_start_num, int *numeration, struct all_flags value,
               FILE *files) {
  char c;
  char c2[3] = {' ', ' ', '\n'};
  while ((c = getc(files)) != EOF) {
    c2[0] = c2[1];
    c2[1] = c2[2];
    c2[2] = c;
    if ((c2[0] != '\n' || c2[1] != '\n') ||
        (c2[2] != '\n' || value.flag_s != 1)) {
      all_print(flag_for_start_num, numeration, value, c, c2);
    }
  }
}

void all_print(int *flag_for_start_num, int *numeration, struct all_flags value,
               char c, char *c2) {
  int flag = 0;
  if (*flag_for_start_num == 0) {
    if (value.flag_b == 1) {
      if (c2[2] != '\n' && c2[1] == '\n') {
        flag = 1;
      }
    } else if (value.flag_n == 1) {
      flag = 1;
    }
  }
  if (flag == 1) {
    print_numeration(numeration);
  }
  if (c == '\n') {
    *flag_for_start_num = 0;
    if (value.flag_e == 1) {
      printf("$");
    }
  } else {
    *flag_for_start_num = 1;
  }
  if (c == '\t' && (value.flag_t == 1)) {
    printf("^I");
    flag = 2;
  }
  if (value.flag_v == 1 && (c != '\t' && c != '\n')) {
    if ((c < 32 && c != 9) && c != 10) {
      printf("^%c", c + 64);
      flag = 2;
    } else if (c == 127) {
      printf("^%c", c - 64);
      flag = 2;
    }
  }
  if (flag != 2) {
    printf("%c", c);
  }
}

void print_numeration(int *numeration) {
  printf("%6d\t", *numeration);
  *numeration += 1;
}
