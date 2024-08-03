#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dll_node_t
{
    int adress;
    void *data;
    struct dll_node_t *prev, *next;
    int data_size;
} dll_node_t;

typedef struct doubly_linked_list_t
{
    dll_node_t *head, *tail;
    unsigned int data_size;
    unsigned int size;
} doubly_linked_list_t;

doubly_linked_list_t *dll_create(unsigned int data_size);

dll_node_t *dll_get_nth_node(doubly_linked_list_t *list, unsigned int n);

void dll_add_nth_node(doubly_linked_list_t *list, unsigned int n, void *new_data);

void dll_create_nth_empty_node(doubly_linked_list_t *list, unsigned int n, int adress, int data_size);

void dll_add_nth_empty_node(doubly_linked_list_t *list, unsigned int n, dll_node_t *new_node);

dll_node_t *dll_remove_nth_node(doubly_linked_list_t *list, unsigned int n);

unsigned int dll_get_size(doubly_linked_list_t *list);

void dll_free(doubly_linked_list_t **pp_list);
