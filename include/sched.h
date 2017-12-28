#ifndef __SCHED_H
#define __SCHED_H

#include <task.h>

extern struct task_struct *running_task_head;
extern struct task_struct *wait_task_head;
extern struct task_struct *current;

void init_sched();
void schedule();
void change_task_to(struct task_struct *next);
void switch_to(struct context *prev, struct context *next);

#endif
