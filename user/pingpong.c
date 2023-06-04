#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int fd1[2]; // parent
  int fd2[2]; // child
  if (pipe(fd1) < 0) {
    fprintf(2, "pipe");
    exit(1);
  }
  if (pipe(fd2) < 0) {
    fprintf(2, "pipe");
    exit(1);
  }

  int pid = fork();
  if (pid > 0) {
    close(fd2[1]); // unused
    close(fd1[0]);

    write(fd1[1], "x", 1);

    wait((int *)0);

    char buf1[1];
    read(fd2[0], buf1, sizeof(buf1));

    printf("%d: received pong\n", getpid());
  } else if (pid == 0) {
    close(fd1[1]); // unused
    close(fd2[0]);

    char buf2[1];
    read(fd1[0], buf2, sizeof(buf2));
    printf("%d: received ping\n", getpid());

    write(fd2[1], buf2, 1);
  } else {
    fprintf(2, "Fork failed\n");
    exit(1);
  }

  exit(0);
}
