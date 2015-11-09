#ifndef EVAL_H_
#define EVAL_H_

#include "list.h"
#include "parse.h"

#define EMISMATCH -1
#define EOPERAND -2
#define EEMPTY -3
#define E0DIV -4
#define EUNKNOWNEXPR -5

int eval(long double *result, struct list_head *expr_item_list);

#endif
