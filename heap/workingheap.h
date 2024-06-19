#ifndef PM_HEAP
#define PM_HEAP
#include <stdbool.h>

int OFFSET = sizeof(int) + sizeof(void*) + 2*sizeof(bool);

typedef struct fragment {
  int size;
  struct fragment* previous;
  bool used;         
  bool end;                 
} fragment_t;

void* pm_init(int size) {
  // Set up empty fragment struct
  fragment_t* frag = malloc(size + OFFSET );
  frag->size = size;
  frag->previous = NULL;
  frag->used = false;
  frag->end = true;
  return (void*)frag+OFFSET;
}

// This function looks through each fragment until it finds a free one that is large enough
// There is no attempt to look for a 'best fit'
// Return NULL if memory cannot be allocated
void* pm_malloc(void* heap, int size) {
  fragment_t* current = heap - OFFSET;

  while (current->end == 0) {
    // If the current block is usable, allocate it
    if (current->used == 0 && current->size >= size+OFFSET ) {
      // allocate needed memory from current.  Make empty fragment after
      fragment_t* next = (void*)current + size + OFFSET;
      next->size = current->size - size - OFFSET;
      next->used = false;
      next->end = false;
      next->previous = current;
      //printf("next: %d, %d, %d\n", next->size, next->used, next->end);

      current->size = size;
      current->used = true;
      //printf("current: %d, %d, %d\n", current->size, current->used, current->end);

      return (void*)current + OFFSET;
    } else if (current->used == 0 && current->size == size ) {
      current->used = true;
      return (void*)current + OFFSET;
    }
    // Otherwise, move to the next block
    current = (void*)current + current->size + OFFSET;
  }

  // check the last block
  if (current->used == 0 && current->size >= size+OFFSET ) {
    // allocate needed memory from current.  Make empty fragment after
    fragment_t *next = (void *) current + size + OFFSET;
    next->size = current->size - size - OFFSET;
    next->used = false;
    next->end = true;
    next->previous = current;
    //printf("next: %d, %d, %d\n", next->size, next->used, next->end);
    current->size = size;
    current->used = true;
    current->end = false;
    //printf("current: %d, %d, %d\n", current->size, current->used, current->end);
    return (void *) current + OFFSET;
  } else if (current->used == 0 && current->size >= size ) {
    // If there's enough space to allocate, but not enough for an empty segment, allocate generously...
    current->used = true;
    return (void*)current + OFFSET;
  } else {
    return NULL;
  }
}

// ptr must point to the beginning of a pm-malloc'ed
int pm_free(void* ptr) {
  fragment_t* frag = ptr - OFFSET;
  fragment_t* next = (void*)frag + frag->size + OFFSET;

  // TODO check logic for merging with free block after (that's not the end)
  // TODO check logic for freeing block between two free blocks

  // If previous fragment is unused, 'merge' the two
  if ( !(frag->previous == NULL) && frag->previous->used == 0) {
    frag->previous->size += frag->size + OFFSET;
    if (frag->end == 0) {
      next->previous = frag->previous;
    }
    frag = frag->previous;
  }


  // If next fragment is unused, 'merge' the two
  if (frag->end == 0 && next->used == 0) {
    frag->size += next->size + OFFSET;
    frag->used = 0;
    if (next->end == 0) {
      fragment_t *nextnext = (void *)next + next->size + OFFSET;
      nextnext->previous = frag;
    } else {
      frag->end = true;
    }
  }
  else {
    frag->used = false;
  }
}

void pm_free_heap(void* heap) {
  free(heap - OFFSET);
}

void print_frag(fragment_t* frag) {
  if (frag->used) {
    printf("Used block.  Size: %d\n", frag->size);
  } else {
    printf("Free block.  Size: %d\n", frag->size);
  }
}

// For debugging, print a list of fragments within the heap
// and specify which parts are used/unused
void print_heap(void* heap) {
  fragment_t* current = heap - OFFSET;

  while ( !(current->end) ) {
    print_frag(current);
    current = (void*)current + current->size + OFFSET;
  }

  print_frag(current);
}

#endif
