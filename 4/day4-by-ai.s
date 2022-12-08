        .data
filename:  .asciz     "input.txt"
format:    .asciz     "%d-%d,%d-%d"
result:    .asciz     "Result: %d\n"

        .text
        .globl  main

main:
        pushl    %ebp
        movl    %esp, %ebp

        xorl    %eax, %eax    # initialize count to 0
        movl    $1, %ebx      # initialize loop variable to 1

        # open the file and store the file descriptor in %ecx
        movl    $5, %eax      # set the number of the "open" syscall to 5
        movl    $filename, %ebx  # move the address of the filename to %ebx
        xorl    %edx, %edx    # set the flags to 0
        int     $0x80

        # read the file line by line
        .loop:
        movl    $3, %eax      # set the number of the "read" syscall to 3
        movl    %ecx, %ebx    # move the file descriptor to %ebx
        movl    $buffer, %ecx # move the address of the buffer to %ecx
        movl    $BUFSIZ, %edx # move the size of the buffer to %edx
        int     $0x80

        # check if end of file has been reached
        cmpl    $0, %eax
        jz      .done         # if so, jump to .done

        # parse the line and check if it satisfies the condition
        call    sscanf 
        movl    %eax, %edi
        movl    %ebx, %esi
        movl    %ecx, %ebp
        movl    %edx, %ebx
        cmpl    %edi, %ebp
        jl      .not_contained
        cmpl    %esi, %ebx
        jg      .not_contained
        incl    %eax          # increment count

        .not_contained:
        incl    %ebx          # increment loop variable

        jmp     .loop         # repeat the loop

        .done:
        # print the result
        movl    $1, %eax      # set the number of the "write" syscall to 1
        movl    $1, %ebx      # set the file descriptor to stdout (1)
        movl    $result, %ecx # move the address of the result string to %ecx
        movl    %eax, %edx    # move the count to %edx
        int     $0x80

        # close the file
        movl    $6, %eax      # set the number of the "close" syscall to 6
        movl    %ecx, %ebx    # move the file descriptor to %ebx
        int     $0x80

        leave
        ret

