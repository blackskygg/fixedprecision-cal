#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "list.h"
#include "tokenize.h"
#include "parse.h"
#include "eval.h"

#define EXPRESSION_LEN 1024*1024

static char buffer[EXPRESSION_LEN];

static inline void del_item(void *data)
{
        list_delete(data, struct expr_item, list);
}

int main(int argc, char **argv)
{
        struct list_head token_list;
        struct list_head item_list;
        long double result;

        init_tokenizer();
        list_init(&token_list);
        list_init(&item_list);

        if(argc > 1) {
                while(--argc) {

                        argv++;

                        if(tokenize(*argv, &token_list) < 0) {
                                list_iter(&token_list, del_item);
                                continue;
                        }

                        if(parse(*argv, &token_list, &item_list) < 0) {
                                list_iter(&token_list, del_item);
                                list_iter(&item_list, del_item);
                                continue;
                        }

                        if(eval(&result, &item_list) <0) {
                                list_iter(&token_list, del_item);
                                list_iter(&item_list, del_item);
                                continue;
                        }

                        printf("%s = %llf\n", *argv, result);

                }

                goto EXIT;

        }

        for(;;) {
                printf(">>");

                fgets(buffer, sizeof(buffer), stdin);
                if(!strncasecmp(buffer, "exit", 4)) {
                        goto EXIT;
                }

                if(tokenize(buffer, &token_list) < 0) {
                        list_iter(&token_list, del_item);
                        continue;
                }

                if(parse(buffer, &token_list, &item_list) < 0) {
                        list_iter(&token_list, del_item);
                        list_iter(&item_list, del_item);
                        continue;
                }

                if(eval(&result, &item_list) <0) {
                        list_iter(&token_list, del_item);
                        list_iter(&item_list, del_item);
                        continue;
                }

                printf("= %llf\n", result);
        }

EXIT:
        list_iter(&token_list, del_item);
        list_iter(&item_list, del_item);
        destroy_tokenizer();

        return 0;
}
