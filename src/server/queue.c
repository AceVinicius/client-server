/**
 * @file queue.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */



#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#include "../../lib/include/queue.h"
#include "../../lib/include/thread.h"
#include "../../lib/include/allocation.h"



struct queue_t
{
    struct queue_node_t *head;
    struct queue_node_t *tail;
    size_t data_size;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool mutex_enabled;
    bool cond_enabled;
};

struct queue_node_t
{
    void *data;
    struct queue_node_t *next;
};



typedef struct queue_t QUEUE;
typedef struct queue_node_t QUEUE_NODE;



volatile bool queue_running = true;


static void          _set_flags    ( QUEUE *, const short );
static bool          _is_empty     ( QUEUE * );
static bool          _enqueue      ( QUEUE *, const void * );
static void *        _dequeue      ( QUEUE * );



/******************************************************************************
 ***                           INTERNAL FUNCTIONS                           ***
 ******************************************************************************/



static void
_set_flags( QUEUE *queue, const short flag )
{
    switch (flag)
    {
    case THREAD_SAFE:
        queue->mutex_enabled = true;
        mutex_create(&queue->mutex);
        break;
    
    case COND_SAFE:
        queue->mutex_enabled = true;
        queue->cond_enabled  = true;

        mutex_create(&queue->mutex);
        cond_create(&queue->cond);
        break;

    default:
        queue->mutex_enabled = false;
        queue->cond_enabled  = false;
    }
}



static bool
_is_empty( QUEUE *queue )
{
    return queue->head == NULL ? true : false;
}



static bool
_enqueue( QUEUE *queue, const void *data )
{
    QUEUE_NODE *new_node = (QUEUE_NODE *) allocate(0, sizeof(QUEUE_NODE));

    new_node->next = NULL;
    new_node->data = allocate(0, queue->data_size);

    memcpy(new_node->data, data, queue->data_size);
    
    if (queue->tail == NULL) queue->head       = new_node;
    else                     queue->tail->next = new_node;

    queue->tail = new_node;

    return true;
}



static void *
_dequeue( QUEUE *queue )
{
    if (_is_empty(queue)) return NULL;
    
    QUEUE_NODE *temp = queue->head;
    void *data = temp->data;

    queue->head = queue->head->next;

    if (queue->head == NULL) queue->tail = NULL;

    free_mem(temp);

    return data;
}



/******************************************************************************
 ***                           EXTERNAL FUNCTIONS                           ***
 ******************************************************************************/



QUEUE *
queue_create( const size_t data_size, const short flag )
{
    QUEUE *new_queue = (QUEUE *) allocate(0, sizeof(QUEUE));

    new_queue->head      = NULL;
    new_queue->tail      = NULL;
    new_queue->data_size = data_size;

    _set_flags(new_queue, flag);

    return new_queue;
}



bool
queue_is_empty( QUEUE *queue )
{
    if (queue == NULL) return true;

    if (queue->mutex_enabled) mutex_lock(&queue->mutex);
    
    bool empty = _is_empty(queue);

    if (queue->mutex_enabled) mutex_unlock(&queue->mutex);
    

    return empty;
}



bool
enqueue( QUEUE *queue, const void *data )
{
    if (queue == NULL) return false;
    if (data  == NULL) return false;
    
    if (queue->mutex_enabled) mutex_lock(&queue->mutex);

    bool status = _enqueue(queue, data);
    
    if (queue->mutex_enabled)
    {
        if (queue->cond_enabled) cond_signal(&queue->cond);

        mutex_unlock(&queue->mutex);
    }

    return status;
}



void *
dequeue( QUEUE *queue )
{
    if (queue == NULL) return NULL;

    if (queue->mutex_enabled) mutex_lock(&queue->mutex);
    
    void *data = _dequeue(queue);

    if (queue->mutex_enabled)
    {
        if (queue->cond_enabled)
        {
            while (queue_running && data == NULL)
            {
                cond_wait(&queue->cond, &queue->mutex);
                data = _dequeue(queue);
            }
        }

        mutex_unlock(&queue->mutex);
    }

    return data;
}



bool
queue_destroy( QUEUE *queue )
{
    if (queue == NULL) return false;

    if (queue->mutex_enabled)
    {
        mutex_unlock(&queue->mutex);
        // if (queue->cond_enabled)
        // {
        //     queue_running = false;
        //     printf("here\n");
        //     cond_destroy(&queue->cond);
        // }

        mutex_destroy(&queue->mutex);
    }

    QUEUE_NODE *curr = queue->head;
    while (curr != NULL)
    {
        QUEUE_NODE *prev = curr;
        curr = curr->next;
        free_mem(prev->data);
        free_mem(prev);
    }

    free_mem(queue);

    return true;
}
