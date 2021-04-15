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


static HASH_ITEM *      create_hash_item    ( const char *, const char * );
static unsigned long    hash                ( const char * );
static void             destroy_hash_item   ( HASH_ITEM * );



/******************************************************************************
 ***                           INTERNAL FUNCTIONS                           ***
 ******************************************************************************/



static HASH_ITEM *
create_hash_item( const char *key   ,
                  const char *value )
{
    HASH_ITEM *new_item = (HASH_ITEM *) allocate(0, sizeof(HASH_ITEM));

    size_t key_len   = strlen(key);
    new_item->key   = allocate(key_len  , sizeof(char));
    memcpy(new_item->key  , key  , key_len+1);

    size_t value_len = strlen(value);
    new_item->data = allocate(value_len, sizeof(char));
    memcpy(new_item->data, value, value_len+1);

    return new_item;
}



static unsigned long
hash( const char *key )
{
    unsigned long i[2];
    MurmurHash3_x64_128(key, strlen(key), 42, &i);
    printf("%d %d\n", i[0], i[1]);
    return i[0] % HASH_SIZE;
}



static void
destroy_hash_item( HASH_ITEM *item )
{
    free_mem(item->key);
    free_mem(item->data);
    free_mem(item);
}



/******************************************************************************
 ***                           EXTERNAL FUNCTIONS                           ***
 ******************************************************************************/



HASH_TABLE *
create_hash_table( void )
{
    HASH_TABLE *new_table = (HASH_TABLE *) allocate(0, sizeof(HASH_TABLE));

    new_table->count = 0;

    new_table->items = (HASH_ITEM **) allocate(HASH_SIZE, sizeof(HASH_ITEM *));
    for (size_t i = 0; i < HASH_SIZE; ++i)
    {
        new_table->items[ i ] = NULL;
    }

    return new_table;
}



void
insert_hash_table( HASH_TABLE *table ,
                   char       *key   ,
                   char       *value )
{
    if (table == NULL)
    {
        fprintf(stderr, "insert_hash_table: NULL Pointer Given: table\n");
        exit(EXIT_FAILURE);
    }
    if (key == NULL)
    {
        fprintf(stderr, "insert_hash_table: NULL Pointer Given: key\n");
        exit(EXIT_FAILURE);
    }
    if (value == NULL)
    {
        fprintf(stderr, "insert_hash_table: NULL Pointer Given: value\n");
        exit(EXIT_FAILURE);
    }

    unsigned long index = hash(key);

    HASH_ITEM *item     = table->items[ index ];
    HASH_ITEM *new_item = create_hash_item(key, value);

    // No match
    if (item == NULL)
    {
        table->items[ index ] = new_item;
        ++(table->count);

        return;
    }

    // Update value
    if (cmp(item->key, key))
    {
        free_mem(item->data);

        size_t value_len = strlen(value);
        new_item->data = allocate(value_len, sizeof(char));
        memcpy(new_item->data, value, value_len+1);

        return;
    }

    // Colision
    // handle_collision();

    return;
}



void *
search_hash_table( HASH_TABLE *table ,
                   const char *key   )
{
    HASH_ITEM *item = table->items[ hash(key) ];

    if (item != NULL)
    {
        return item->data;
    }

    return NULL;
}



void
destroy_hash_table( HASH_TABLE *hash )
{
    for (size_t i = 0; i < HASH_SIZE; ++i)
    {
        HASH_ITEM *item = hash->items[ i ];
        if (item != NULL)
        {
            destroy_hash_item(item);
        }
    }
    
    free_mem(hash->items);
    free_mem(hash);
}



void
print_hash_table( const HASH_TABLE* table ) {
    printf("\nHash Table\n-------------------\n");
    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (table->items[ i ])
        {
            printf("Index:%d, Key:%s, Value:%s\n", i, table->items[ i ]->key, table->items[ i ]->value);
        }
    }
    printf("-------------------\n\n");
}
