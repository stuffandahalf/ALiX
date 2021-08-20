#include <sys/mem.h>
	
void *
malloc(size_t count)
{
	struct mblk *mem, *new;

	for (mem = mavail; mem != NULL; mem = mem->next) {
		if (!mem->status && mem->sz >= count) {
			if (mem->sz - count > sizeof(struct mblk)) {
				new = mem->ptr + mem->sz;
				new->ptr = new + sizeof(struct mblk);
				new->sz = mem->sz - count - sizeof(struct mblk);
				new->status = MEMORY_STATUS_FREE;

				mem->sz -= new->sz + sizeof(struct mblk);

				new->next = mem->next;
				new->prev = mem;
				mem->next = new;
			}

			mem->status = MEMORY_STATUS_INUSE;
			return mem->ptr;
		}
	}

	return NULL;
}

void
free(void *ptr)
{
	struct mblk *next;
	struct mblk *blk = ptr - sizeof(struct mblk);

	blk->status = MEMORY_STATUS_FREE;
	/*for (next = blk->prev; next != NULL && !next->status; next = next->prev) {
		
	}*/

}

