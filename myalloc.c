#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>
#include "myalloc.h"  

int statusno = 0;
static void *arena_start = NULL;
static size_t arena_size = 0;


int myinit(size_t size)
{
    long page_size;
    size_t adjusted_size;
    void *mapping;


    if (size == 0 || size > MAX_ARENA_SIZE) {
        statusno = ERR_BAD_ARGUMENTS;
        return statusno;
    }
    if (arena_start != NULL) {
        statusno = ERR_CALL_FAILED;
        return statusno;
    }
    page_size = sysconf(_SC_PAGESIZE);
    if (page_size <= 0) {
        statusno = ERR_SYSCALL_FAILED;
        return statusno;
    }

    adjusted_size = ((size + (size_t)page_size - 1) /
                     (size_t)page_size) * (size_t)page_size;
    if (adjusted_size > MAX_ARENA_SIZE) {
        statusno = ERR_BAD_ARGUMENTS;
        return statusno;
    }
    mapping = mmap(NULL, adjusted_size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mapping == MAP_FAILED) {
        statusno = ERR_SYSCALL_FAILED;
        return statusno;
    }
    arena_start = mapping;
    arena_size = adjusted_size;
    statusno = 0;
    return (int)arena_size;
}

int mydestroy(void)
{
    if (arena_start == NULL) {
        statusno = ERR_UNINITIALIZED;
        return statusno;
    }
    if (munmap(arena_start, arena_size) == -1) {
        statusno = ERR_SYSCALL_FAILED;
        return statusno;
    }
    arena_start = NULL;
    arena_size = 0;
    statusno = 0;
    return 0;
}