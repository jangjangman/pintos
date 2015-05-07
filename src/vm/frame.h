#ifndef VM_FRAME_H
#define VM_FRAME_H
#include "threads/thread.h"
#include "threads/synch.h"

struct frame_entry
{
	void *upage;
	void *kpage;
	struct thread *t;
	struct list_elem list_elem;
};

struct list frame_list;
struct lock frame_lock;
struct bitmap *frame_alloc;

void frame_init (void);
void frame_insert (void *upage, void *kpage, struct thread *t);
void frame_remove (void *kpage);
void *frame_get (void);
void frame_clear (struct thread *t);
#endif
