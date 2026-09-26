#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

  char *buff = NULL;
  size_t size = 0;
  ssize_t prog;

  while (true) {

    printf("Enter progams to run.\n> ");
    prog = getline(&buff, &size, stdin);

    if (prog == -1) {
      perror("getline failed!");
      break;
    }

    if (prog > 0 && (buff[prog - 1] == '\n')) {
      (buff)[prog - 1] = '\0';
    }

    if (strlen(buff) == 0) {
      continue;
    }

    pid_t pid = fork();

    if (pid < 0) {
      perror("fork failed!");
      continue;
    } else if (pid == 0) {
      execlp(buff, buff, (char *)NULL);
      perror("Exec failed!");
      exit(EXIT_FAILURE);
    } else {
      int wstatus = 0;
      if (waitpid(pid, &wstatus, 0) == -1) {
        perror("waitpid failed!");
      }
    }
  }

  free(buff);
  return 0;
}
