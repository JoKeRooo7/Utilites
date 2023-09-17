#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
  if (argc > 2) {
    GrepUtilityCheckFlags(argc, argv);
  } else {
    fprintf(stderr, "No such command");
  }

  return 0;
}

// Memory allocation One-dimensional array
int GrepUtilityMemoryAllocTDArray(char **str, size_t *size_str, size_t need_size) {
  int fail = 0;
  *str = NULL;
  *str = (char *)malloc(need_size * sizeof(char));
  *size_str = need_size;
  if (*str == NULL) {
    fprintf(stderr, "Memory was not allocated");
    fail = 1;
  }

  return fail;
}

// Memory Overexpression One-dimensional array
int GrepUtilityMemoryOverODArray(char **str, size_t *size_str, size_t need_size) {
  int fail = 0;
  char *new_str;
  fail = GrepMemoryAllocation(new_str, size_str, need_size);
  if (fail == 0) {
    memcpy(new_str, *str, *size_str);
    free(*str);
    *str = new_str;
    *size_str = need_size;
  }

  return fail;
}


void GrepUtilityCheckFlags(int argc, char **argv) {
  struct GrepDates data;
  data.pattern.size_p = 1;
  data.fail = 0;
  if (GrepUtilityMemoryAllocTDArray(&data.pattern.str_p, &data.pattern.size_p, 1)) {
    int res = 0;
    while ((res = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1 && data.fail == 0) {
      switch (res) {
        case 'f': GrepUtilityRealizationFlagF(&data); break;
        case 'e': GrepUtilityRealizationFlagE(&data); break;
        case 'i': data.mode.i = 1; break;
        case 'v': data.mode.v = 1; break;
        case 'c': data.mode.c = 1; break; 
        case 'l': data.mode.l = 1; break;
        case 'n': data.mode.n = 1; break;
        case 'h': data.mode.h = 1; break;
        case 's': data.mode.s = 1; break;
        case 'o': data.mode.o = 1; break;
        default: data.fail = 2; break;
      }
    }
    GrepUtilityRealization(argc, argv, data);
    free(data.pattern.str_p);
  }
}

// opening a file to take a pattern
void GrepUtilityOpenFileInFlagF(struct GrepDates *data) {
  FILE *file_stream;
  file_stream = NULL;
  file_stream = fopen(optarg, "r");
  if (file_stream == NULL) {
    fprintf(stderr, "%s: no such file\n", optarg);
    data->fail = 1
  } else {
    GrepUtilityRealizationInFlagF(file_stream, data);
    fclose(file_stream);
  }
  data -> mode.f = 1;
}

// Take pattern
void GrepUtilityRealizationInFlagF(FILE* file_stream, struct GrepDates *data) {
  char *str = NULL;
  size_t size_str = 0;
  while (getline(&str, &size_str, file_stream) != -1) {
    if (str[strlen(str) - 1] == '\n' && strlen(str) > 2) {
        str[strlen(str) - 1] = '\0';
    }
    size_t mode = (data -> pattern.size_p == 1) ? 2 : 5;
    size_t need_size = size_str + mode;
    if (GrepUtilityMemoryOverODArray(&data ->pattern.str_p, &data -> pattern.size_p, need_size)) {
      if (mode == 1) {
        strcpy(data ->pattern.str_p, str);
      } else {
        char sepparator[] = "|";
        strcat(data ->pattern.str_p, sepparator);
        strcat(data ->pattern.str_p, str);
      }
    }
  }
  if (str != NULL) {
    free(str)
  }
}

// Take pattern
void GrepUtilityRealizationFlagE(struct GrepDates *data) {
  data -> mode.e = 1;
  size_t mode = (data -> pattern.size_p == 1) ? 1 : 3;
  size_t need_size = strlen(optarg) + mode;
  if (GrepUtilityMemoryOverODArray(&data ->pattern.str_p, &data ->pattern.size_p, need_size)) {
    if (mode == 1) {
      strcpy(data ->pattern.str_p, optarg);
    } else {
      char sepparator[] = "|";
      strcat(data ->pattern.str_p, sepparator);
      strcat(data ->pattern.str_p, optarg);
    }
  } else {
    data -> fail = 1;
  }
}

void GrepUtilityRealization(int argc, char **argv, struct GrepDates *data) {
  if (data -> fail == 2) {
    fprintf(stderr, "no such commands");
  } else {
    if (data -> flags.size == 1) { // there were no <e> and <f> flags
      GrepUtilityTakingAPattern(argc, argv, data);
    } 
    if (data -> fail == 0) {
      GrepUtilityArrayFiles(argc, argv, data);
    }
  }
}

// Take pattern
void GrepUtilityTakingAPattern(int argc, char **argv, struct GrepDates *data) {
  size_t need_size = 0;
  for (size_t i = 1; i < argc && need_size == 0, ++i) {
    if (argv[i][0] != '-') {
      size_t new_size = strlen(*argv) + 1;
      if (GrepUtilityMemoryOverODArray(&data ->inp.pattern, &data -> inp.size, need_size)) {
        strcpy(*pattern, argv[i]);
        data -> last_pisition = i + 1;
      }
    }
  }
}

// Memory allocation to a two-dimensional files array
int GrepUtilityMemoryAllocTDFArray(char ***str, size_t *size_str, size_t need_size) {
  *str = NULL;
  *str = (char **)malloc(need_size, sizeof(char *));
  *size_str = need_size;
  int fail = 0;
  if (*str == NULL) {
    fprintf(stderr, "Memory was not allocated");
    fail = 1;
  }
  return fail;
}

/* Memory Overexpression to a two-dimensional files array
 * need_fs - we need to change the main size to this one (need first size)
 * need_ss - we need to change the size of the derived matrix to this (need second size)
 */
int GrepUtilityMemoryOverTDFArray(char ***str, size_t *size_str ,size_t need_fs, size_t need_ss) {
  int fail = 0;
  char **new_str = NULL;
  size_t new_size = 0;
  if (GrepUtilityMemoryAllocTDFArray(&new_str, &new_size, need_fs)) {
    for (size_t i = 0; i < *size_str; ++i) {
      memcpy(new_str[i], (*str)[i], strlen((*str)[i]));
    }
    GrepUtilityCleanTDFArray(str, size_str);
    *str = new_str;
    *size_str = new_size;
    size_t second_size = 0;
    fail = GrepUtilityMemoryAllocTDArray(&(*str)[*size_str - 1], &second_size, need_ss);
  } else {
    fail = 1;
  }
  return fail;
}

void GrepUtilityCleanTDFArray(char ***str, size_t *size_str) {
  for (size_t i = 0; i < *size_str; ++i) {
    free(*str[i]);
  }
  free(*str);
}

// getting an array of files
void GrepUtilityArrayFiles(int argc, char **argv, struct GrepDates *data) {
  if (GrepUtilityMemoryAllocTDFArray(&data -> files.str_f, &data -> files.size_f, 1)) {
    for (size_t i = data -> last_position; i < argc && data -> fail == 0; ++i) {
      if (strcmp("-e", argv[i]) == 0 || strcmp("-f", argv[i]) == 0) {
        ++i;
      } else if (argv[i][0] != '-') {
        data -> fail = GrepUtilityMemoryOverTDFArray(&data -> files.str_f, &data -> files.size_f, data -> files.size_f + 1, strlen(argv[i]) + 1);
        strcpy(data -> files.str_f[data -> files.size_f - 1], argv[i]);
      } else if (argv[i][strlen(argv[i]) - 1] == 'e' ||
                 argv[i][strlen(argv[i]) - 1] == 'f') {
        ++i;
      }
    }
    if (data -> fail == 0) {
      if (data -> files.size_f == 1) {
        fprintf(stderr, "no such file or directory\n");
      } else {
        GrepUtilityOpenFile(data);
      }
    }
    GrepUtilityCleanTDFArray(data -> files.str_f, data -> files.size_f);
  }
}

void GrepUtilityOpenFile(struct GrepDates *data) {
  FILE *file_stream = NULL;
  for (size_t i = 0; i < data -> files.size_f; ++i) {
    if ((file_stream = fopen(data -> files.str_f[i], "r")) == NULL) {
      if (data -> mode.s == 0) {
        fprintf(stderr, "%s: no such file\n", data -> files.str_f[i]);
      }
    } else {
      GrepUtilityReadFile(data, i);
      fclose(file_stream);
    }
  }
}

void GrepUtilityReadFile(struct GrepDates *data, int *i) {
  char string = NULL;
  size_t size_string = 0;
  // for regex
  size_t nmatch = 1;
  regmatch_t pmatch[1];
  regex_t buffer;
  int temp, regex_flag;

  if (data -> mode.i == 0) {
    regex_flag = REG_NEWLINE | REG_EXTENDED | REG_ICASE;
  } else {
    regex_flag = REG_NEWLINE | REG_EXTENDED;
  }
  if (regcomp(&buffer, data -> pattern.str_p, regex_flag) == 0) {

  }
  regfree(&buffer);
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


// void open_file(char **files, int *sf, char *pattern, struct all_flags value) {
//   FILE *f;
//   for (int t = 0; t < *sf; t++) {
//     if ((f = fopen(files[t], "r")) == NULL) {
//       if (value.s == 0) {
//         fprintf(stderr, "%s: no such file\n", files[t]);
//       }
//     } else {
//       char *string_file = files[t];
//       read_file(string_file, sf, pattern, value, f);  // string = files[t]
//       fclose(f);
//     }
//   }
// }


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
