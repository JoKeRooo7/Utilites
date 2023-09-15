#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct all_flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int fail;
};

void check_flags(int argc, char **argv);
char *realization_flag_f(int *size_p, char **pattern, char *optarg,
                         struct all_flags *value);
void new_pattern(int argc, char **argv, char **pattern, int *lp,
                 struct all_flags value);
void massive_files(int argc, char **argv, char *pattern, int *lp,
                   struct all_flags value);
void open_file(char **files, int *sf, char *pattern, struct all_flags value);
void read_file(char *string_file, int *sf, char *pattern,
               struct all_flags value, FILE *f);
void newstr(struct all_flags value, char *string_file, char *string, int n,
            int *sf, char *pattern, regex_t buffer);
void printf_in_loop(struct all_flags value, char *string_file, char *string,
                    int n, int *sf);

int main(int argc, char **argv) {
  char *check_memmory = NULL;
  check_memmory = (char *)malloc(sizeof(char));
  if (check_memmory == NULL) {
    fprintf(stderr, "Memmory is not allocated");
  } else {
    free(check_memmory);
    if (argc > 2) {
      check_flags(argc, argv);
    } else {
      fprintf(stderr, "No such command");
    }
  }

  return 0;
}

void check_flags(int argc, char **argv) {
  char *pattern = NULL;
  pattern = (char *)malloc(sizeof(char));
  struct all_flags value = {0};
  int last_position = 1;
  int rezult = 0;
  int size_p = 0;
  while ((rezult = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (rezult) {
      case 'e': {
        if (size_p == 0) {  // grep -e -i pattern text.tc  value.e == 0
          value.e = 1;
          int z = strlen(optarg);
          size_p = z + 1;
          pattern = (char *)realloc(pattern, size_p * sizeof(char));
          strcpy(pattern, optarg);
        } else {
          int z = strlen(optarg);
          char c[2] = "|";
          size_p += z + 3;
          pattern = (char *)realloc(pattern, size_p * sizeof(char));
          strcat(pattern, c);
          strcat(pattern, optarg);
        }
        break;
      }
      case 'i':
        value.i = 1;
        break;
      case 'v':
        value.v = 1;
        break;
      case 'c':
        value.c = 1;
        break;
      case 'l':
        value.l = 1;
        break;
      case 'n':
        value.n = 1;
        break;
      case 'h':
        value.h = 1;
        break;
      case 's':
        value.s = 1;
        break;
      case 'f':
        value.f = 1;
        pattern = realization_flag_f(&size_p, &pattern, optarg, &value);
        break;
      case 'o':
        value.o = 1;
        break;
      default:
        value.fail = 1;
        break;
    }
  }
  if (value.fail == 0) {
    if (value.e == 0 && value.f == 0) {
      new_pattern(argc, argv, &pattern, &last_position, value);
    } else {
      massive_files(argc, argv, pattern, &last_position, value);
    }
  } else {
    fprintf(stderr, "no such commands");
  }
  free(pattern);
}

char *realization_flag_f(int *size_p, char **pattern, char *optarg,
                         struct all_flags *value) {
  FILE *f;
  if ((f = fopen(optarg, "r")) == NULL) {
    fprintf(stderr, "%s: no such file\n", optarg);
    value->fail = 1;
  } else {
    char *string = NULL;
    size_t size_string = 0;
    while (getline(&string, &size_string, f) != -1) {
      if (string[strlen(string) - 1] == '\n' && strlen(string) > 2) {
        string[strlen(string) - 1] = '\0';
      }
      if (*size_p != 0) {
        char c[2] = "|";
        *size_p += size_string + 5;
        *pattern = (char *)realloc(*pattern, *size_p * sizeof(char));
        strcat(*pattern, c);
        strcat(*pattern, string);
      } else {
        *size_p += size_string + 2;
        *pattern = (char *)realloc(*pattern, *size_p * sizeof(char));
        strcpy(*pattern, string);
      }
      *size_p += size_string + 2;
    }
    free(string);
  }
  fclose(f);
  return *pattern;
}

void new_pattern(int argc, char **argv, char **pattern, int *lp,
                 struct all_flags value) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-') {
      int size_p = strlen(*argv) + 1;
      *pattern = (char *)realloc(*pattern, size_p * sizeof(char));
      strcpy(*pattern, argv[i]);
      *lp = i + 1;
      break;
    }
  }
  massive_files(argc, argv, *pattern, lp, value);
}

void massive_files(int argc, char **argv, char *pattern, int *lp,
                   struct all_flags value) {
  char **files = NULL;
  int size_file = 0;
  files = (char **)malloc(sizeof(char *));
  for (int i = *lp; i < argc; i++) {
    if (strcmp("-e", argv[i]) == 0 || strcmp("-f", argv[i]) == 0) {
      i++;
    } else if (argv[i][0] != '-') {
      size_file++;
      files = (char **)realloc(files, (size_file) * sizeof(char *));
      files[size_file - 1] =
          (char *)malloc((strlen(argv[i]) + 1) * sizeof(char));
      if (files == NULL || files[size_file - 1] == NULL) {
        fprintf(stderr, "memory is over =(");
        size_file = -1;
        break;
      } else {
        strcpy(files[size_file - 1], argv[i]);
      }
    } else if (argv[i][strlen(argv[i]) - 1] == 'e' ||
               argv[i][strlen(argv[i]) - 1] == 'f') {
      i++;
    }
  }
  if (size_file != -1) {
    if (size_file == 0) {
      fprintf(stderr, "no such file or directory\n");
    } else {
      open_file(files, &size_file, pattern, value);
    }
  }
  for (int i = 0; i < size_file; i++) {
    free(files[i]);
  }
  free(files);
}

