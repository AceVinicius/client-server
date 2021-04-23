/**
 * @file hash-table.c
 * @author Vinícius Aguiar (acevinicius AT icloud DOT com)
 * @brief 
 * @version 0.1
 * @date 2021-04-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../lib/include/murmur3.h"
#include "../../lib/include/allocation.h"
#include "../../lib/include/hash-table.h"
#include "../../lib/include/general.h"



static HASH_ITEM *     create_hash_item   ( const char *, const char * );
static HASH_ITEM *     search_hash_item   ( const HASH_TABLE *, const char * );
static void            destroy_hash_item  ( HASH_ITEM * );
static unsigned long   hash               ( const char * );

static LIST *  create_list   ( void );
static NODE *  create_node   ( void );
static void    insert_node   ( LIST *, HASH_ITEM * );
static NODE *  search_node   ( LIST *, const char * );
static void    delete_node   ( LIST *, const char * );
static void    destroy_node  ( NODE * );
static void    destroy_list  ( LIST * );



/******************************************************************************
 ***                           INTERNAL FUNCTIONS                           ***
 ******************************************************************************/



static HASH_ITEM *
create_hash_item( const char *key   ,
                  const char *value )
{
    HASH_ITEM *new_item = (HASH_ITEM *) allocate(0, sizeof(HASH_ITEM));

    new_item->key = strndup(key, MAX_DATA);
    new_item->data = strndup(value, MAX_DATA);

    return new_item;
}



static HASH_ITEM *
search_hash_item( const HASH_TABLE *table ,
                  const char       *key   )
{
    unsigned long index = hash(key);

    HASH_ITEM *item = table->items[ index ];

    if (!item)
    {
        return NULL;
    }

    if (cmp(item->key, key))
    {
        return item;
    }

    NODE *node = search_node(table->overflow[ index ], key);
    
    return (node)
        ? node->item
        : NULL;
}



static void
destroy_hash_item( HASH_ITEM *item )
{
    free_mem(item->key);
    free_mem(item->data);
    free_mem(item);
}



static unsigned long
hash( const char *key )
{
    unsigned long i[ 2 ];
    MurmurHash3_x64_128(key, strlen(key), 42, &i);

    return (i[ 0 ] + i[ 1 ]) % HASH_SIZE;
}



static LIST *
create_list( void )
{
    LIST *new_list = (LIST *) allocate(0, sizeof(LIST));

    new_list->head = NULL;
    new_list->count = 0;

    return new_list;
}



static NODE *
create_node( void )
{
    NODE *new_node = (NODE *) allocate(0, sizeof(NODE));

    new_node->item = NULL;
    new_node->next = NULL;

    return new_node;
}



static void
insert_node( LIST      *list ,
             HASH_ITEM *item )
{
    NODE *new_node = create_node();

    new_node->item = item;
    new_node->next = list->head;

    list->head = new_node;
    ++(list->count);
}



static NODE *
search_node(       LIST *list ,
             const char *key  )
{
    if (!list)
    {
        return NULL;
    }

    NODE *aux = list->head;
    while (aux && !cmp(aux->item->key, key))
    {
        aux = aux->next;        
    }

    return aux;
}



static void
delete_node(       LIST *list ,
             const char *key  )
{
    if (!list)
    {
        return;
    }

    NODE *curr = list->head;
    NODE *prev = NULL;
    
    while (curr)
    {
        if (cmp(curr->item->key, key))
        {
            if (!prev)
            {
                list->head = curr->next;
            }
            else
            {
                prev->next = curr->next;
            }
            
            destroy_node(curr);
            --(list->count);

            return;
        }
        prev = curr;
        curr = curr->next;
    }
}



static void
destroy_node( NODE *node )
{
    destroy_hash_item(node->item);
    free_mem(node);
}



static void
destroy_list( LIST *list )
{
    NODE *node = list->head;
    while (node)
    {
        NODE *tmp = node;
        node = node->next;
        destroy_node(tmp);
    }
    
    free_mem(list);
}



/******************************************************************************
 ***                           EXTERNAL FUNCTIONS                           ***
 ******************************************************************************/



HASH_TABLE *
create_hash_table( void )
{
    HASH_TABLE *new_table = (HASH_TABLE *) allocate(0, sizeof(HASH_TABLE));

    new_table->count    = 0;
    new_table->items    = (HASH_ITEM **) allocate(HASH_SIZE, sizeof(HASH_ITEM *));
    new_table->overflow = (LIST **)      allocate(HASH_SIZE, sizeof(LIST *));

    for (size_t i = 0; i < HASH_SIZE; ++i)
    {
        new_table->items[ i ]    = NULL;
        new_table->overflow[ i ] = NULL;
    }

    return new_table;
}



