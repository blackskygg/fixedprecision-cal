#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "eval.h"

#define err_exit(errno) ({                                      \
                        list_iter(&expression, del_expr_list);  \
                        list_iter(&eval_stack, del_expr_list);  \
                                                                \
                        eval_perror(errno);                     \
                        return errno;})                         \

#define do_opt_1(op, stack) ({                                          \
                        if(list_is_empty(stack))                        \
                                err_exit(EOPERAND);                     \
                                                                        \
                        struct expr_item *__item;                       \
                        __item = list_container_of((stack)->prev, struct expr_item, list); \
                        __item->val = op __item->val;})


#define do_opt_2(op, stack) ({                                          \
                        if(list_is_empty(stack))                        \
                                err_exit(EOPERAND);                     \
                                                                        \
                        struct expr_item *__item1;                      \
                        __item1 = list_container_of((stack)->prev, struct expr_item, list); \
                                                                        \
                        long double __operand2 = __item1->val;          \
                        list_delete((stack)->prev, struct expr_item, list); \
                                                                        \
                        if(list_is_empty(stack))                        \
                                err_exit(EOPERAND);                     \
                                                                        \
                        struct expr_item *__item2;                      \
                        __item2 = list_container_of((stack)->prev, struct expr_item, list); \
                        __item2->val = __item2->val op __item1->val;})


//global variables
static int priority[] = {
        0,  //EXPR_NULL
        0,  //EXPR_CONST
        1,  //EXPR_OP_ADD
        3,  //EXPR_OP_POS
        1,  //EXPR_OP_SUB
        3,  //EXPR_OP_NEG
        2,  //EXPR_OP_MUL
        2,  //EXPR_OP_DIV
};

static char *errmap[] = {
        "",
        "mismatched parentheses",
        "invalid number of operands",
        "empty expression",
        "0-division",
        "unknown evaluation error"
};

//local functions
static inline void eval_perror(int errno);
static inline void del_expr_list(void *list_member);

static inline void del_expr_list(void *list_member)
{
        list_delete((struct list_head *)list_member, struct expr_item, list);
}

int eval(long double *result, struct list_head *expr_item_list)
{
        struct list_head op_stack, eval_stack;
        struct list_head expression;
        struct list_head *curr, *__curr;
        struct expr_item *item_ptr, *top_item_ptr;

        list_init(&op_stack);
        list_init(&eval_stack);
        list_init(&expression);

        if(list_is_empty(expr_item_list)) {
                return EEMPTY;
        }
// construct the RPN
        while(!list_is_empty(expr_item_list)) {
                curr = expr_item_list->next;

                item_ptr = list_container_of(curr, struct expr_item, list);
                switch(item_ptr->type) {

                case EXPR_CONST:
                        list_move_back(&expression, curr);
                        break;

                case EXPR_OP_ADD:
                case EXPR_OP_POS:
                case EXPR_OP_SUB:
                case EXPR_OP_NEG:
                case EXPR_OP_MUL:
                case EXPR_OP_DIV:
                        top_item_ptr = list_container_of(op_stack.prev, struct expr_item, list);
                        if(list_is_empty(&op_stack)
                           || priority[top_item_ptr->type] < priority[item_ptr->type]) {
                                list_move_back(&op_stack, curr);
                        } else {
                                while(!list_is_empty(&op_stack)
                                      && priority[top_item_ptr->type] >= priority[item_ptr->type]) {
                                        list_move_back(&expression, &top_item_ptr->list);
                                        top_item_ptr = list_container_of(op_stack.prev, struct expr_item, list);
                                }
                                list_move_back(&op_stack, curr);
                        }
                        break;

                case EXPR_LB:
                        list_move_back(&op_stack, curr);
                        break;

                case EXPR_RB:
                        top_item_ptr = list_container_of(op_stack.prev, struct expr_item, list);
                        while(!list_is_empty(&op_stack)
                              && top_item_ptr->type != EXPR_LB) {
                                list_move_back(&expression, &top_item_ptr->list);
                                top_item_ptr = list_container_of(op_stack.prev, struct expr_item, list);
                        }

                        list_delete(curr, struct expr_item, list);
                        if(top_item_ptr->type == EXPR_LB) {
                                list_delete(&top_item_ptr->list, struct expr_item, list);
                        } else {
                                err_exit(EMISMATCH);
                        }
                        break;

                }
        }

        while(!list_is_empty(&op_stack)) {
                list_move_back(&expression, op_stack.prev);
        }

//real evaluation
        while(!list_is_empty(&expression)) {
                curr = expression.next;
                item_ptr = list_container_of(curr, struct expr_item, list);

                switch(item_ptr->type) {
                case EXPR_CONST:
                        list_move_back(&eval_stack, curr);
                        break;
                case EXPR_OP_ADD:
                        do_opt_2(+, &eval_stack);
                        list_delete(curr, struct expr_item, list);
                        break;
                case EXPR_OP_POS:
                        do_opt_1(+, &eval_stack);
                        list_delete(curr, struct expr_item, list);
                        break;
                case EXPR_OP_SUB:
                        do_opt_2(-, &eval_stack);
                        list_delete(curr, struct expr_item, list);
                        break;
                case EXPR_OP_NEG:
                        do_opt_1(-, &eval_stack);
                        list_delete(curr, struct expr_item, list);
                        break;
                case EXPR_OP_MUL:
                        do_opt_2(*, &eval_stack);
                        list_delete(curr, struct expr_item, list);
                        break;
                case EXPR_OP_DIV:
                        if(list_container_of(eval_stack.prev, struct expr_item, list)->val == 0)
                                err_exit(E0DIV);

                        do_opt_2(/, &eval_stack);
                        list_delete(curr, struct expr_item, list);
                        break;
                case EXPR_LB:
                case EXPR_RB:
                        err_exit(EMISMATCH);
                        break;

                }
        }

        if(list_is_empty(&eval_stack) || eval_stack.prev->prev != &eval_stack) {
                err_exit(EUNKNOWNEXPR);
        }

        item_ptr = list_container_of(eval_stack.prev, struct expr_item, list);
        *result = item_ptr->val;
        list_delete(eval_stack.prev, struct expr_item, list);

        return 0;
}

static inline void eval_perror(int errno)
{
        printf("eval error: %s\n", errmap[-errno]);
}
