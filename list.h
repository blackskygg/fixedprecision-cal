#ifndef LIST_HEAD_H_
#define LIST_HEAD_H_

struct list_head {
        struct list_head *prev, *next;
};

static inline void list_init(struct list_head *head)
{
        head->prev = head->next = head;
}

static inline void list_insert_back(struct list_head *head, struct list_head *elem)
{
        head->prev->next = elem;
        elem->prev = head->prev;
        elem->next = head;
        head->prev = elem;
}

static inline void list_insert_front(struct list_head *head, struct list_head *elem)
{
        head->next->prev = elem;
        elem->next = head->next;
        head->next = elem;
        elem->prev = head;
}

static inline void __list_delete(struct list_head *elem)
{
        elem->prev->next = elem->next;
        elem->next->prev = elem->prev;
}

#define list_is_empty(head_ptr) ((head_ptr)->next == (head_ptr))

static inline void list_iter(struct list_head *head, void (*func)(void*))
{
        while(!list_is_empty(head)) {
                func(head->next);
        }
}

static inline void list_move_back(struct list_head *head, struct list_head *elem)
{
        __list_delete(elem);
        list_insert_back(head, elem);
}

static inline void list_move_front(struct list_head *head, struct list_head *elem)
{
        __list_delete(elem);
        list_insert_front(head, elem);
}

#define __offset_of(type, member) ((size_t) &((type *)0)->member)

#define list_container_of(ptr, type, member) ({ \
                        size_t __offset = __offset_of(type, member); \
                        (type *)(((char *)ptr) - __offset);          \
                })

#define list_delete(ptr, type, member) ({ \
                        type *__container = list_container_of(ptr, type, member); \
                        __list_delete(ptr); \
                        free((void*)(__container)); \
                })

#endif
