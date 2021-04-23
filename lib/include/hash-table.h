#ifndef HASH_TABLE_H
# define HASH_TABLE_H



# define HASH_SIZE 2
# define MAX_DATA   512



struct hash_item_t
{
    char *key;
    char *data;
};

struct hash_table_t
{
    struct hash_item_t **items;
    struct list_t      **overflow;
    int count;
    int size;
};

struct list_t
{
    struct node_t *head;
    int count;
};

struct node_t
{
    struct hash_item_t *item;
    struct node_t      *next;
};



typedef struct hash_table_t HASH_TABLE;
typedef struct hash_item_t HASH_ITEM;
typedef struct list_t LIST;
typedef struct node_t NODE;



HASH_TABLE *  create_hash_table   ( void );
void          insert_hash_table   ( HASH_TABLE *, const char *, const char * );
void *        search_hash_table   ( const HASH_TABLE *, const char * );
void          delete_hash_table   ( HASH_TABLE *, const char * );
void          destroy_hash_table  ( HASH_TABLE * );
void          print_hash_table    ( const HASH_TABLE * );



#endif // HASH_TABLE_H
