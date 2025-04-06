#include <stdlib.h>
#include <stdio.h>
extern char **environ;
int main(int argc, char* argv[]){
  int j;
  char **ep;
  clearenv();
  for (j = 1; j < argc; j++)
    if (putenv(argv[j]) != 0)
      printf("putenv: %s", argv[j]);
      exit(EXIT_FAILURE);

    if (setenv("GREET", "Hello",0) == -1)
        perror("setenv");

    unsetenv("GREET");


  }