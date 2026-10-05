#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void add_to_list(char *list[], int *count, char *line) {
  if (*count < 5) {
    list[*count] = line;
    (*count)++;
  } else {
    free(list[0]);
    for (int i = 0; i < 4; i++) {
      list[i] = list[i + 1];
    }
    list[4] = line;
  }
}

int main(void) {
  char *list[5] = {NULL};
  int count = 0;
  char *line = NULL;
  size_t size = 0;
  ssize_t read;

  printf("Enter input: ");
  while (true) {

    read = getline(&line, &size, stdin);

    if (read == -1) {
      perror("getline failed");
      break;
    }

    if (strcmp(line, "print\n") == 0) {
      add_to_list(list, &count, line);
      for (int i = 0; i < count; i++) {
        printf("%s", list[i]);
      }
    } else {
      add_to_list(list, &count, line);
    }

    line = NULL;
    size = 0;

    printf("Enter input: ");
  }

  free(line);

  for (int i = 0; i < count; i++) {
    free(list[i]);
    list[i] = NULL;
  }
  return 0;
}
