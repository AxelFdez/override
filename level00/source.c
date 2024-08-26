#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int input;

  puts("***********************************");
  puts("*          -Level00               *");
  puts("***********************************");
  printf("Password:");
  scanf("%d", &input);
  if (input != 0x149c) {
    puts("\nInvalid Password!");
  }
  else {
    puts("\nAuthenticated!");
    system("/bin/sh");
    return 0;
  }
  return 1;
}