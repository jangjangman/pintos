#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
void syscall_exit(int status);
void syscall_munmap(int);

typedef int pid_t;

struct lock syscall_lock;
struct lock mmap_lock;
#endif /* userprog/syscall.h */
