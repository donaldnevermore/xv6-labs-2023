#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int fd1[2];
  int fd2[2];
  pipe(fd1);
  pipe(fd2);

  int pid = fork();
  if (pid > 0) {
    write(fd1[1], "p", 1);
    close(fd1[1]);

    wait((int *)0);

    char buf1[1];
    read(fd2[0], buf1, sizeof buf1);
    close(fd2[0]);

    printf("%d: received pong\n", getpid());
  } else if (pid == 0) {
    char buf2[1];
    read(fd1[0], buf2, sizeof buf2);
    close(fd1[0]);

    write(fd2[1], buf2, 1);
    close(fd2[1]);
    printf("%d: received ping\n", getpid());
  } else {
    fprintf(2, "Fork failed\n");
    exit(1);
  }

  exit(0);
}
