#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char env_name[16];
	strcpy(env_name, argv[1]);
    printf("0x%08xn", (getenv(env_name) + strlen(strcat(env_name, "="))));
    return 0;
}