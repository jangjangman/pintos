#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
/*
static void syscall_halt (struct intr_frame *);
static void syscall_exec (struct intr_frame *);
static void syscall_wait (struct intr_frame *);
static void syscall_write (struct intr_frame *);
static void syscall_create (struct intr_frame *);
static void syscall_remove (struct intr_frame *);
static void syscall_open (struct intr_frame *);
static void syscall_read (struct intr_frame *);
static void syscall_write (struct intr_frame *);
static void syscall_seek (struct intr_frame *);
static void syscall_tell (struct intr_frame *);
static void syscall_close (struct intr_frame *);
static void syscall_filesize (struct intr_frame *);
struct file_info * find_by_fd(int);
*/
void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/*
bool
validate_address(void *pointer)
{
	if (is_user_vaddr (pointer) && pagedir_get_page (thread_current ()->pagedir, pointer) != NULL)
		return true;
	return false;
}

*/

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	printf("system call!\n");
	thread_exit();
	
	/*
	if (!validate_address (f->esp))
		    syscall_exit (-1);

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
		*/
}

/*
static void
syscall_halt(struct intr_frame *f)
{
	power_off();
}

static void
syscall_write (struct intr_frame *f)
{
	  int fd;
	  void *buffer;
	  unsigned size;
	  memcpy (&fd, f->esp+4, sizeof (int));
	  memcpy (&buffer, f->esp+8, sizeof (void *));
	  memcpy (&size, f->esp+12, sizeof (unsigned));
	  
	  if (!validate_address (buffer))
		  syscall_exit (-1);
	  if (!validate_fd (fd)){
		  f->eax = -1;
		  return;
	  }
	  
	  struct thread *t;
	  t = thread_current();
	  
	  if (fd ==0) {
		  f->eax = -1;
	  } else if (fd == 1) {
		  putbuf(buffer,size);
		  f->eax = size;
	  } else {
		  struct file *target_file;
		  target_file = find_by_fd (fd)->f;
		  if(!target_file)
			  f->eax = -1;
		  else 
			  f->eax = file_write(target_file, buffer, size);
	  }
}


struct file_info*
find_by_fd (int fd)
{
	struct thread *t = thread_current ();
	struct file_info *fip;
	struct list_elem *ittr;

	ittr = list_begin (&t->fd_table);
	while (ittr != list_end (&t->fd_table))
	{
		fip = list_entry (ittr, struct file_info, elem);
		if (fip->fd == fd)
			return fip;
		ittr = list_next (ittr);
	}
	return NULL;
}
*/
