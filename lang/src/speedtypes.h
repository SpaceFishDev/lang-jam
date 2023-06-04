#ifndef SPEED_TYPES
#define SPEED_TYPES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define llat(type, list, index) \
    ((type *)(linked_list_at(index, list)->data))
typedef struct list_node
{
    void *data;
    struct list_node *next;
} list_node;

typedef struct
{
    list_node *head;
    list_node *tail;
    size_t length;
} linked_list;

list_node *initialize_node(void *data)
{
    list_node *n = malloc(sizeof(list_node));
    n->data = data;
    n->next = (void *)0;
    return n;
}

void append_node(linked_list *list, list_node *data)
{
    if (list->head == (void *)0)
    {
        list->head = data;
        list->tail = data;
        list->length = 1;
        return;
    }
    ++list->length;
    list->tail->next = data;
    list->tail = data;
}
list_node *linked_list_at(size_t index, linked_list *list)
{
    if (index > list->length)
    {
        printf("Index '%ld' higher than length of list\n", index);
        exit(-1);
    }
    list_node *n = list->head;
    for (int i = 0; i != index; ++i)
    {
        n = n->next;
    }
    return n;
}

typedef struct
{
    void* value;
    char *key;
} hash_map_element;

typedef struct
{
    size_t *element_indices;
    linked_list elements;
    size_t count;
    size_t element_max;
} hash_map;

hash_map_element *init_element(void* value, char *key)
{
    hash_map_element *h = malloc(sizeof(hash_map_element));
    *h = (hash_map_element){value, key};
    return h;
}

size_t hash_string(char *string);

hash_map *init_hash_map()
{
    hash_map h = (hash_map){malloc(sizeof(size_t)), (linked_list){0}, 0, 0};
    hash_map *x = malloc(sizeof(hash_map));
    *x = h;
    return x;
}

void add_element(hash_map *hash, hash_map_element *element)
{
    append_node(&hash->elements, initialize_node(element));
    size_t index_el = hash->count++;
    size_t index = hash_string(element->key);
    if (index > hash->element_max)
    {
        hash->element_indices = realloc(hash->element_indices, index * sizeof(size_t) + sizeof(size_t));
        hash->element_max = index;
    }
    hash->element_indices[index] = index_el;
}

hash_map_element *get_element(hash_map *hash, char *key)
{
    size_t hash_key = hash_string(key);
    hash_map_element *element = linked_list_at(hash->element_indices[hash_key], &hash->elements)->data;
    return element;
}

void* get_value(hash_map *hash, char *key)
{
    return get_element(hash, key)->value;
}

size_t hash_string(char *string)
{
    char *cpy = string;
    size_t total = 0;
    while (*string)
    {
        total += *string;
        ++string;
    }
    total *= (size_t)strlen(cpy);
    total = total + strlen(cpy) | (cpy[0] * strlen(cpy));
    return total;
}

#endif
