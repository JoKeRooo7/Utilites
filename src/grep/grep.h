#ifndef UTILITES_C_LANGUAGE_GREP_GREP_H_
#define UTILITES_C_LANGUAGE_GREP_GREP_H_


struct GrepFlags {
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
};

struct GrepPattern {
    // string pattern
    char *str_p; // this is the pattern we will be looking for
    size_t size_p;
};

struct GrepFiles {
    // string files
    char **str_f; // this is the pattern we will be looking for
    size_t size_f;
};

struct GrepDates {
    struct GrepFlags mode;
    struct GrepPattern pattern; //the introduced pattern
    struct GrepFiles files;
    size_t last_position;  // reading position
    int fail;
};

struct GrepRegex {
    size_t nmatch;
    regmatch_t pmatch[1];
    regex_t buffer;
    int reg_flag;
};

int GrepUtilityMemoryAllocODArray(char **str, size_t *size_str, size_t need_size);
int GrepUtilityMemoryOverODArray(char **str, size_t *size_str, size_t need_size);
void GrepUtilityCheckFlags(int argc, char **argv);
void GrepUtilityOpenFileInFlagF(struct GrepDates *data);
void GrepUtilityRealizationInFlagF(FILE* file_stream, struct GrepDates *data);
void GrepUtilityRealizationFlagE(struct GrepDates *data);
void GrepUtilityRealization(int argc, char **argv, struct GrepDates *data);
void GrepUtilityTakingAPattern(int argc, char **argv, struct GrepDates *data);
int GrepUtilityMemoryAllocTDFArray(char ***str, size_t *size_str, size_t need_size);
int GrepUtilityMemoryOverTDFArray(char ***str, size_t *size_str ,size_t need_fs, size_t need_ss);
void GrepUtilityCleanTDFArray(char ***str, size_t *size_str);
void GrepUtilityArrayFiles(int argc, char **argv, struct GrepDates *data);
void GrepUtilityOpenFile(struct GrepDates *data);
void GrepUtilityReadFile(struct GrepDates *data, int i, FILE *fs);
void GrepUtilityProccesReadFile(struct GrepDates *data, int i, struct GrepRegex *reg_data, FILE *fs);
void GrepUtilityPrintDates(struct GrepDates *data, int i, char *string, int num);
int GrepUtilityOptionO(struct GrepDates *data, int i, struct GrepRegex *reg_data, char *string, int num);


#endif // UTILITES_C_LANGUAGE_GREP_GREP_H_
