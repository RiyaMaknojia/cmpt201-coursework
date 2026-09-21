#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ssize_t read(char **buff, size_t *size) {

  ssize_t thoughts;

  printf("Enter your thoughts:");

  thoughts = getline(buff, size, stdin);
  if (thoughts == -1) {
    perror("getline failed");
    exit(EXIT_FAILURE);
  }

  if (thoughts > 0 && (*buff)[thoughts - 1] == '\n') {
    (*buff)[thoughts - 1] = '\0';
  }

  return thoughts;
}

void split(char *buff) {

  char *saveptr;

  char *ret = strtok_r(buff, " ", &saveptr);

  while (ret != NULL) {
    printf("%s\n", ret);
    ret = strtok_r(NULL, " ", &saveptr);
  };
}

int main() {

  char *buff = NULL;
  size_t size = 0;
  ssize_t thoughts;

  while ((thoughts = read(&buff, &size)) != -1) {
    split(buff);
  }

  free(buff);
  return 0;
}
