#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/*
 * Function that needs to be called to allocate and initialize a list.
 * (Set initial values for the fields specific to the list structure).
 */
doubly_linked_list_t *
dll_create(unsigned int data_size)
{
    doubly_linked_list_t *t;
    t = malloc(sizeof(doubly_linked_list_t));
    t->head = NULL;
    t->tail = NULL;
    t->data_size = data_size;
    t->size = 0;
    return t;
}

/*
 * Function that returns a pointer to the node at position n in the list.
 * Positions in the list are indexed starting from 0 (i.e., the first node in the
 * list is at position n=0). If n >= number_of_nodes, then it returns the node at the
 * position obtained by "cycling" (possibly multiple times) through the list and
 * wrapping from the last node back to the first, continuing from there. How can
 * we find the desired position without simulating the entire traversal?
 * Note: n>=0
 */
dll_node_t *
dll_get_nth_node(doubly_linked_list_t *list, unsigned int n)
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

/*
 * Based on the data sent through the new_data pointer, a new node is created and
 * added at position n of the list represented by the list pointer. Positions in the
 * list are indexed starting from 0 (i.e., the first node in the list is at
 * position n=0). When indexing positions, we do not "cycle" through the circular
 * list as in get, but consider nodes in the order from head to the last (i.e., the node
 * that points to head as the next node in the list). If n >= number_of_nodes, then
 * we add the new node at the end of the list.
 * Note: n>=0
 */
void dll_add_nth_node(doubly_linked_list_t *list, unsigned int n, void *new_data)
{
    dll_node_t *new_node, *curent;
    curent = list->head;
    new_node = malloc(sizeof(dll_node_t));
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
        dll_node_t *p;
        p = list->head;
        p->prev = NULL;
        p = list->tail;
        p->next = NULL;
        new_node->next = list->head;
        list->head = new_node;
        new_node->prev = NULL;
        list->size++;
        return;
    }

    if (n >= list->size)
    {
        unsigned int nr = 0;
        while (nr < list->size - 1)
        {
            curent = curent->next;
            nr++;
        }
        curent->next = new_node;
        new_node->next = NULL;
        new_node->prev = curent;
        list->tail = new_node;
        list->size++;
        return;
    }

    dll_node_t *t, *dt;
    t = dll_get_nth_node(list, n - 1);
    dt = dll_get_nth_node(list, n);
    t->next = new_node;
    new_node->next = dt;
    dt->prev = new_node;
    new_node->prev = t;
    list->size++;
}

// Function that adds an empty node to a list

void dll_create_nth_empty_node(doubly_linked_list_t *list, unsigned int n, int adress, int data_size)
{
    dll_node_t *new_node, *curent;
    curent = list->head;
    new_node = malloc(sizeof(dll_node_t));
    new_node->adress = adress;
    new_node->data_size = data_size;

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
        dll_node_t *p;
        p = list->head;
        p->prev = NULL;
        p = list->tail;
        p->next = NULL;
        new_node->next = list->head;
        list->head = new_node;
        new_node->prev = NULL;
        list->size++;
        return;
    }

    if (n >= list->size)
    {
        unsigned int nr = 0;
        while (curent->next != NULL)
        {
            curent = curent->next;
        }
        curent->next = new_node;
        new_node->next = NULL;
        new_node->prev = curent;
        list->tail = new_node;
        list->size++;
        return;
    }

    dll_node_t *t, *dt;
    t = dll_get_nth_node(list, n - 1);
    dt = dll_get_nth_node(list, n);
    t->next = new_node;
    new_node->next = dt;
    dt->prev = new_node;
    new_node->prev = t;
    list->size++;
}

void dll_add_nth_empty_node(doubly_linked_list_t *list, unsigned int n, dll_node_t *new_node)
{
    dll_node_t *curent;
    curent = list->head;

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
        dll_node_t *p;
        p = list->head;
        p->prev = NULL;
        p = list->tail;
        p->next = NULL;
        new_node->next = list->head;
        list->head = new_node;
        new_node->prev = NULL;
        list->size++;
        return;
    }

    if (n >= list->size)
    {
        unsigned int nr = 0;
        while (curent->next != NULL)
        {
            curent = curent->next;
        }
        curent->next = new_node;
        new_node->next = NULL;
        new_node->prev = curent;
        list->tail = new_node;
        list->size++;
        return;
    }

    dll_node_t *t, *dt;
    t = dll_get_nth_node(list, n - 1);
    dt = dll_get_nth_node(list, n);
    t->next = new_node;
    new_node->next = dt;
    dt->prev = new_node;
    new_node->prev = t;
    list->size++;
}

/*
 * Removes the node at position n from the list whose pointer is sent as a parameter.
 * Positions in the list are indexed from 0 (i.e., the first node in the list is at
 * position n=0). The function returns a pointer to this freshly removed node from the list.
 * If n >= number_of_nodes - 1, the node at the end of the list is removed.
 * It is the caller's responsibility to free the memory of this node.
 * Note: n>=0
 */
dll_node_t *
dll_remove_nth_node(doubly_linked_list_t *list, unsigned int n)
{
    dll_node_t *p, *pp;
    p = list->head;
    if (list->size == 1)
    {
        list->head = NULL;
        list->tail = NULL;
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
    if (n >= list->size)
    {
        p = list->tail;
        list->tail = p->prev;
        (list->tail)->next = NULL;
        list->size--;
        return p;
    }
    dll_node_t *r;
    r = dll_get_nth_node(list, n);
    p = dll_get_nth_node(list, n - 1);
    pp = dll_get_nth_node(list, n + 1);
    p->next = pp;
    pp->prev = p;
    list->size--;
    return r;
}

/*
 * The function returns the number of nodes in the list whose pointer is sent as a parameter.
 */
unsigned int
dll_get_size(doubly_linked_list_t *list)
{
    return list->size;
}

/*
 * Procedure frees the memory used by all nodes in the list, and finally,
 * frees the memory used by the list structure.
 */
void dll_free(doubly_linked_list_t **pp_list)
{
    dll_node_t *p, *pp;
    p = (*pp_list)->head;
    while (p != NULL)
    {
        pp = p;
        p = p->next;
        free(pp);
    }
    free(*pp_list);
}
