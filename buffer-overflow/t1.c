#include <stdio.h>
#include <unistd.h>

int hackThePlanet() 
{
  system("cowsay -f tux 'You got Hacked' | lolcat");
  return 0;
}

int bufferOverflow()
{
  char buffer[200];
  int input;
  printf("Hello! Welcome to the Buffer Overflow Program, can you solve me?\n");
  input = read(0, buffer, 400);
  printf("\n[+] user supplied: %d bytes!",input);
  printf("\n[+] buffer content --> %s!", buffer);
  return 0;
}

int main(int argc, char *argv[])
{
  bufferOverflow();
  return 0;
}