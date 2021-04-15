#ifndef HASH_TABLE_H
# define HASH_TABLE_H



# define HASH_SIZE 4096



struct hash_item_t
{
    char *key;
    char *data;
};

struct hash_table_t
{
    struct hash_item **items;
    int count;
    int size;
};



typedef struct hash_table_t HASH_TABLE;
typedef struct hash_item_t HASH_ITEM;



HASH_TABLE *  create_hash_table   ( void );
void          destroy_hash_table  ( HASH_TABLE * );
void          insert_hash_table   ( HASH_TABLE *, char *, char * );
void *        search_hash_item    ( HASH_TABLE *, char * );
void          print_hash_table    ( const HASH_TABLE * );



#endif // HASH_TABLE_H
