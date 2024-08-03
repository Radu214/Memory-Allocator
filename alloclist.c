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
alloc_dll_create()
{
    alloc_doubly_linked_list_t *t;
    t = malloc(sizeof(alloc_doubly_linked_list_t));
    t->head = NULL;
    t->tail = NULL;
    t->size = 0;
    return t;
}

/*
 * Function that returns a pointer to the node with address <adress> in the list.
 */
dll_node_t *
alloc_dll_get_node_adress(alloc_doubly_linked_list_t *list, int adress)
{
    dll_node_t *curent;
    curent = list->head;
    while (curent->adress != adress)
    {
        curent = curent->next;
    }
    return curent;
}

/*
 * Function that adds the node new_node given as a parameter to the list <list>
 */
dll_node_t *
alloc_dll_get_nth_node(alloc_doubly_linked_list_t *list, unsigned int n)
{
    if (n > list->size)
        n = n % list->size;
    dll_node_t *curent;
    curent = list->head;
    unsigned int i = 0;
    while (i < n)
    {
        i++;
        curent = curent->next;
    }
    return curent;
}

void alloc_dll_add_nth_node(alloc_doubly_linked_list_t *list, unsigned int n, dll_node_t *new_node)
{
    dll_node_t *curent;
    curent = list->head;
    new_node->data = malloc(new_node->data_size);

    if (list->size == 0)
    {
        new_node->next = NULL;
        list->head = new_node;
        new_node->prev = NULL;
        list->tail = new_node;
        list->size++;
        return;
    }

    if (n == 0)
    {
        new_node->next = list->head;
        new_node->prev = NULL;
        (list->head)->prev = new_node;
        list->head = new_node;
        list->size++;
        return;
    }

    if (n >= list->size)
    {
        list->tail->next = new_node;
        new_node->prev = list->tail;
        new_node->next = NULL;
        list->tail = new_node;
        list->size++;
        return;
    }
    dll_node_t *t, *dt;
    t = alloc_dll_get_nth_node(list, n - 1);
    dt = alloc_dll_get_nth_node(list, n);
    t->next = new_node;
    new_node->next = dt;
    dt->prev = new_node;
    new_node->prev = t;
    list->size++;
}

/*
 * Removes the node at position n from the list whose pointer is sent as a parameter.
 * Positions in the list are indexed from 0 (i.e., the first node in the list is at position n=0).
 * The function returns a pointer to this freshly removed node from the list.
 * If n >= number of nodes - 1, the node at the end of the list is removed.
 * It is the caller's responsibility to free the memory of this node.
 * Note: n>=0
 */
dll_node_t *
alloc_dll_remove_nth_node(alloc_doubly_linked_list_t *list, unsigned int n)
{
    dll_node_t *p, *pp;
    p = list->head;
    if (list->size == 1)
    {
        list->head = NULL;
        list->size = 0;
        return p;
    }
    if (n == 0)
    {
        list->head = (list->head)->next;
        (list->head)->prev = NULL;
        list->size--;
        return p;
    }
    if (n >= list->size - 1)
    {
        p = list->tail;
        list->tail = p->prev;
        (list->tail)->next = NULL;
        p->prev = NULL;
        list->size--;
        return p;
    }
    dll_node_t *r;
    r = alloc_dll_get_nth_node(list, n);
    p = alloc_dll_get_nth_node(list, n - 1);
    pp = alloc_dll_get_nth_node(list, n + 1);
    p->next = pp;
    pp->prev = p;
    list->size--;
    return r;
}

/*
 * The function returns the number of nodes in the list whose pointer is sent as a parameter.
 */
unsigned int
alloc_dll_get_size(alloc_doubly_linked_list_t *list)
{
    return list->size;
}

/*
 * Procedure frees the memory used by all nodes in the list, and finally,
 * frees the memory used by the list structure.
 */
void alloc_dll_free(alloc_doubly_linked_list_t **pp_list)
{
    dll_node_t *p, *pp;
    p = (*pp_list)->head;
    while (p != NULL)
    {
        pp = p;
        p = p->next;
        free(pp->data);
        free(pp);
    }
    free(*pp_list);
}
