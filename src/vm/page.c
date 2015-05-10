#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/vaddr.h"
#include "threads/thread.h"
#include "userprog/process.h"
#include "userprog/syscall.h"
#include "userprog/pagedir.h"
#include "vm/frame.h"
#include "vm/page.h"

unsigned page_val(const struct hash_elem *e, void *aux UNUSED)
{
	struct sup_page_entry *spte = hash_entry(e, struct sup_page_entry, hash_elem);
	return spte->upage;
}

bool page_cmp(const struct hash_elem *a, const struct hash_elem *b, void *aux UNUSED){
	struct sup_page_entry *spte_a = hash_entry(a, struct sup_page_entry, hash_elem);
	struct sup_page_entry *spte_b = hash_entry(b, struct sup_page_entry, hash_elem);
	return spte_a->upage < spte_b->upage;
}

void page_init(struct hash *spt_hash){
	hash_init(spt_hash, &page_val, &page_cmp, NULL);
}

static void page_destroy_func (struct hash_elem *e, void *aux UNUSED)
{
	struct sup_page_entry *spte = hash_entry(e, struct sup_page_entry,hash_elem);
	
	frame_remove(pagedir_get_page(thread_current()->pagedir, spte->upage));
	pagedir_clear_page(thread_current()->pagedir, spte->upage);
	free(spte);
}

void page_destroy(struct hash *spt_hash){
	hash_destroy(spt_hash, page_destroy_func);
}

struct sup_page_entry *init_entry(void *upage, void *frame, struct thread *t)
{
	struct sup_page_entry *spte = malloc(sizeof(struct sup_page_entry));
	spte->upage = upage;
	spte->frame = frame;
	spte->t = t;

	return spte;
}

void page_insert(void *upage, void *frame, struct thread *t)
{
	struct sup_page_entry *spte = init_entry (upage,frame,t);
	hash_insert(&t->spt_hash, &spte->hash_elem);
	frame_insert(upage, frame-PHYS_BASE);
}

	