void open_file(char **files, int *sf, char *pattern, struct all_flags value) {
  FILE *f;
  for (int t = 0; t < *sf; t++) {
    if ((f = fopen(files[t], "r")) == NULL) {
      if (value.s == 0) {
        fprintf(stderr, "%s: no such file\n", files[t]);
      }
    } else {
      char *string_file = files[t];
      read_file(string_file, sf, pattern, value, f);  // string = files[t]
      fclose(f);
    }
  }
}

void read_file(char *string_file, int *sf, char *pattern,
               struct all_flags value, FILE *f) {
  char *string = NULL;
  size_t size_string;
  size_t nmatch = 1;
  regmatch_t pmatch[1];
  regex_t buffer;
  int temp, cflag;
  int flag_for_l = 0;
  int n = 1;  // counter for flag n
  int c = 0;
  if (value.i == 1) {
    cflag = REG_NEWLINE | REG_EXTENDED | REG_ICASE;
  } else {
    cflag = REG_NEWLINE | REG_EXTENDED;
  }
  if (regcomp(&buffer, pattern, cflag) == 0) {
    while (getline(&string, &size_string, f) != -1) {
      temp = regexec(&buffer, string, nmatch, pmatch, 0);
      if ((temp == 0 && value.v == 0) || (temp == 1 && value.v == 1)) {
        if ((value.c == 0 && value.l != 1) &&
            (value.o == 0 || (value.o == 1 && value.v == 1))) {
          printf_in_loop(value, string_file, string, n, sf);
        } else if ((value.c == 0 && value.l != 1) &&
                   (value.o == 1 && value.v == 0)) {
          newstr(value, string_file, string, n, sf, pattern, buffer);
        } else if (value.l == 1 && value.c == 1) {
          flag_for_l = 1;
          c++;
          break;
        } else {
          flag_for_l = 1;
          c++;
        }
        if (value.fail == 1) {
          break;
        }
      }
      n++;
    }
    if (value.c == 1) {
      if (*sf > 1 && value.h == 0) {
        printf("%s:%d\n", string_file, c);
      } else {
        printf("%d\n", c);
      }
    }
    if (value.l == 1) {
      if (flag_for_l == 1) {
        printf("%s\n", string_file);
      }
    }
  }
  regfree(&buffer);
  free(string);
}

void newstr(struct all_flags value, char *string_file, char *string, int n,
            int *sf, char *pattern, regex_t buffer) {
  int size_string = strlen(string);
  size_t nmatch = 1;
  regmatch_t pmatch[1];
  int temp;
  temp = regexec(&buffer, string, nmatch, pmatch, 0);
  if (temp == 0) {
    if (pmatch->rm_eo <= size_string) {
      char *new_string = NULL;
      int z = (pmatch->rm_eo - pmatch->rm_so);
      new_string = (char *)calloc(z + 1, sizeof(char));
      for (int i = pmatch->rm_so, j = 0; i < pmatch->rm_eo; i++, j++) {
        new_string[j] = string[i];
      }
      printf_in_loop(value, string_file, new_string, n, sf);
      free(new_string);
      value.n = 0;
      new_string = (char *)calloc(2, sizeof(char));
      if ((pmatch->rm_eo + 1) >= size_string) {
        free(new_string);
      } else {
        new_string[0] = ' ';
        new_string = (char *)realloc(
            new_string, (size_string - pmatch->rm_eo + 1) * sizeof(char));
        for (int i = pmatch->rm_eo + 1, j = 1; i <= size_string; i++, j++) {
          new_string[j] = string[i];
        }
        newstr(value, string_file, new_string, n, sf, pattern, buffer);
        free(new_string);
      }
    }
  }
}

void printf_in_loop(struct all_flags value, char *string_file, char *string,
                    int n, int *sf) {
  if (string[strlen(string) - 1] == '\n') {
    if (value.n == 1) {
      if (*sf > 1 && value.h == 0) {
        printf("%s:%d:%s", string_file, n, string);
      } else {
        printf("%d:%s", n, string);
      }
    } else {
      if (*sf > 1 && value.h == 0) {
        printf("%s:%s", string_file, string);
      } else {
        printf("%s", string);
      }
    }
  } else {
    if (value.n == 1) {
      if (*sf > 1 && value.h == 0) {
        printf("%s:%d:%s\n", string_file, n, string);
      } else {
        printf("%d:%s\n", n, string);
      }
    } else {
      if (*sf > 1 && value.h == 0) {
        printf("%s:%s\n", string_file, string);
      } else {
        printf("%s\n", string);
      }
    }
  }
}
