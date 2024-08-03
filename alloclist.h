#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct alloc_dll_node_t
{
    int adress;
    void *data;
    struct alloc_dll_node_t *prev, *next;
    int data_size;
} alloc_dll_node_t;

typedef struct alloc_doubly_linked_list_t
{
    dll_node_t *head, *tail;
    unsigned int size;
} alloc_doubly_linked_list_t;

/*
 * Function that needs to be called to allocate and initialize a list.
 * (Set initial values for the fields specific to the list structure).
 */
alloc_doubly_linked_list_t *
alloc_dll_create();
dll_node_t *
alloc_dll_get_node_adress(alloc_doubly_linked_list_t *list, int adress);
dll_node_t *
alloc_dll_get_nth_node(alloc_doubly_linked_list_t *list, unsigned int n);

void alloc_dll_add_nth_node(alloc_doubly_linked_list_t *list, unsigned int n, dll_node_t *new_node);

dll_node_t *alloc_dll_remove_nth_node(alloc_doubly_linked_list_t *list, unsigned int n);

unsigned int alloc_dll_get_size(alloc_doubly_linked_list_t *list);

void alloc_dll_free(alloc_doubly_linked_list_t **pp_list);
