#ifndef VM_PAGE_H
#define VM_PAGE_H
#include <hash.h>
#include "vm/frame.h"

void page_init(struct hash *spt_hash);
void page_insert(void *upage, void *frame, struct thread *t);
void page_destroy(struct hash *spt_hash);


struct sup_page_entry{

	void *upage;
	void *frame;
	struct thread *t;
	uint32_t swap_index;

	struct hash_elem hash_elem;
};

#endif
