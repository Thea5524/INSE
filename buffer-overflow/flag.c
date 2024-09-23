#include <stdio.h>
#include <string.h>
int main(int argc, char* argv[])
{
    int	 flag=0;
    char buffer[16];    // khai báo vùng nhớ 16 byte
    strcpy(buffer,argv[1]); //argv[1] là chuỗi ta input
    if (flag!=0)
	printf("Modified\n");
    else
	printf("Try again\n");
    return 0;
} 
