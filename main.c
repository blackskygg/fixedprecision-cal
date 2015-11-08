#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "tokenize.h"
#include "parse.h"

void print_expr_item(void *data)
{
        struct expr_item *item = list_container_of(data, struct expr_item, list);
        printf("type %d, %llf\n", item->type, item->val);
}

static inline void del_item(void *data)
{
        list_delete(data, struct expr_item, list);
}

int main()
{
        struct list_head token_list;
        struct list_head item_list;
        char s[1024];

        init_tokenizer();
        list_init(&token_list);
        list_init(&item_list);
        while(1) {
                fgets(s, 1024, stdin);
                if(tokenize(s, &token_list) < 0) {
                        printf("error\n");
                        continue;
                }
                parse(s, &token_list, &item_list);

                list_iter(&item_list, print_expr_item);
                list_iter(&item_list, del_item);
        }

        return 0;
}
