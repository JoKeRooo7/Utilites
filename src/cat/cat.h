#ifndef UTILITES_C_LANGUAGE_CAT_CAT_H_
#define UTILITES_C_LANGUAGE_CAT_CAT_H_

struct CatFlags {
  int flag_b;
  int flag_e;
  int flag_n;
  int flag_s;
  int flag_t;
  int flag_v;
  int flag_fail;
};

struct CatReadFile {
  int flag_n; // flag_for_start_num
  int num; // numeration
  char symvol;
  char last_symvols[];
};

struct CatFlags CatUtilityCheckFlags(int argc, char **argv);
int CatUtilityUsingGetOpt(int argc, char **argv);
void CatUtilityOpenFile(int argc, char **argv, struct CatFlags *value);
void CatUtilityReadFile(struct CatFlags *value, FILE *files);
void CatUtilityAllPrint(struct CatFlags *value, struct CatReadFile *data);
void CatUtilityModeForNewLine(struct CatFlags *value, struct CatReadFile *data);
void CatUtilityPrintSymbol(struct CatReadFile *data, int *mode);
void CatUtilityPrintNumeration(struct CatReadFile *data);
int CatUtilityModeForTabs(struct CatFlags *value, struct CatReadFile *data);
int CatUtilityModeForNumeration(struct CatFlags *value, struct CatReadFile *data);


#endif // UTILITES_C_LANGUAGE_CAT_CAT_H_
