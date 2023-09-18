#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    GrepUtilityCheckFlags(argc, argv);
  } else {
    fprintf(stderr, "No such command");
  }

  return 0;
}

// Memory allocation One-dimensional array
int GrepUtilityMemoryAllocODArray(char **str, size_t *size_str, size_t need_size) {
  int fail = 0;
  *str = NULL;
  *str = (char *)calloc(need_size, sizeof(char));
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
  fail = GrepUtilityMemoryAllocODArray(&new_str, size_str, need_size);
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
  if (!GrepUtilityMemoryAllocODArray(&data.pattern.str_p, &data.pattern.size_p, 0)) {
    int res = 0;
    while ((res = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1 && data.fail == 0) {
      switch (res) {
        case 'f': GrepUtilityOpenFileInFlagF(&data); break;
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
    GrepUtilityRealization(argc, argv, &data);
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
    data->fail = 1;
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
    if (!GrepUtilityMemoryOverODArray(&data ->pattern.str_p, &data -> pattern.size_p, need_size)) {
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
    free(str);
  }
}

// Take pattern
void GrepUtilityRealizationFlagE(struct GrepDates *data) {
  data -> mode.e = 1;
  size_t mode = (data -> pattern.size_p == 0) ? 1 : 3;
  size_t need_size = strlen(optarg) + mode;
  if (!GrepUtilityMemoryOverODArray(&data ->pattern.str_p, &data ->pattern.size_p, need_size)) {
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
    data -> last_position = 1;
    if (data -> pattern.size_p == 0) { // there were no <e> and <f> flags
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
  for (size_t i = 1; i < (size_t)argc && need_size == 0; ++i) {
    if (argv[i][0] != '-') {
      need_size =  strlen(*argv) + 1;
      if (!GrepUtilityMemoryOverODArray(&data ->pattern.str_p, &data -> pattern.size_p, need_size)) {
        strcpy(data ->pattern.str_p, argv[i]);
        data -> last_position = i + 1;
      }
    }
  }
}

// Memory allocation to a two-dimensional files array
int GrepUtilityMemoryAllocTDFArray(char ***str, size_t *size_str, size_t need_size) {
  *str = NULL;
  *str = (char **)calloc(need_size, sizeof(char *));
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
  if (!GrepUtilityMemoryAllocTDFArray(&new_str, &new_size, need_fs)) {
    size_t new_second_size = 0;
    for (size_t i = 0; i < *size_str && fail == 0; ++i) {
      if (GrepUtilityMemoryAllocODArray(&new_str[i], &new_second_size, strlen((*str)[i]))) {
        fail = 1;
      } else {
        strcpy(new_str[i], (*str)[i]);
      }
    }
    if (fail == 0) {
      GrepUtilityCleanTDFArray(str, size_str);
      *str = new_str;
      *size_str = new_size;
      size_t second_size = 0;
      fail = GrepUtilityMemoryAllocODArray(&(*str)[*size_str - 1], &second_size, need_ss);
    }
  } else {
    fail = 1;
  }
  return fail;
}

void GrepUtilityCleanTDFArray(char ***str, size_t *size_str) {
  for (size_t i = 0; i < *size_str; ++i) {
    free((*str)[i]);
  }
  free(*str);
}

// getting an array of files
void GrepUtilityArrayFiles(int argc, char **argv, struct GrepDates *data) {
  if (!GrepUtilityMemoryAllocTDFArray(&data -> files.str_f, &data -> files.size_f, 0)) {
    for (size_t i = data -> last_position; i < (size_t)argc && data -> fail == 0; ++i) {
      if (strcmp("-e", argv[i]) == 0 || strcmp("-f", argv[i]) == 0) {
        ++i;
      } else if (argv[i][0] != '-') {
          data -> fail = GrepUtilityMemoryOverTDFArray(&data -> files.str_f, &data -> files.size_f, data -> files.size_f + 1, strlen(argv[i]) + 1);
        if (data -> fail == 0) {
          strcpy(data -> files.str_f[data -> files.size_f - 1], argv[i]);
          }
      } else if (argv[i][strlen(argv[i]) - 1] == 'e' ||
                 argv[i][strlen(argv[i]) - 1] == 'f') {
        ++i;
      }
    }
    if (data -> fail == 0) {
      if (data -> files.size_f < 1) {
        fprintf(stderr, "no such file or directory\n");
      } else {
        GrepUtilityOpenFile(data);
      }
    }
    GrepUtilityCleanTDFArray(&data -> files.str_f, &data -> files.size_f);
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
      GrepUtilityReadFile(data, i, file_stream);
      fclose(file_stream);
    }
  }
}

// fs - file_stream
void GrepUtilityReadFile(struct GrepDates *data, int i, FILE *fs) {
  struct GrepRegex reg_data;
  if (data -> mode.i == 1) {
    reg_data.reg_flag = REG_NEWLINE | REG_EXTENDED | REG_ICASE;
  } else {
    reg_data.reg_flag = REG_NEWLINE | REG_EXTENDED;
  }
  if (regcomp(&reg_data.buffer, data -> pattern.str_p, reg_data.reg_flag) == 0) {
    GrepUtilityProccesReadFile(data, i, &reg_data, fs);
  }
  regfree(&reg_data.buffer);
}

void GrepUtilityProccesReadFile(struct GrepDates *data, int i, struct GrepRegex *reg_data, FILE *fs) {
  char *string = NULL;
  size_t size_string = 0;
  reg_data -> nmatch = 1;
  int mode_l = 0;
  int regex_res = 0;
  int numbering = 1;
  int count = 1;
  while (getline(&string, &size_string, fs) != -1) {
    regex_res = regexec(&reg_data -> buffer, string, reg_data -> nmatch, reg_data -> pmatch, 0);
    if ((regex_res == 0 && data -> mode.v == 0) || (regex_res = 1 && data -> mode.v == 1)) {
      if ((data -> mode.c == 0 && data -> mode.l != 1) && (data -> mode.o == 0 || (data -> mode.o == 1 && data -> mode.v == 1))) {
        GrepUtilityPrintDates(data, i, string, numbering);
      } else if ((data -> mode.c == 0 && data -> mode.l != 1) && (data -> mode.o == 1 && data -> mode.v == 0)) {
        data -> fail = GrepUtilityOptionO(data, i, reg_data, string, numbering);
      } else if (data -> mode.l == 1 && data -> mode.c == 1) {
        mode_l = 1;
        ++count;
        break;
      } else {
        mode_l = 1;
        ++count;
      }
      if (data -> fail != 0) {
        break;
      }
    }
    ++numbering;
  }
  if (data -> fail == 0 && data -> mode.c == 1) {
    if (data -> files.size_f > 1 && data -> mode.h == 0) {
      printf("%s:%d\n", data -> files.str_f[i], count);
    } else {
      printf("%d\n", count);
    }
  }
  if (data -> fail == 0 && data -> mode.l == 1) {
      if (mode_l == 1) {
        printf("%s\n", data -> files.str_f[i]);
      }
  }
  free(string);
}

void GrepUtilityPrintDates(struct GrepDates *data, int i, char *string, int num) {
  if (string[strlen(string) - 1] == '\n') {
    if (data -> mode.n == 1) {
      if (data -> files.size_f > 1 && data -> mode.h == 0) {
        printf("%s:%d:%s", data -> files.str_f[i], num, string);
      } else {
        printf("%d:%s", num, string);
      }
    } else {
      if (data -> files.size_f > 1 && data -> mode.h == 0) {
        printf("%s:%s", data -> files.str_f[i], string);
      } else {
        printf("%s", string);
      }
    }
  } else {
    if (data -> mode.n == 1) {
      if (data -> files.size_f > 1 && data -> mode.h == 0) {
        printf("%s:%d:%s\n", data -> files.str_f[i], num, string);
      } else {
        printf("%d:%s\n", num, string);
      }
    } else {
      if (data -> files.size_f > 1 && data -> mode.h == 0) {
        printf("%s:%s\n", data -> files.str_f[i], string);
      } else {
        printf("%s\n", string);
      }
    }
  }
}

int GrepUtilityOptionO(struct GrepDates *data, int i, struct GrepRegex *reg_data, char *string, int num) {
  size_t size_string = strlen(string);
  int new_res_reg;
  new_res_reg = regexec(&reg_data -> buffer, string, reg_data -> nmatch, reg_data -> pmatch, 0);
  if (new_res_reg == 0 && (size_t)reg_data -> pmatch -> rm_eo <= size_string) {
    char *new_string = NULL;
    size_t size_new_string = 0;
    size_t need_size = reg_data -> pmatch -> rm_eo - reg_data -> pmatch -> rm_so;
    if (!GrepUtilityMemoryAllocODArray(&new_string, &size_new_string, need_size)) {
      for (size_t i = reg_data -> pmatch -> rm_so, j = 0; i < (size_t)reg_data -> pmatch -> rm_eo; ++i, ++j) {
        new_string[j] = string[i];
      }
      GrepUtilityPrintDates(data, i, string, num);
      free(new_string);
      data -> mode.n = 0;
      size_t old_size = size_new_string;
      if (!GrepUtilityMemoryAllocODArray(&new_string, &size_new_string, 2)) {
        if ((size_t)reg_data -> pmatch -> rm_eo + 1 >= old_size) {
          free(new_string);
        } else {
          new_string[0] = ' ';
          need_size = old_size - reg_data -> pmatch -> rm_eo + 1;
          if (!GrepUtilityMemoryOverODArray(&new_string, &size_new_string, need_size)) {
            for (size_t i = reg_data -> pmatch -> rm_eo, j = 1; i <= old_size; ++i, ++j) {
              new_string[j] = string[i];
            }
            if (GrepUtilityOptionO(data, i, reg_data, new_string, num)) {
              data -> fail = 1;
            }
            free(new_string);
          } else {
            data -> fail = 1;
          }
        }
      } else {
        data -> fail = 1;
      }
    } else {
      data -> fail = 1;
    }
  }
  return data -> fail;
}

