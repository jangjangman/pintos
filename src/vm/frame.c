#include "threads/vaddr.h"
#include "threads/malloc.h"
#include "threads/thread.h"
#include <bitmap.h>
#include "vm/frame.h"



void frame_init()
{
	int i;
	frame_alloc = bitmap_create(1024);
	for(i=0;i<0x28b;i++){
		bitmap_set(frame_alloc, i, true);
	}
	lock_init(&frame_lock);
	list_init(&frame_list);
}


void frame_insert(struct sup_page_entry *spte, void *frame)
{
	struct frame_entry *fte = malloc(sizeof(struct frame_entry));
	fte->spte = spte;
	fte->frame = frame;
	fte->t = thread_current();

	bitmap_set(frame_alloc, ((int)frame)/PGSIZE, true);
	list_push_back(&frame_list, &fte->elem);
}

void frame_remove(void *frame)
{
	struct list_elem *e;

	for(e=list_begin(&frame_list); e!=list_end(&frame_list); e=list_next(e))
	{
		struct frame_entry *fte = list_entry(e, struct frame_entry, elem);
		if(fte->frame == frame)
		{
			list_remove(e);
			free(fte);
			palloc_free_page(frame);
			break;
		}
	}
}


	
