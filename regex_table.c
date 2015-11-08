#define REGEX_TABLE_C

#include "stdio.h"
#include "stdlib.h"
#include "syntax.h"
#include "regex_table.h"
#include "regex.h"

#define ERR_BUFSIZE 1024

regex_t fa_table[NR_SYNTAX];
char* regex_table[NR_SYNTAX];

static void build_regex_table()
{
        regex_table[DEC_NUM_NR] = R_DEC_NUM;
        regex_table[HEX_NUM_NR] = R_HEX_NUM;
        regex_table[OCT_NUM_NR] = R_OCT_NUM;
        regex_table[BIN_NUM_NR] = R_BIN_NUM;

        regex_table[ADD_POS_NR] = R_ADD_POS;
        regex_table[SUB_NEG_NR] = R_SUB_NEG;
        regex_table[MUL_NR] = R_MUL;
        regex_table[DIV_NR] = R_DIV;
        regex_table[LEFT_BRACE_NR] = R_LEFT_BRACE;
        regex_table[RIGHT_BRACE_NR] = R_RIGHT_BRACE;
        regex_table[BLANK_NR] = R_BLANK;
}

void build_fa_table()
{
        char err_buf[ERR_BUFSIZE];
        int err_code;
        int i;

        build_regex_table();
        for(i = 0; i < NR_SYNTAX; ++i) {
                err_code = regcomp(&fa_table[i], regex_table[i], REG_EXTENDED|REG_ICASE);
                printf("%d: %s\n", i, regex_table[i]);
                if(err_code != 0) {
                        regerror(err_code, &fa_table[i], err_buf, ERR_BUFSIZE);
                        printf("error: %s\nwhile processing No.(%d) %s", err_buf, i, regex_table[i]);
                        exit(EXIT_FAILURE);
                }
        }
}
