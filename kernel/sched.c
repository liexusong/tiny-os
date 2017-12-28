#include <sched.h>
#include <heap.h>
#include <debug.h>
#include <string.h>
#include <printk.h>
#include <mm.h>

struct task_struct *running_task_head;
struct task_struct *wait_task_head;
struct task_struct *current;
pid_t next_pid;

void init_sched()
{
	current = (struct task_struct *)(kernel_stack_top - STACK_SIZE);

	current->state = TASK_RUNNABLE;
	current->pid = next_pid++;
	current->stack = current;
	current->mm = NULL;
	current->next = current;

	running_task_head = current;
}

void schedule()
{
	if (current) {
		change_task_to(current->next);
	}
}

void change_task_to(struct task_struct *next)
{
	if (current != next) {
		struct task_struct *prev = current;
		current = next;
		switch_to(&(prev->ctx), &(next->ctx));
	}
}

int32_t kthread_create(int (*fn)(void *), void *arg)
{
	struct task_struct *task = (struct task_struct *)kmalloc(STACK_SIZE);
	uint32_t *stack_top;
	struct task_struct *tail;

	bzero(task, sizeof(struct task_struct));

	task->state = TASK_RUNNABLE;
	task->stack = task;
	task->pid = next_pid++;
	task->mm = NULL;

	stack_top = (uint32_t *)((uint32_t)task + STACK_SIZE);

	*(--stack_top) = (uint32_t)arg;
	*(--stack_top) = (uint32_t)kthread_exit;
	*(--stack_top) = (uint32_t)fn;
	
	task->ctx.esp = (uint32_t)task + STACK_SIZE - sizeof(uint32_t) * 3;
	task->ctx.eflags = 0x200;

	task->next = running_task_head;

	tail = running_task_head;
	while (tail->next != running_task_head) {
		tail = tail->next;
	}
	tail->next = task;

	return task->pid;
}

void kthread_exit()
{
	register uint32_t eax __asm__("eax");

	printk("Thread exit with value %d\n", eax);

	while (1);
}
