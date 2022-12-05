# [Day 4](https://adventofcode.com/2022/day/4) 
:gift::gift::gift::gift:
Today's language: **gnu x86-64** (and alternatively **c**)

Today I wanted to use OpenAI to solve the tasks using gnu x86-64, which I have never used before 
(I did some small things in [6502-asssembly](https://github.com/MatthiasQuintern/6502), so it wasn't all new).
The prompt can be seen in prompt.txt. 
It did a lot of things right, however:
- It did generate x86 (32-bit) code because I did not specify x86-64. So I changed the `int $0x80` system calls to use `syscall` (which also meant I had to change all the registers and [syscall numbers](https://filippo.io/linux-syscall-table/))
- It did get the argument order for `sscanf` wrong, even when I specifically asked it told me that the line to read comes last
- It did not correctly implement the logic to test the condition
- It sometimes me wrong syscall numbers (they were just wrong, not even x86)
- It told me the `read` syscall can read until a `newline` by passing `newline` as last arg

So I had to fix all that. I tried to exclusively use the AI to get answers to my questions, however I did sometimes revert to the internet.
It was still a good starting point and provided lots of help. 
At the end of the ~~evening~~ next morning I had coded the task in x86-64 assembly.

To test scanf, I gave it the exact same prompt but said I wanted it in *c*.
It instantly produced compilable c code that gave the right answer...
```shell
#  for the assembly program
make asm
./day4
# for the c promgram
make c
./day4
```
