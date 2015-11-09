#ifndef PARSE_H_
#define PARSE_H_

#include "list.h"
#include "tokenize.h"

#define EPRECISION -1
#define EPARSEMEMORY -2

enum expr_item_type {
        EXPR_NULL = 0,
        EXPR_CONST,
        EXPR_OP_ADD,
        EXPR_OP_POS,
        EXPR_OP_SUB,
        EXPR_OP_NEG,
        EXPR_OP_MUL,
        EXPR_OP_DIV,
        EXPR_RB,
        EXPR_LB,
};

struct expr_item {
        struct list_head list;

        enum expr_item_type type;
        long double val;
};

int parse(char *s, struct list_head* token_list, struct list_head* expr_item_list);

#endif

