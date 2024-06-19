#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_BYTES 1024

char pm_heap[HEAP_BYTES];

struct memory_block {
    char * start;
    int size;
    enum {OCCUPIED, FREE, SPLIT} state;
};

struct memory_node {
    struct memory_block block;
    struct memory_node * left;
    struct memory_node * right;
};

typedef struct memory_block memory_block;
typedef struct memory_node memory_node;

char * get_block_state(memory_block b) {
    switch(b.state) {
        case 0:
            return "OCCUPIED";
        case 1:
            return "FREE";
        case 2:
            return "SPLIT";
    }
}

int pm_alloc(memory_node * heap, int size, memory_node ** alloc) {
	int new_size = 1;
    while(new_size < size) {
        new_size = new_size << 1;
    }
	
	if (heap->block.size < new_size) {
        return 0; //canot allocate, given is smaller than new size
    } else {
        if (heap->block.size == new_size) {
            if (heap->block.state == FREE) {
                *alloc = heap;
                heap->block.state = OCCUPIED;
                return 1;
            } else {
                return 0;
            }
        } else {
//            if (heap->left == NULL && heap->right == NULL) {
//                // Split the given node in two
//                split(tree);
//            }
            if (pm_alloc(heap->left, size, alloc)) {
                return 1;
            } else {
                return pm_alloc(heap->right, size, alloc);
            }
        }
    }
}

void pm_free(memory_node * m) {
	m->block.state = FREE;
}

// helper for main
memory_block make_block(char * start, int size) {
    memory_block m;
    m.start = start;
    m.size = size;
    m.state = FREE;
    return m;
}

void output_result(memory_node * root) {
	memory_block m = root->block;
	printf("|Node Info: %p\t %p\t %p\n", root, root->left, root->right);
	printf("|Block Info: %p\t %d\t %s\n", m.start, m.size, get_block_state(m));
	output_result(root->left);
	output_result(root->right);
}

int main() {
	// Set the entire heap to NULL
    for(int i = 0; i < HEAP_BYTES; i++) {
        pm_heap[i] = 0;
    }

    // allocate memory, assign to root
    memory_node * root = malloc(sizeof(memory_node));
    root->block = make_block(pm_heap, HEAP_BYTES);
    root->left = NULL;
    root->right = NULL;
	
    memory_node * blocks[3];
    pm_alloc(root, 123, &blocks[0]);
    pm_alloc(root, 1123, &blocks[1]);
    output_result(root);
    blocks[0]->block.state = FREE;
    blocks[1]->block.state = FREE;
    pm_free(root);
    output_result(root);
    return 0;
}