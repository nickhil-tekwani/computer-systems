#include <stdio.h>
#include <stdlib.h>

typedef struct data_node
{
    int data_id;
    struct data_node * next;
    struct data_node * prev;
}data_node;

typedef struct NewQueue
{
    struct data_node * front;
    struct data_node * rear;
    int size;
}NewQueue;

data_node * get_data_node(int data_id, data_node * prev)
{
    data_node * ref = (data_node * ) malloc(sizeof(data_node));
    if (ref == NULL)
    {
        return NULL;
    }
    ref->data_id = data_id;
    ref->next = NULL;
    ref->prev = prev;
    return ref;
}

NewQueue * getNewQueue(int size)
{
    NewQueue * ref = (NewQueue * ) malloc(size);
    if (ref == NULL)
    {
        return NULL;
    }
    ref->front = NULL;
    ref->rear = NULL;
    return ref;
}

void enqueue(NewQueue * ref, int data_id)
{
    data_node * node = get_data_node(data_id, ref->rear);
    if (ref->front == NULL)
    {
        ref->front = node;
        ref->size = 1;
    }
    else
    {
        ref->rear->next = node;
        ref->size = ref->size + 1;
    }
    ref->rear = node;
}

int isEmpty(NewQueue * ref)
{
    if (ref->size == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int peek(NewQueue * ref)
{
    if (isEmpty(ref) == 1)
    {
        printf("\n Empty Queue");
        // When stack is empty
        return -1;
    }
    else
    {
        return ref->front->data_id;
    }
}

int queueSize(NewQueue * ref)
{
    return ref->size;
}

int dequeue(NewQueue * ref)
{
    if (isEmpty(ref) == 1)
    {
        return -1;
    }
    else
    {
        int data = peek(ref);
        data_node * temp = ref->front;
        if (ref->front == ref->rear)
        {
            ref->rear = NULL;
            ref->front = NULL;
        }
        else
        {
            ref->front = ref->front->next;
            ref->front->prev = NULL;
        }
        ref->size--;
        return data;
    }
}

void printQdata(NewQueue * ref)
{
    data_node * node = ref->front;
    printf("\n Queue Elements\n");
    while (node != NULL)
    {
        printf(" %d", node->data_id);
        node = node->next;
    }
    printf("\n");
}
