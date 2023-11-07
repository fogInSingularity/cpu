#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif // #ifndef MIN

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif // #ifndef MAX

static const double Epsilon = 0.00000001;

#define IS_EQUAL(a, b) (abs(a - b) < Epsilon ? true : false)

#define USE_VAR(v) (void)(v);

#define CHECK_FILE(file, fileName, retVl) \
  if (file == nullptr) { \
    fprintf(stderr, RED BOLD "!  CANT OPEN FILE: %s\n" RESET, fileName); \
    fprintf(stderr, RED BOLD "!    %s(%d) %s\n" RESET, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    return retVl; \
  }

#define CHECK_BIT_FLAG(var, flag) ((var & (flag)) == (flag))

#endif // COMMON_DEFS_H
