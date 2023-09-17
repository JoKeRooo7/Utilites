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
    int last_position;  // reading position
    int fail;
};


#endif // UTILITES_C_LANGUAGE_GREP_GREP_H_
