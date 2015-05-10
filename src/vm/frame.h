#ifndef VM_FRAME_H
#define VM_FRAME_H

#include "threads/synch.h"
#include "threads/thread.h"
#include <list.h>
#include "vm/page.h"

struct lock frame_lock;
struct list frame_list;
struct bitmap *frame_alloc;

struct frame_entry{
	void *frame;
	struct sup_page_entry *spte;
	struct thread *t;
	struct list_elem elem;
};

void frame_init(void);
void frame_insert(struct sup_page_entry *spte, void *frame);
void frame_remove(void *frame);


#endif
