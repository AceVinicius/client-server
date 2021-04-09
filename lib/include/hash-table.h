#ifndef HASH_TABLE_H
# define HASH_TABLE_H



# define HASH_SIZE 7000



struct hash_item
{
    char *key;
    void *value;
};

struct hash_table
{
    struct hash_item **items;
    int count;
    int size;
};



typedef struct hash_table HASH_TABLE;
typedef struct hash_item HASH_ITEM;



HASH_TABLE  *create_hash_table   ( void );
void         destroy_hash_table  ( HASH_TABLE * );
void         insert_hash_table   ( HASH_TABLE *, char *, char * );
void         print_hash_table    ( const HASH_TABLE * );



#endif // HASH_TABLE_H
