#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "parse.h"

#define exit_err(errno, ptr) ({                         \
                        parser_perror(errno);           \
                        NULL != ptr ? free(ptr) : 0;    \
                        return errno;})

static char* errmap[] = {
        "",
        "out of precision",
        "out of memory",
};

static void parser_perror(int errno);
static int getval(char *begin, char *end, long double *val, int base);

static int getval(char *begin, char *end, long double *val, int base)
{
        char *pos_dot;
        char val_str[MAX_TOKEN_SIZE + 1];

        if(end - begin + 1> LDBL_DIG / log10(base))
        {
                return EPRECISION;
        }

        pos_dot = begin;
        while(pos_dot != end + 1 && *pos_dot != '.')
                ++pos_dot;

        memcpy(val_str, begin, pos_dot - begin);
        val_str[pos_dot - begin] = '\0';

        *val = strtol(begin, NULL, base);

        if(pos_dot < end) {
                size_t dec_len = end - pos_dot;
                long double factor = 1;

                memcpy(val_str, pos_dot + 1, dec_len);
                val_str[dec_len] = '\0';

                while(dec_len--)
                        factor /= base;
                *val += factor * strtol(val_str, NULL, base);
        }

}

int parse(char *s, struct list_head* token_list, struct list_head* expr_item_list)
{
        struct list_head *curr;
        struct token_t *token;
        struct expr_item *item;
        enum expr_item_type prev_valid_item;

        prev_valid_item = EXPR_NULL;
        while(!list_is_empty(token_list)) {
                curr = token_list->next;
                token = list_container_of(curr, struct token_t, list);
                if( NULL == (item = malloc(sizeof(struct expr_item))) ) {
                        exit_err(EPARSEMEMORY, NULL);
                }

                switch(token->type) {
                case TOKEN_DEC:
                        prev_valid_item = item->type = EXPR_CONST;
                        if (EPRECISION ==  getval(s+token->start, s+token->end, &item->val, 10)) {
                                fwrite(s, token->start - token->end, 1, stdout);
                                exit_err(EPRECISION, item);
                        }
                        list_insert_back(expr_item_list, &item->list);
                        break;

                case TOKEN_HEX:
                        prev_valid_item = item->type = EXPR_CONST;
                        if (EPRECISION ==  getval(s+token->start + 2, s+token->end, &item->val, 16)) {
                                fwrite(s, token->start - token->end, 1, stdout);
                                exit_err(EPRECISION, item);
                        }
                        list_insert_back(expr_item_list, &item->list);
                        break;

                case TOKEN_OCT:
                        prev_valid_item = item->type = EXPR_CONST;
                        if (EPRECISION ==  getval(s+token->start + 1, s+token->end, &item->val, 8)) {
                                fwrite(s, token->start - token->end, 1, stdout);
                                exit_err(EPRECISION, item);
                        }
                        list_insert_back(expr_item_list, &item->list);
                        break;

                case TOKEN_BIN:
                        prev_valid_item = item->type = EXPR_CONST;
                        if (EPRECISION ==  getval(s+token->start, s+token->end - 1, &item->val, 2)) {
                                fwrite(s, token->start - token->end, 1, stdout);
                                exit_err(EPRECISION, item);
                        }
                        list_insert_back(expr_item_list, &item->list);
                        break;

                case TOKEN_ADD_POS:
                        if(prev_valid_item == EXPR_CONST
                           || prev_valid_item == EXPR_RB) {
                                prev_valid_item = item->type = EXPR_OP_ADD;
                        } else {
                                prev_valid_item = item->type = EXPR_OP_POS;
                        }
                        list_insert_back(expr_item_list, &item->list);
                        break;

                case TOKEN_SUB_NEG:
                        if(prev_valid_item == EXPR_CONST
                           || prev_valid_item == EXPR_RB) {
                                prev_valid_item = item->type = EXPR_OP_SUB;
                        } else {
                                prev_valid_item = item->type = EXPR_OP_NEG;
                        }
                        list_insert_back(expr_item_list, &item->list);
                        break;

                case TOKEN_MUL:
                        prev_valid_item = item->type = EXPR_OP_MUL;
                        list_insert_back(expr_item_list, &item->list);
                        break;

                case TOKEN_DIV:
                        prev_valid_item = item->type = EXPR_OP_DIV;
                        list_insert_back(expr_item_list, &item->list);
                        break;

                case TOKEN_LB:
                        prev_valid_item = item->type = EXPR_LB;
                        list_insert_back(expr_item_list, &item->list);
                        break;

                case TOKEN_RB:
                        prev_valid_item = item->type = EXPR_RB;
                        list_insert_back(expr_item_list, &item->list);
                        break;

                case TOKEN_BLANK:
                        free(item);
                        break;

                }

                list_delete(curr, struct token_t, list);
        }

        return 0;
}

static void parser_perror(int errno)
{
        printf("tokenizer error: %s\n", errmap[-errno]);

}
