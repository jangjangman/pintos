#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"

static void syscall_handler (struct intr_frame *);
static void syscall_halt (struct intr_frame *);
//static void syscall_exit (int status);
static void syscall_exec (struct intr_frame *);
static void syscall_wait (struct intr_frame *);
static void syscall_create (struct intr_frame *);
static void syscall_remove (struct intr_frame *);
static void syscall_open (struct intr_frame *);
static void syscall_close (struct intr_frame *);
static void syscall_read (struct intr_frame *);
static void syscall_write (struct intr_frame *);
static void syscall_seek (struct intr_frame *);
static void syscall_tell (struct intr_frame *);
static void syscall_filesize (struct intr_frame *);

static struct user_file* file_by_fd (int fd);
bool validate_address(void *pointer);



void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


bool
validate_address(void *pointer)
{
	if (pointer != NULL && is_user_vaddr (pointer) && pointer >0 && pagedir_get_page(thread_current()->pagedir, pointer)!=NULL)
		return true;
	return false;
}

bool
validate_fd(int fd)
{
	if(fd<0 || fd>127) 
		return false;
	if(fd> thread_current()->cur_fd)
		return false;
	if(fd!=0 &&fd!=1 && file_by_fd(fd) ==NULL)
		return false;
	return true;
}


static void
syscall_handler (struct intr_frame *f) 
{
	//printf("system call!\n");
	//thread_exit();
	
	 // printf("%p\n", f->esp);
	if (!validate_address (f->esp))
	{
				//printf("HERE\n");
		    syscall_exit (-1);
	}

	  int syscall_n = *(int *)(f->esp);
	      
	    switch (syscall_n){
			case SYS_HALT:
				syscall_halt (f);
				break;
			case SYS_EXIT:
				syscall_exit (*(int *)(f->esp+4));
				break;
			case SYS_EXEC:
				syscall_exec (f);
				break;
			case SYS_WAIT:
				syscall_wait (f);
				break;
			case SYS_CREATE:
				syscall_create (f);
				break;
			case SYS_REMOVE:
				syscall_remove (f);
				break;
			case SYS_OPEN:
				syscall_open (f);										
				break;
			case SYS_FILESIZE:
				syscall_filesize (f);
				break;
			case SYS_READ:
				//printf("HERE\n");
				syscall_read (f);
				break;
			case SYS_WRITE:
				syscall_write (f);
				break;
			case SYS_SEEK:
				syscall_seek (f);
				break;
			case SYS_TELL:
				syscall_tell (f);
				break;
			case SYS_CLOSE:
				syscall_close (f);
				break;
		}
		

}


static void
syscall_halt(struct intr_frame *f)
{
	power_off();
	NOT_REACHED ();
}

void
syscall_exit(int status)
{
	//printf("HERE\n");
	if(status <0)
		status=-1;

	struct thread *curr = thread_current();

	curr->ip->exited = true;
	curr->ip->exit_status=status;
	//printf("%s\n",thread_name());
	printf ("%s: exit(%d)\n", thread_name (), status);
	thread_exit();
}

static void
syscall_exec(struct intr_frame *f)
{
	const char *cmd_line;
	memcpy(&cmd_line, f->esp+4, sizeof(char *));

	if(!validate_address(cmd_line))
		syscall_exit(-1);

	int pid = process_execute(cmd_line);
	if(pid==-1)
		f->eax =-1;
	else
		f->eax= pid;

}

static void
syscall_wait(struct intr_frame *f)
{
	pid_t pid;
	memcpy(&pid, f->esp+4, sizeof(pid_t));
	f->eax = process_wait(pid);
}

static void
syscall_create(struct intr_frame *f)
{
	const char *file;
	memcpy(&file,f->esp+4, sizeof(char *));
	unsigned initial_size;
	memcpy(&initial_size,f->esp+8,sizeof(unsigned));
	
	if(!validate_address(file))
		syscall_exit(-1);
	if(!file){
		f->eax=false;
	}
	else
		f->eax=filesys_create(file, initial_size);
}

static void 
syscall_remove(struct intr_frame *f)
{
	const char *file;
	memcpy(&file,f->esp+4, sizeof(char *));
	
	if(!validate_address(file))
		syscall_exit(-1);
	if(!file)
		f->eax=false;
	else
		f->eax=filesys_remove(file);
}

