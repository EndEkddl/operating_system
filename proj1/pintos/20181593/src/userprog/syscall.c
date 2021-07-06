#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "userprog/syscall.h"
#include "userprog/process.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "devices/input.h"
#include <string.h>
#define max(a,b) (((a) > (b)) ? (a) : (b))

static void syscall_handler (struct intr_frame *);
void isMemoryValid(void *addr);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	void *syscallNum = (f->esp);
	void *token1, *token2, *token3, *token4;

	isMemoryValid(syscallNum+3);

	switch(*(int *)syscallNum){
		case SYS_HALT: halt(); break;
		case SYS_EXIT: {
						   token1 = f->esp + 4;
						   isMemoryValid(token1+3);
						   exit(*(int *)token1);
						   break;
					   }
		case SYS_EXEC: {
						   token1 = f->esp +4;
						   isMemoryValid(token1+3);
						   f->eax = (uint32_t)exec( (char *)*(int*)token1 );
						   break;
					   }
		case SYS_WAIT: {
						   token1 = f->esp +4;
						   isMemoryValid(token1+3);
						   f->eax = (uint32_t)process_wait(*(int *)token1);
							break;
					   }
		case SYS_READ: {
						   token1 = f->esp +4;
						   token2 = f->esp +8;
						   token3 = f->esp + 12;
						   isMemoryValid(token3 + 3);
						   f->eax = read(*(int *)token1, *(void **)token2, *(unsigned int *)token3);
						   break;
					   }
		case SYS_WRITE:{
						   token1 = f->esp +4;
						   token2 = f->esp +8;
						   token3 = f->esp + 12;
						   isMemoryValid(token3 + 3);
						   f->eax = write(*(int *)token1, *(void **)token2, *(unsigned int *)token3);
						   
						   break;
					   }
		case SYS_FIBO: {
						   token1 = f->esp +4;
						   isMemoryValid(token1 + 3);
						   f->eax = fibonacci(*(int *)token1);
						   break;
					   }
		case SYS_MAXINT: {
							
						   token1 = f->esp +4;
						   token2 = f->esp +8;
						   token3 = f->esp +12;
						   token4 = f->esp +16;
						   isMemoryValid(token4 + 3);
						   f->eax = max_of_four_int(*(int *)token1, *(int *)token2, *(int *)token3, *(int *)token4);
						   break;
					   }
		default : break;
	}
  
}

void halt(){
	shutdown_power_off();
	return;
}
void exit(int status){
	struct thread *curr = thread_current();
	curr->exit_status = status;

	char name[31];
	int i;

	memset(name, 0x00, sizeof(name));

	int len = strlen(curr->name);
	for(i=0; i<len; i++){
		if(curr->name[i] == '\0' || curr->name[i] == ' ') break;

		name[i] = curr->name[i];
	}
	name[i] = '\0';

	printf("%s: exit(%d)\n", name, curr->exit_status);
	thread_exit();

}

pid_t exec(const char *cmd_line){
	return process_execute(cmd_line);
}

int wait(pid_t pid){
	return process_wait(pid);
}

int read(int fd, void *buffer, unsigned size){
	char tmp; int i;

	if(fd) return -1;
	else{
		for(i=0; i<size; i++){
			tmp = input_getc();
			*(unsigned char *)(buffer+i) = tmp;
	
			if(tmp == '\0' || tmp == ' ') {
				*((char *)buffer+i) = '\0';
				break;
			}
		}
		return i;
	}
}

int write(int fd, const void *buffer, unsigned size){
	if(fd != 1) return -1;
	else {
		putbuf(buffer, size);
		return size;
	}
}

int fibonacci(int n){
	int x=1, y=1, tmp;
	if(n == 1 || n == 2) return 1;

	for(int i=3; i<=n; i++){
		tmp = x;
		x = y;
		y = y + tmp;
	}

	return y;
}

int max_of_four_int(int a, int b, int c, int d){

	return max(a, max(b, max(c, d)));
}

void isMemoryValid(void *addr){
	if(addr < 0x08048000 || addr >= PHYS_BASE) {
		exit(-1);
	}

	return;
}
