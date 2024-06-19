#include "doubly.h"
//methods: get_data_node(data, prev) | getQueue(size) | enqueue(ref, data) | isEmpty(ref) | peek(ref) | queueSize(ref) | dequeue(ref) 

// other imports
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

// global constants
#define PAGE_SIZE 1024
#define NUM_PAGES 10
#define HEAP_SIZE NUM_PAGES*PAGE_SIZE

void write_page(int data_id)
{
    // printf("%d", data_id);
    FILE *fptr;
    char *num;
    char buffer[100];

    if (asprintf(&num, "%d", data_id) == -1) {
        perror("asprintf");
    } else {
        strcat(strcpy(buffer, "../backingstore/page"), num);
        // printf("%s\n", buffer);
        free(num);
    }

    fptr = fopen(buffer,"w");

    if(fptr == NULL)
    {
        perror("Error in write file!");
        exit(1);
    }

    // fprintf(fptr,"%d",data_id);
    fclose(fptr);
}

void enqueue_fifo(NewQueue * ref, int data_id) {
    if(queueSize(ref) < NUM_PAGES) {
        enqueue(ref,data_id);
    } else {
        write_page(dequeue(ref));
        enqueue(ref, data_id);
    }
}

void load_from_store(NewQueue * ref, int data_id)
{
    char *num;
    char buffer[100];
    char buffer2[100];

    if (asprintf(&num, "%d", data_id) == -1) {
        perror("asprintf");
    } else {
        strcat(strcpy(buffer, "page"), num);
        strcat(strcpy(buffer2, "../backingstore/page"), num);
//        printf("%s\n", buffer);
//        printf("%s\n", buffer2);
        free(num);
    }

    DIR *d;
    struct dirent *dir;
    d = opendir("../backingstore");
    int indicator = 0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp(dir->d_name, buffer) == 0) {
                // found the file in backing store, flip indicator for error msg
                indicator = 1;
                // remove it from file system
                remove(buffer2);
                // enqueue it, and deq the ref too
                enqueue_fifo(ref, data_id);
            }
            // printf("%s\n", dir->d_name);
        }
        closedir(d);
    }

    if (indicator == 0) {
        printf("\n The file you attempted to load does not exist in backing store!");
    }
}

int main() //TESTING
{
    // set up backing store
    int result = mkdir("../backingstore", 0777);
    if(result<0) {
        printf("Backing Store %s\n", strerror(errno));
    }

    // init queue, add 13 data objects
    NewQueue * q = getNewQueue(HEAP_SIZE);
    enqueue_fifo(q, 1);
    enqueue_fifo(q, 2);
    enqueue_fifo(q, 3);
    enqueue_fifo(q, 4);
    enqueue_fifo(q, 5);
    enqueue_fifo(q, 6);
    enqueue_fifo(q, 7);
    enqueue_fifo(q, 8);
    enqueue_fifo(q, 9);
    enqueue_fifo(q, 10); // this should be the max amt in queue
    enqueue_fifo(q, 11);
    enqueue_fifo(q, 12);
    enqueue_fifo(q, 13);

    // contents of queue after enqueue'ing 13 (10 are shown, 3 are in the backing store)
    printQdata(q);
    printf(" Size : %d", queueSize(q));

    // bring page2 into memory from the backing store, use FIFO to kick out the oldest page in memory to make space
    load_from_store(q, 2);
    // contents of queue after loading
    printQdata(q);
    printf(" Size : %d", queueSize(q));

    // try loading a file that doesn't exist in the store
    load_from_store(q, 24); // console will indicate that file doesn't exist in backing store
    // contents of queue are unchanged
    printQdata(q);
    printf(" Size : %d", queueSize(q));

    return 0;
}