void
insert_hash_table(       HASH_TABLE *table ,
                   const char       *key   ,
                   const char       *value )
{
    if (!table)
    {
        fprintf(stderr, "insert_hash_table: NULL Pointer Given: table\n");
        exit(EXIT_FAILURE);
    }
    if (!key)
    {
        fprintf(stderr, "insert_hash_table: NULL Pointer Given: key\n");
        exit(EXIT_FAILURE);
    }
    if (!value)
    {
        fprintf(stderr, "insert_hash_table: NULL Pointer Given: value\n");
        exit(EXIT_FAILURE);
    }

    unsigned long index = hash(key);

    HASH_ITEM *item = search_hash_item(table, key);

    // No item found so let's create a new item
    if (!item)
    {
        HASH_ITEM *new_item = create_hash_item(key, value);
        ++(table->count);
        
        // Table at index empty
        if (!table->items[ index ])
        {
            table->items[ index ] = new_item;
        }
        // Table at index filled, resolve collision
        else
        {
            LIST *list = table->overflow[ index ];
            if (!list)
            {
                table->overflow[ index ] = create_list();
            }
            insert_node(table->overflow[ index ], new_item);
        }
    }
    // Item found at index, update data
    else
    {
        
    }

    return;
}



void *
search_hash_table( const HASH_TABLE *table ,
                   const char       *key   )
{
    if (!table)
    {
        fprintf(stderr, "search_hash_table: NULL Pointer Given: table\n");
        exit(EXIT_FAILURE);
    }
    if (!key)
    {
        fprintf(stderr, "search_hash_table: NULL Pointer Given: key\n");
        exit(EXIT_FAILURE);
    }

    HASH_ITEM *item = search_hash_item(table, key);

    return item->data;
}



void
delete_hash_table(       HASH_TABLE *table ,
                   const char       *key   )
{
    if (!table)
    {
        fprintf(stderr, "delete_hash_table: NULL Pointer Given: table\n");
        exit(EXIT_FAILURE);
    }
    if (!key)
    {
        fprintf(stderr, "delete_hash_table: NULL Pointer Given: key\n");
        exit(EXIT_FAILURE);
    }

    unsigned long index = hash(key);

    HASH_ITEM *item = table->items[ index ];
    
    // Item does not exist
    if (!item)
    {
        return;
    }

    LIST *list = table->overflow[ index ];
    
    // Item is only in table and no collision exists
    if (!list && cmp(item->key, key))
    {
        table->items[ index ] = NULL;
        --(table->count);
        destroy_hash_item(item);
    }
    else if (list)
    {   
        // Item is only in table and has collision exists
        if (cmp(item->key, key))
        {
            destroy_hash_item(item);
            table->items[ index ] = create_hash_item(list->head->item->key, list->head->item->data);
        }
        
        delete_node(list, list->head->item->key);

        if (list->count == 0)
        {
            destroy_list(list);
            table->overflow[ index ] = NULL;
        }
    }
}



void
destroy_hash_table( HASH_TABLE *table )
{
    if (!table)
    {
        fprintf(stderr, "destroy_hash_table: NULL Pointer Given: table\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < HASH_SIZE; ++i)
    {
        HASH_ITEM *item = table->items[ i ];
        if (item)
        {
            destroy_hash_item(item);
        }

        LIST *list = table->overflow[ i ];
        if (list)
        {
            destroy_list(list);
        }
    }

    free_mem(table->items);
    free_mem(table->overflow);
    free_mem(table);
}



void
print_hash_table( const HASH_TABLE *table )
{
    if (!table)
    {
        fprintf(stderr, "print_hash_table: NULL Pointer Given: table\n");
        exit(EXIT_FAILURE);
    }

    printf("\n\tHash Table\n-------------------\n");
    for (size_t i = 0; i < HASH_SIZE; ++i)
    {
        if (table->items[ i ])
        {
            printf("Index:%zu, Key:%s, Value:%s", i, table->items[ i ]->key, table->items[ i ]->data);

            if (table->overflow[ i ])
            {
                printf(" => Overflow %d => ", table->overflow[ i]->count);
                NODE *node = table->overflow[ i ]->head;
                fflush(stdout);
                while (node)
                {
                    printf("Key: %s, Value: %s ", node->item->key, node->item->data);
                    node = node->next;
                }
            }
            printf("\n");
        }
    }
    printf("-------------------\n\n");
}
