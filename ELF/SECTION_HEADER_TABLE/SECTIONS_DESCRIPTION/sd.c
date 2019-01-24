// Author: Abhinav Thakur
// Email : compilepeace@gmail.com


#include <stdint.h>
#include <stdio.h>


uint32_t global_var = 8;
uint32_t global_var_in_bss;

int main()
{
	
	// Some assembly instructions in 'intel' dialect that would 
	// help provide an understanding about the .text section disassembly
	asm("mov rax, 0x4\n"
		"mov rdi, 0x5\n"
		"mov rsi, 0x6\n"
		"mov rdx, 0x7\n"
		);


	// This string gets stored in .rodata section.
	printf("Hello, critical ^_^\n");

return 0;
}
