#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 256

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

int main(void) {

  void *heap_start = sbrk(256);
  if (heap_start == (void *)-1) {
    perror("sbrk failed!");
    return 1;
  }

  struct header *block1 = (struct header *)heap_start;
  struct header *block2 = (struct header *)(heap_start + 128);

  block1->size = 128;
  block2->size = 128;

  block1->next = NULL;
  block2->next = block1;

  void *data1 = (char *)block1 + sizeof(struct header);
  void *data2 = (char *)block2 + sizeof(struct header);
  size_t len = 128 - sizeof(struct header);

  memset(data1, 0, len);
  memset(data2, 1, len);

  print_out("first block:        %p\n", &block1, sizeof(void *));
  print_out("second block:       %p\n", &block2, sizeof(void *));
  print_out("first block size:   %lu\n", &(block1->size), sizeof(uint64_t));
  print_out("first block next:   %p\n", &(block1->next), sizeof(void *));
  print_out("second block size:  %lu\n", &(block2->size), sizeof(uint64_t));
  print_out("second block next:  %p\n", &(block2->next), sizeof(void *));

  char *d1 = (char *)data1;
  char *d2 = (char *)data2;
  for (size_t i = 0; i < len; i++) {
    char out[2] = {'0' + d1[i], '\n'};
    write(STDOUT_FILENO, out, 2);
  }
  for (size_t i = 0; i < len; i++) {
    char out[2] = {'0' + d2[i], '\n'};
    write(STDOUT_FILENO, out, 2);
  }

  return 0;
}
