#include "fcntl.h"
#include "stat.h"
#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf(1, "Usage: %s filename\n", argv[0]);
    exit();
  }

  int p[2];
  char *arg[] = {"wc", 0};

  if (pipe(p) < 0) {
    printf(1, "pipe failed\n");
    exit();
  }

  int pid = fork();
  if (pid < 0) {
    printf(1, "fork failed\n");
    exit();
  }

  if (pid == 0) { // child
    close(0);     // close stdin
    dup(p[0]);    // duplicate pipe read end to stdin
    close(p[0]);
    close(p[1]);
    exec("wc", arg);
    printf(1, "exec failed\n");
    exit();
  } else { // parent
    close(p[0]);
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
      printf(1, "open %s failed\n", argv[1]);
      exit();
    }

    char buf[512];
    int n;

    while ((n = read(fd, buf, sizeof(buf))) > 0) {
      if (write(p[1], buf, n) != n) {
        printf(1, "write failed\n");
        exit();
      }
    }
    if (n < 0) {
      printf(1, "read failed\n");
      exit();
    }

    close(fd);
    close(p[1]);
    wait();
  }
  exit();
}
