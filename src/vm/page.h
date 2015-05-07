#ifndef VM_PAGE_H
#define VM_PAGE_H
#include "threads/synch.h"
#include <hash.h>

void spt_init (struct thread *t);
void spt_insert (void *upage, void *kpage, struct thread *t);
void spt_remove (void *upage, struct thread *t);
void spt_destroy (struct hash_elem *elem, void *aux);
struct spt_entry *spt_find_upage (void *upage, struct thread *t);
void stack_growth (void *upage, struct thread *t);

struct spt_entry
{
	bool swapped;
	void *upage;
	uint32_t swap_idx;
	void *kpage;
	struct thread *t;

	struct hash_elem hash_elem;
};
#endif 
