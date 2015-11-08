#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "tokenize.h"

void print_token(void *data)
{
        struct token_t *token = list_container_of(data, struct token_t, list);
        printf("type %d, start: %d, end %d\n", token->type, token->start, token->end);
}

static inline void del_token(void *data)
{
        list_delete(data, struct token_t, list);
}

int main()
{
        struct list_head token_list;
        char s[1024];

        init_tokenizer();
        list_init(&token_list);
        while(1) {
                fgets(s, 1024, stdin);
                if(tokenize(s, &token_list) < 0) {
                        printf("error\n");
                        continue;
                }
                list_iter(&token_list, print_token);
                list_iter(&token_list, del_token);
        }

        return 0;
}
