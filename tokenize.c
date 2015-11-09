#include <stdlib.h>
#include <stdio.h>
#include "regex_table.h"
#include "tokenize.h"

static char* errmap[] = {
        "",
        "unknown token",
        "token too long"
};

static void tokenizer_error(int errno, char *s);
static int get_next_token(char* s, struct token_t* token);

static int get_next_token(char* s, struct token_t* token)
{
        regmatch_t pmatch[1];
        enum token_type token_nr;
        int i, len;

        len = 0;
        token_nr = TOKEN_UNKNOWN;
        for(i = 0; i < NR_SYNTAX; ++i) {
                if(0 == regexec(&fa_table[i], s, 1, pmatch, 0)) {
                        if(pmatch[0].rm_eo - pmatch[0].rm_so > len) {
                                len = pmatch[0].rm_eo - pmatch[0].rm_so;
                                token_nr = (enum token_type) i;
                                token->type = token_nr;
                        }
                }
        }

        if(token_nr == TOKEN_UNKNOWN) {
                return EUNKNOWN;
        } else if(len > MAX_TOKEN_SIZE) {
                return ETOOLONG;
        } else {
                return len;
        }

}

int tokenize(char* s, struct list_head *token_list)
{
        char *pos = s;
        int token_len;
        struct token_t *token_ptr;

        while(*pos != '\0') {
                token_ptr = malloc(sizeof(struct token_t));

                token_len = get_next_token(pos, token_ptr);
                if(token_len < 0) {
                        free(token_ptr);
                        tokenizer_error(token_len, pos);
                        return token_len;
                } else {
                        token_ptr->start = pos - s;
                        token_ptr->end = token_ptr->start + token_len - 1;
                        list_insert_back(token_list, &token_ptr->list);

                        pos += token_len;
                }
        }

        return 0;
}

inline void init_tokenizer()
{
        build_fa_table();
}

static void tokenizer_error(int errno, char *s)
{
        printf("tokenizer error: %s\n"
               "near position %s\n", errmap[-errno], s);
}
