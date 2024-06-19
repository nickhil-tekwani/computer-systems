
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "workingheap.h"

// test initialization
void test_init() {
  printf("Test initializing two different heaps:\n");
  void* heap = pm_init(500);
  print_heap(heap);
  pm_free_heap(heap);

  heap = pm_init(2000);
  print_heap(heap);
  pm_free_heap(heap);
}
// test creating blocks
void test_create() {
  printf("\nTest allocating three chunks:\n");
  void* heap = pm_init(500);
  char* first = pm_malloc(heap, 20);
  char* second = pm_malloc(heap, 50);
  char* third = pm_malloc(heap, 50);
  print_heap(heap);
  pm_free_heap(heap);
}

// test freeing block between used blocks
void test_free_between() {
  printf("\nTest freeing a block between used chunk:\n");
  void* heap = pm_init(500);
  char* first = pm_malloc(heap, 20);
  char* second = pm_malloc(heap, 50);
  char* third = pm_malloc(heap, 50);
  print_heap(heap);

  printf("After removing second chunk:\n");
  pm_free(second);
  print_heap(heap);
  pm_free_heap(heap);
}

// test freeing block with free end block after
void test_free_last() {
  printf("\nTest freeing a block before last empty chunk\n");
  void* heap = pm_init(500);
  char* first = pm_malloc(heap, 20);
  char* second = pm_malloc(heap, 50);
  char* third = pm_malloc(heap, 50);
  print_heap(heap);

  printf("After removing third chunk:\n");
  pm_free(third);
  print_heap(heap);
  pm_free_heap(heap);
}
// test freeing block with free non-end block after
void test_free_first_two() {
  printf("\nTest freeing a block before a different empty chunk\n");
  void* heap = pm_init(500);
  char* first = pm_malloc(heap, 20);
  char* second = pm_malloc(heap, 50);
  char* third = pm_malloc(heap, 50);
  pm_free(second);
  print_heap(heap);

  printf("After removing first chunk:\n");
  pm_free(first);
  print_heap(heap);
  pm_free_heap(heap);
}
// test freeing block with free block before
void test_free_first_two_reversed() {
  printf("\nTest freeing a block after empty chunk\n");
  void* heap = pm_init(500);
  char* first = pm_malloc(heap, 20);
  char* second = pm_malloc(heap, 50);
  char* third = pm_malloc(heap, 50);
  pm_free(first);
  print_heap(heap);

  printf("After removing first used chunk:\n");
  pm_free(second);
  print_heap(heap);
  pm_free_heap(heap);
}

// test freeing block between two free blocks
void test_free_between_unused() {
  printf("\nTest freeing a block between unused chunks\n");
  void* heap = pm_init(500);
  char* first = pm_malloc(heap, 20);
  char* second = pm_malloc(heap, 50);
  char* third = pm_malloc(heap, 50);
  pm_free(first);
  pm_free(third);
  print_heap(heap);

  printf("After removing the remaining chunk:\n");
  pm_free(second);
  print_heap(heap);
  pm_free_heap(heap);
}

// test adding block to free space between blocks
//    at beginning, perfect fit, at end
void test_adding_middle() {
  printf("\nTest adding to a free block between used blocks\n");
  void* heap = pm_init(500);
  char* first = pm_malloc(heap, 20);
  char* second = pm_malloc(heap, 100);
  char* third = pm_malloc(heap, 50);
  pm_free(second);
  print_heap(heap);

  printf("After adding a 30 byte chunk:\n");
  pm_malloc(heap, 30);
  printf("after adding:\n");
  print_heap(heap);
  pm_free_heap(heap);
}

void test_adding_beginning() {
  printf("\nTest adding to a free block at the beginning\n");
  void* heap = pm_init(500);
  char* first = pm_malloc(heap, 50);
  char* second = pm_malloc(heap, 100);
  char* third = pm_malloc(heap, 50);
  pm_free(first);
  print_heap(heap);

  printf("After adding a 10 byte chunk:\n");
  pm_malloc(heap, 10);
  print_heap(heap);
  pm_free_heap(heap);
}

void test_adding_perfect_fit() {
  printf("\nTest adding to a free block that just fits\n");
  void* heap = pm_init(500);
  char* first = pm_malloc(heap, 20);
  char* second = pm_malloc(heap, 100);
  char* third = pm_malloc(heap, 50);
  pm_free(second);
  print_heap(heap);

  printf("After adding a 100 byte chunk:\n");
  pm_malloc(heap, 100);
  print_heap(heap);
  pm_free_heap(heap);
}

void test_adding_later() {
  printf("\nTest adding to a free block between used blocks\n");
  void* heap = pm_init(500);
  char* first = pm_malloc(heap, 20);
  char* second = pm_malloc(heap, 100);
  char* third = pm_malloc(heap, 50);
  char* fourth = pm_malloc(heap, 70);
  pm_free(first);
  pm_free(third);
  print_heap(heap);

  printf("After adding a 25 byte chunk:\n");
  pm_malloc(heap, 25);
  print_heap(heap);
  pm_free_heap(heap);
}
int main() {
  test_init();
  test_create();
  test_free_last();
  test_free_between();
  test_free_first_two();
  test_free_first_two_reversed();
  test_free_between_unused();
  test_adding_middle();
  test_adding_beginning();
  test_adding_later();
  test_adding_perfect_fit();
}