static void 
syscall_open(struct intr_frame *f)
{
	char *file;
	memcpy(&file, f->esp+4, sizeof(char *));

	if(!validate_address(file))
		syscall_exit(-1);

	struct thread *curr=thread_current();
	struct file *t_file=filesys_open(file);
	struct user_file *uf=malloc(sizeof(struct user_file));
	
	if(t_file==NULL){
		f->eax=-1;
		return;
	}
	uf->file = t_file;
	uf->fd = curr->cur_fd;
	list_push_back (&curr->files, &uf->elem);
	f->eax = curr->cur_fd++;
}

static void
syscall_close(struct intr_frame *f)
{
	int fd;
	memcpy(&fd,f->esp+4,sizeof(int));

	if(!validate_fd(fd))
		syscall_exit(-1);
	
	if(fd==0 ||fd==1)
		syscall_exit(-1);

	struct user_file *uf;
	uf = file_by_fd(fd);
	list_remove(&uf->elem);
	file_close(uf->file);
	free(uf);
}

static void
syscall_filesize(struct intr_frame *f)
{
	int fd;
	memcpy(&fd,f->esp+4,sizeof(int));

	struct user_file *t_file= file_by_fd(fd)->file;

	f->eax=file_length(t_file);
}

static void
syscall_read(struct intr_frame *f)
{
				//printf("HERE\n");
	syscall_exit (-1);
				//printf("HERE\n");

	int fd;
	memcpy(&fd,f->esp+4,sizeof(int));
	void *buffer;
	memcpy(&buffer,f->esp+8,sizeof(void *));
	unsigned size;
	memcpy(&size, f->esp+12, sizeof(unsigned));
	
	if(!validate_address(buffer))
		syscall_exit(-1);
	if(!validate_address(buffer+size-1))
		syscall_exit(-1);
	if(buffer==NULL)
		syscall_exit(-1);

	if(!validate_fd(fd)){
		f->eax=-1;
		return;
	}

	if(fd==0){
		int i;
		for(i=0;i<size;i++){
			*(char *)(buffer + i) = input_getc ();
			f->eax=size;
		}
	}
	else if(fd==1)
		f->eax=-1;
	else{
		struct user_file *uf=file_by_fd(fd);
		struct file *t_file= uf->file;
		if(!t_file)
			f->eax=-1;
		else
			f->eax=file_read(t_file, buffer, size);
	}
		


}

static void
syscall_write(struct intr_frame *f)
{
	int fd;
	memcpy(&fd,f->esp+4,sizeof(int));
	void *buffer;
	memcpy(&buffer,f->esp+8,sizeof(void *));
	unsigned size;
	memcpy(&size, f->esp+12, sizeof(unsigned));

	if(!validate_address(buffer))
		syscall_exit(-1);
	if(!validate_address(buffer+size-1))
		syscall_exit(-1);
	if(buffer==NULL)
		syscall_exit(-1);


	if(!validate_fd(fd)){
		f->eax=-1;
		return;
	}
	if(fd==0)
		f->eax= -1;
	else if(fd == 1){
		putbuf(buffer, size);
		f->eax=size;
	}
	else{
		struct user_file *uf=file_by_fd(fd);
		struct file *t_file= uf->file;
		if(!t_file)
			f->eax=-1;
		else
			f->eax=file_write(t_file, buffer, size);
	}



}

static void 
syscall_seek(struct intr_frame *f)
{
	int fd;
	memcpy(&fd,f->esp+4,sizeof(int));
	unsigned position;
	memcpy(&position,f->esp+8,sizeof(unsigned));

	struct user_file *t_file=file_by_fd(fd)->file;
	
	f->eax=file_seek(t_file, position);
}

static void
syscall_tell(struct intr_frame *f)
{
	int fd;
	memcpy(&fd,f->esp+4,sizeof(int));

	struct user_file *t_file = file_by_fd(fd)->file;
	
	f->eax=file_tell(t_file);

}

static struct user_file* 
file_by_fd (int fd)
{
	struct list_elem *e;
	struct thread *curr=thread_current();
	struct user_file *uf;

	for (e = list_begin (&curr->files); e != list_end (&curr->files); e = list_next(e)){
		uf = list_entry (e, struct user_file, elem);
		if (uf->fd == fd)
			return uf;
	}
	
	return NULL;
}
