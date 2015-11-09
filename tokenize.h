#ifndef TOKENIZE_H_
#define TOKENIZE_H_

#include "list.h"
#include "syntax.h"
#include "regex_table.h"

#define EUNKNOWN -1
#define ETOOLONG -2

#define MAX_TOKEN_SIZE 1024

enum token_type {
        TOKEN_UNKNOWN = UNKNOWN_NR,
        TOKEN_DEC = DEC_NUM_NR,
        TOKEN_HEX = HEX_NUM_NR,
        TOKEN_OCT = OCT_NUM_NR,
        TOKEN_BIN = BIN_NUM_NR,
        TOKEN_ADD_POS = ADD_POS_NR,
        TOKEN_SUB_NEG = SUB_NEG_NR,
        TOKEN_MUL = MUL_NR,
        TOKEN_DIV = DIV_NR,
        TOKEN_LB = LEFT_BRACE_NR,
        TOKEN_RB = RIGHT_BRACE_NR,
        TOKEN_BLANK = BLANK_NR,
};

struct token_t {
        struct list_head list;

        enum token_type type;
        size_t start;
        size_t end;
};

void init_tokenizer();
void destroy_tokenizer();
int tokenize(char* s, struct list_head *token_list);

#endif
