#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

#include "cat.h"

int main(int argc, char *argv[]) {
  struct CatFlags value = CatUtilityCheckFlags(argc, argv);

  if (value.flag_fail == 1) {
    fprintf(stderr, "Error: command no found");
  } else {
    CatUtilityOpenFile(argc, argv, &value);
  }

  return 0;
}

struct CatFlags CatUtilityCheckFlags(int argc, char **argv) {
  struct CatFlags num = {0};
  int result = 0;
  while ((result = CatUtilityUsingGetOpt(argc, argv)) != -1) {
    switch (result) {
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
  if (num.flag_b == 1 && num.flag_n == 1) {
      num.flag_n = 0;
  }
  return num;
}

int CatUtilityUsingGetOpt(int argc, char **argv) {
  int option_index;
  struct option long_flags[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {"number", no_argument, NULL, 'n'},
      {NULL, 0, NULL, 0}};
    (void)long_flags;
  return getopt_long(argc, argv, "+beEnsvtT", long_flags, &option_index);
}

void CatUtilityOpenFile(int argc, char **argv, struct CatFlags *value) {
  FILE *files;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {  // skip the flags
        continue;
    }
    if ((files = fopen(argv[i], "r")) == NULL) {
       fprintf(stderr, "%s: no such file\n", argv[i]);
    } else {
       CatUtilityReadFile(value, files);
    }
  }
  fclose(files);
}

void CatUtilityReadFile(struct CatFlags *value, FILE *files) {
  struct CatReadFile data;
  data.last_symvols[0] = ' ';
  data.last_symvols[1] = ' ';
  data.last_symvols[2] = '\n';
  data.flag_n = 0;
  data.num = 1;
  
  while ((data.symvol = getc(files)) != EOF) {
    data.last_symvols[0] = data.last_symvols[1];
    data.last_symvols[1] = data.last_symvols[2];
    data.last_symvols[2] = data.symvol;
    if ((data.last_symvols[0] != '\n' || data.last_symvols[1] != '\n') ||
        (data.last_symvols[2] != '\n' || value -> flag_s != 1)) {
      CatUtilityAllPrint(value, &data);
    }
  }
}



void CatUtilityAllPrint(struct CatFlags *value, struct CatReadFile *data) {
  int mode = 0;
  if (data -> flag_n == 0) {
    mode = CatUtilityModeForNumeration(value, data);
  }
  if (mode == 1) {
    CatUtilityPrintNumeration(data);
  }
  CatUtilityModeForNewLine(value, data);
  if (CatUtilityModeForTabs(value, data) == 2) {
    mode = 2;
  } 
  if (value -> flag_v == 1 && (data -> symvol != '\t' && data -> symvol != '\n')) {
    CatUtilityPrintSymbol(data, &mode);
  }
  if (mode != 2) {
    printf("%c", data -> symvol);
  }
}

int CatUtilityModeForNumeration(struct CatFlags *value, struct CatReadFile *data) {
  int mode = 0;
  if (value -> flag_b == 1) {
    if (data -> last_symvols[2] != '\n' && data -> last_symvols[1] == '\n') {
      mode = 1;
    }
  } else if (value -> flag_n == 1) {
   mode = 1;
  }
  return mode;
}

void CatUtilityPrintNumeration(struct CatReadFile *data) {
  printf("%6d\t", data -> num);
  data -> num += 1;
}

void CatUtilityModeForNewLine(struct CatFlags *value, struct CatReadFile *data) {
  if (data -> symvol == '\n') {
    data -> flag_n = 0;
    if (value -> flag_e == 1) {
      printf("$");
    }
  } else {
    data -> flag_n = 1;
  }
}

int CatUtilityModeForTabs(struct CatFlags *value, struct CatReadFile *data) {
  int mode = 0;
  if (data -> symvol == '\t' && (value -> flag_t == 1)) {
    printf("^I");
    mode = 2;
  }
  return mode;
}

void CatUtilityPrintSymbol(struct CatReadFile *data, int *mode) {
    if ((data -> symvol < 32 && data -> symvol != 9) && data -> symvol!= 10) {
      printf("^%c", data -> symvol + 64);
      *mode = 2;
    } else if (data -> symvol == 127) {
      printf("^%c", data -> symvol - 64);
      *mode = 2;
    }
}