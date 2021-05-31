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
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include "../../lib/include/general.h"
#include "../../lib/include/thread.h"
#include "../../lib/include/murmur3.h"
#include "../../lib/include/allocation.h"
#include "../../lib/include/hash-table.h"
#include "../../lib/include/sockets.h"



struct hash_table_t
{
    struct hash_table_item_t **array;
    size_t size;
    size_t data_size;
    pthread_mutex_t mutex;
};

struct hash_table_item_t
{
    char *key;
    void *data;
    struct hash_table_item_t *next;
};



typedef struct hash_table_t HASH_TABLE;
typedef struct hash_table_item_t HASH_ITEM;



static        bool         _table_insert  ( HASH_TABLE *, const char *, const void * );
static        void *       _table_peek    ( HASH_TABLE *, const char * );
static        bool         _table_delete  ( HASH_TABLE *, const char * );
static        size_t       _hash          ( const size_t, const char * );
static        HASH_ITEM *  _create_item   ( const size_t, const char *, const void * );
static inline void         _free_item     ( HASH_ITEM * );



/******************************************************************************
 ***                           INTERNAL FUNCTIONS                           ***
 ******************************************************************************/



static size_t
_hash( const size_t  table_size ,
       const char   *key        )
{
    size_t i[ 2 ];

    MurmurHash3_x64_128(key, strnlen(key, MAX_KEY), 42, &i);

    return (i[ 0 ] + i[ 1 ]) % table_size;
}



static HASH_ITEM *
_create_item( const size_t  data_size ,
              const char   *key       ,
              const void   *data      )
{
    HASH_ITEM *new_item = (HASH_ITEM *) allocate(0, sizeof(HASH_ITEM));

    const size_t key_size = strnlen(key, MAX_KEY);

    new_item->key  = (char *) allocate(key_size, sizeof(char));
    new_item->data = (void *) allocate(0, data_size);
    new_item->next = NULL;

    memcpy(new_item->key , key , key_size);
    memcpy(new_item->data, data, data_size);

    new_item->key[key_size] = '\0';

    return new_item;
}



static inline void
_free_item( HASH_ITEM *item )
{
    free_mem(item->key);
    free_mem(item->data);
    free_mem(item);
}



static bool
_table_insert(       HASH_TABLE *table ,
               const char       *key   ,
               const void       *data  )
{
    const size_t index = _hash(table->size, key);

    HASH_ITEM *item = _create_item(table->data_size, key, data);

    item->next = table->array[ index ];
    table->array[ index ] = item;

    return true;
}



static void *
_table_peek(       HASH_TABLE *table ,
             const char       *key   )
{
    const size_t index = _hash(table->size, key);

    HASH_ITEM *temp = table->array[ index ];

    while (temp != NULL && strncmp(temp->key, key, MAX_KEY) != 0)
    {
        temp = temp->next;
    }
    
    if (temp == NULL) return NULL;

    return temp->data;
}



static bool
_table_delete( HASH_TABLE *table ,
               const char *key   )
{
    const size_t index = _hash(table->size, key);

    HASH_ITEM *temp = table->array[ index ];
    HASH_ITEM *prev = NULL;

    while (temp != NULL && strncmp(temp->key, key, MAX_KEY) != 0)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return true;
    
    if (prev == NULL) table->array[ index ] = temp->next;
    else              prev->next = temp->next;

    _free_item(temp);
    
    return true;
}



/******************************************************************************
 ***                           EXTERNAL FUNCTIONS                           ***
 ******************************************************************************/



HASH_TABLE *
hash_table_create( const size_t table_size ,
                   const size_t data_size  )
{
    HASH_TABLE *new_table = (HASH_TABLE *) allocate(0, sizeof(HASH_TABLE));

    new_table->array     = (HASH_ITEM **) allocate(table_size-1, sizeof(HASH_ITEM *));
    new_table->size      = table_size;
    new_table->data_size = data_size;

    mutex_create(&new_table->mutex);

    for (size_t i = 0; i < table_size; ++i)
    {
        new_table->array[ i ] = NULL;
    }

    return new_table;
}



bool
hash_table_insert(       HASH_TABLE *table ,
                   const char       *key   ,
                   const void       *data  )
{
    if (table == NULL) return false;
    if (key   == NULL) return false;
    if (data  == NULL) return false;

    mutex_lock(&table->mutex);
    bool status = false;
    void *ret = _table_peek(table, key);
    if (ret == NULL)
    {
        status = _table_insert(table, key, data);
    }
    mutex_unlock(&table->mutex);

    return status;
}



void *
hash_table_peek(       HASH_TABLE *table ,
                 const char       *key   )
{
    if (table == NULL) return NULL;
    if (key   == NULL) return NULL;

    mutex_lock(&table->mutex);
    void *data = _table_peek(table, key);
    mutex_unlock(&table->mutex);

    return data;
}



bool
hash_table_delete(       HASH_TABLE *table ,
                   const char       *key   )
{
    if (table == NULL) return false;
    if (key   == NULL) return false;

    mutex_lock(&table->mutex);
    bool status = _table_delete(table, key);
    mutex_unlock(&table->mutex);

    return status;
}



bool
hash_table_destroy( HASH_TABLE *table )
{
    if (table == NULL) return false;

    for(size_t i = 0; i < table->size; ++i)
    {
        HASH_ITEM *curr = table->array[ i ];
        while(curr != NULL)
        {
            HASH_ITEM *prev = curr;
            curr = curr->next;
            _free_item(prev);
        }
    }

    mutex_destroy(&table->mutex);
    free_mem(table->array);
    free_mem(table);

    return true;
}



void
hash_table_print( const HASH_TABLE *table )
{
    puts("Start");

    for (size_t index = 0; index < table->size; ++index)
    {
        printf("\t%zu\t", index);

        if (table->array[ index ] == NULL)
        {
            printf("---");
        }
        else
        {
            for (HASH_ITEM *temp = table->array[ index ]; temp != NULL; temp = temp->next)
            {
                printf("%s -> ", temp->key);
            }
        }

        puts("");
    }

    puts("End");
}



void
hash_table_list( const HASH_TABLE *table, const int client_fd )
{
    for (size_t index = 0; index < table->size; ++index)
    {
        if (table->array[ index ] != NULL)
        {
            for (HASH_ITEM *temp = table->array[ index ]; temp != NULL; temp = temp->next)
            {
                send_int(client_fd, 1);
                send_str(client_fd, temp->key);
            }
        }
    }
    
    send_int(client_fd, -1);
}
