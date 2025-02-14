#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("No file given!");
    return 0;
  }

  int p[2];
  char *arg[] = {"wc", NULL};

  if (pipe(p) == -1) {
    perror("pipe failed");
    exit(1);
  }

  pid_t pfork = fork();
  if (pfork < 0) {
    perror("fork failed");
    exit(1);
  }

  if (pfork == 0) {
    close(STDIN_FILENO);
    dup2(p[0], STDIN_FILENO);
    close(p[0]);
    close(p[1]);
    execvp("wc", arg);
    perror("exec failed");
    exit(1);
  } else {
    close(p[0]);

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
      perror("open file failed");
      exit(1);
    }

    const int BUFFERSIZE = 1024;
    char buffer[BUFFERSIZE];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, BUFFERSIZE)) > 0) {
      int pwrite = write(p[1], buffer, bytesRead);
      if (pwrite != bytesRead) {
        perror("write failed");
        exit(1);
      }
    }
    if (bytesRead < 0) {
      perror("read failed");
      exit(1);
    }

    close(fd);
    close(p[1]);
    wait(NULL);
  }

  return 0;
}
