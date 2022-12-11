ASM_SRC	= day4.s
C_SRC 	= day4-from-ai.c
# DEBUG   = -g
DEBUG 	=

asm:
	as -o day4.o $(ASM_SRC) $(DEBUG)
	ld $(DEBUG) -o day4 day4.o  -lc
	@# for some reason an invalid interpreter is in the binary, which leads to 'file not found error' when running
	patchelf --set-interpreter "/usr/lib/ld-linux-x86-64.so.2" day4

c:
	gcc $(C_SRC) -o day4 "section-pairs.txt"

