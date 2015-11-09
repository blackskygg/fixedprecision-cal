#ifndef REGEX_TABLE_H_
#define REGEX_TABLE_H_

#include <regex.h>

#define UNKNOWN_NR -1
#define DEC_NUM_NR 0
#define HEX_NUM_NR 1
#define OCT_NUM_NR 2
#define BIN_NUM_NR 3

#define ADD_POS_NR 4
#define SUB_NEG_NR 5
#define MUL_NR 6
#define DIV_NR 7
#define LEFT_BRACE_NR 8
#define RIGHT_BRACE_NR 9
#define BLANK_NR 10

#ifdef REGEX_TABLE_C
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN regex_t fa_table[];

void build_fa_table();
void destroy_fa_table();

#endif
