#ifndef __TASK_H
#define __TASK_H

#include <types.h>
#include <mm.h>

typedef uint32_t pid_t;

typedef enum {
	TASK_SLEEPING = 1,
	TASK_RUNNABLE = 2,
	TASK_ZOMBIE = 3,
} task_state_t;

struct context {
	uint32_t esp;
	uint32_t ebp;
	uint32_t ebx;
	uint32_t esi;
	uint32_t edi;
	uint32_t eflags;
};

struct mm_struct {
	pgd_t *pgd_dir;
};

struct task_struct {
	volatile task_state_t state;
	pid_t pid;
	void *stack;
	struct mm_struct *mm;
	struct context ctx;
	struct task_struct *next;
};

extern pid_t next_pid;

int32_t kthread_create(int (*fn)(void *), void *arg);
void kthread_exit();

#endif
