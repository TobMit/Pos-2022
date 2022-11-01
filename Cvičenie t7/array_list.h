
#ifndef POS_2022_ARRAY_LIST_H
#define POS_2022_ARRAY_LIST_H

#include "date.h"

typedef struct array_list {
    int capacity;
    int size;
    struct date* array;
} ARRAY_LIST;

void array_list_init(ARRAY_LIST* list); // musí inicializovať arrayList
void array_list_dispose(ARRAY_LIST* list);
void array_list_add(ARRAY_LIST* list, const DATE* date);

void array_list_print(const ARRAY_LIST* list);


#endif //POS_2022_ARRAY_LIST_H
