#ifndef __PTRACE_H
#define __PTRACE_H

struct pt_regs {
	uint32_t ds;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t int_no;
	uint32_t err_code;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t useresp;
	uint32_t ss;
};

typedef void (*interrupt_handler_t)(struct pt_regs *);

void register_interrupt_handler(uint8_t n,
	interrupt_handler_t h);

void isr_handler(struct pt_regs *regs);

void isr0();        // 0 #DE 除 0 异常
void isr1();        // 1 #DB 调试异常
void isr2();        // 2 NMI
void isr3();        // 3 BP 断点异常
void isr4();        // 4 #OF 溢出
void isr5();        // 5 #BR 对数组的引用超出边界
void isr6();        // 6 #UD 无效或未定义的操作码
void isr7();        // 7 #NM 设备不可用(无数学协处理器)
void isr8();        // 8 #DF 双重故障(有错误代码)
void isr9();        // 9 协处理器跨段操作
void isr10();       // 10 #TS 无效TSS(有错误代码)
void isr11();       // 11 #NP 段不存在(有错误代码)
void isr12();       // 12 #SS 栈错误(有错误代码)
void isr13();       // 13 #GP 常规保护(有错误代码)
void isr14();       // 14 #PF 页故障(有错误代码)
void isr15();       // 15 CPU 保留
void isr16();       // 16 #MF 浮点处理单元错误
void isr17();       // 17 #AC 对齐检查
void isr18();       // 18 #MC 机器检查
void isr19();       // 19 #XM SIMD(单指令多数据)浮点异常

void isr20();
void isr21();
void isr22();
void isr23();
void isr24();
void isr25();
void isr26();
void isr27();
void isr28();
void isr29();
void isr30();
void isr31();

void isr255();

#endif
