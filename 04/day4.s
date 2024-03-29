# Parts of this code were generated by OpenAI
# I did however have to modifiy nearly all of it

.data
        count_contained:.long 0
        count_overlap:  .long 0
        file:           .quad 0       # file descriptor
        line_length:    .quad 0    # store the length of the current line in line_buffer
        line_nr:        .long 0       # store the line number
        r1_upper:       .long 0       # range 1 upper value
        r2_upper:       .long 0       # range 2 upper value
        r1_lower:       .long 0       # range 1 lower value
        r2_lower:       .long 0       # range 2 lower value
        current_char:   .long 0

        filename:       .asciz     "section-pairs.txt"
        format:         .asciz     "%d-%d,%d-%d"
        f_contained:    .asciz     "Current line has containment. count_contained: [%d]\n"
        f_res_cont:     .asciz     "The file contains %d lines where one of the two ranges contains the other\n"
        f_res_over:     .asciz     "The file contains %d lines where one of the two ranges overlaps the other\n"
        f_line_nr:      .asciz     "Line Nr: [%d] -\t"
        f_line_length:  .asciz     "Line Length: [%d] -\tContent: '"
        f_q_newline:    .asciz   "'\n"
        f_error_open:   .asciz     "Error %d while opening file\n"
        f_error_read:   .asciz     "Error %d while reading file.\n"
        f_error_match:  .asciz     "Error %d while matching line.\n"

LINE_BUFSIZ     = 32
PRINT_BUFSIZ    = 100
SYS_READ        = 0
SYS_WRITE       = 1
SYS_OPEN        = 2
SYS_CLOSE       = 3
SYS_EXIT        = 60
STDOUT          = 1

.bss
        line_buffer:   .skip    LINE_BUFSIZ
        print_buffer:  .skip    PRINT_BUFSIZ

.text
.globl  _start
.globl  sscanfl


# syscall arg order
# (call #: rax) - rdi rsi rdx r10 r8 r9
_start:
        push    %rbp
        mov     %rsp, %rbp

        # xorl    %eax, %eax    # initialize count_contained to 0
        # movl    $1, %ebx      # initialize loop variable to 1

        # open the file and store the file descriptor in %ecx
        movq    $SYS_OPEN, %rax      # set the number of the "open" syscall
        movq    $filename, %rdi  # move the address of the filename to %ebx
        xorq    %rsi, %rsi    # set the flags to 0
        xorq    %rdx, %rdx    # set the mode to 0 = read
        # movq    $777, %rsi
        syscall

        # if read fails, jump to error
        jl      .error_open
        jg      .error_open
        # negative return value is error
        cmpq    $0, %rax
        jl      .error_open

        movq    %rax, file      # store file descriptor in file

        # read the file line by line
        .loop:
                call    read_next_line
                
                cmpq    $0, %rax        # rax is number of chars read, if 0 we are done
                je      .done

                # get and print line lenght
                movq    %rax, line_length

                # increment line nr
                incl    line_nr         # increment line_nr

                # print line nr
                movl    line_nr, %edx   # load to print
                leaq    f_line_nr, %rsi # set print format
                push    %rdx
                call    print_rsi_rdx
                pop     %rdx

                # print line lenght
                movq    line_length, %rdx
                leaq    f_line_length, %rsi
                push    %rdx
                call    print_rsi_rdx
                pop     %rdx

                # print the line
                movq    $SYS_WRITE, %rax        # set the number of the "write" syscall to 4
                movq    $STDOUT, %rdi                # move the file descriptor (1 for stdout) to %ebx
                movq    $LINE_BUFSIZ, %rdx       # number of bytes
                leaq    line_buffer, %rsi       # buffer
                syscall

                # print newline
                movq    $SYS_WRITE, %rax        # set the number of the "write" syscall to 4
                movq    $STDOUT, %rdi           # move the file descriptor (1 for stdout) to %ebx
                movq    $2, %rdx                # number of bytes
                leaq    f_q_newline, %rsi       # buffer
                syscall

                # from first arg to last
                leaq    line_buffer, %rdi       # Load the address of the line_buffer into %rdi
                leaq    format, %rsi            # Move the address of the format string into %rs
                leaq    r1_lower, %rdx          # Move the address of the first output register into %esi
                leaq    r1_upper, %rcx          # Move the address of the second output register into %edx
                leaq    r2_lower, %r8           # Move the address of the third output register into %ecx
                leaq    r2_upper, %r9           # Move the address of the fourth output register into %r8d

                push    %rbx            # to align the stack
                call    sscanf          # Call the scanf function
                pop     %rbx            # restore stack ptr
                cmp     $0, %rax
                je      .error_match    # 0 is not succesful

                # load the results into registers
                movl    r1_lower, %eax
                movl    r2_lower, %ebx
                movl    r1_upper, %ecx
                movl    r2_upper, %edx

                cmp     %ebx, %eax              # compare r1l to r2l
                jg      .cont_r1l_greater_r2l
                je      .is_contained           # if lower bounds are equal, one must be contained in the other
                jl      .cont_r1l_less_r2l
                
                # 1) check containment
                .cont_r1l_greater_r2l:               # r1l > r2l
                        # to be contained: r1u <= r2u
                        cmp     %edx, %ecx
                        jl      .is_contained
                        je      .is_contained
                        jmp     .not_contained
                        

                .cont_r1l_less_r2l:
                        # to be contained: r1u >= r1u
                        cmp     %edx, %ecx
                        jg      .is_contained
                        je      .is_contained
                        jmp     .not_contained

                movq    $42, %rax
                jmp .error_open         # it should not come to this

                .is_contained:
                        incl    count_contained
                        jmp     .overlaps       # containment € overlap
                .not_contained:

                # 2) overlap
                # two ranges overlap if
                # r1l > r2l:
                #       r1l <= r2u
                # r1l < r2l:
                #       r1u >= r2l
                # r1l == r2l
                cmpl    %ebx, %eax              # compare r1l to r2l
                jg      .over_r1l_greater_r2l
                je      .overlaps
                jl      .over_r1l_less_r2l

                movq    $43, %rax
                jmp     .error_open             # should not happen

                .over_r1l_greater_r2l:          # r1l > r2l
                        cmpl    %edx, %eax      # overlap if r1l <= r2u
                        je      .overlaps
                        jl      .overlaps
                        jmp     .no_overlap

                .over_r1l_less_r2l:             # r1l < r2l 
                        cmpl    %ebx, %ecx      # overlap if r1u >= r2l
                        jg      .overlaps
                        je      .overlaps
                        jmp     .no_overlap

                .overlaps:
                        incl    count_overlap

                .no_overlap:

                jmp     .loop         # repeat the loop

        .done:

                movl    count_contained, %edx
                leaq    f_res_cont, %rsi # set print format
                push    %rsi
                call    print_rsi_rdx
                pop     %rsi

                movl    count_overlap, %edx
                leaq    f_res_over, %rsi # set print format
                push    %rsi
                call    print_rsi_rdx
                pop     %rsi

                movq    $0, %r10        # set exit status
                jmp     .close_file

        .error_open:
                leaq    f_error_open, %rsi   # set print format
                jmp     .error
        .error_read:
                leaq    f_error_read, %rsi   # set print format
                jmp     .error
        .error_match:
                leaq    f_error_match, %rsi   # set print format
        .error:
                movq    %rax, %rdx      # print rax
                # leaq    f_error, %rsi   # set print format
                push    %rax
                call    print_rsi_rdx
                pop     %rax
                movq    %rax, %r10      # set exit status

        .close_file:
                # close the file, expect exit status in %r10
                movq    $SYS_CLOSE, %rax      # set the number of the "close" syscall to 3
                movq    file, %rdi              # move the file descriptor to %rdi
                syscall

                leave

                movq    $SYS_EXIT, %rax
                movq    %r10, %rdi
                syscall


read_next_line:
        # read char by char, until newline or eof, return number of chars in rax
        xorq    %r10, %r10               # charcount_contained
        xorq    %rsi, %rsi               # make sure rsi is 0
        xorq    %r11, %r11               # make sure r11 is 0
        .load_char:
                movq    $SYS_READ, %rax         # set the number of the "read" syscall
                movq    file, %rdi              # move the file descriptor to %rdi
                leaq    current_char, %rsi      # move the address of the charbuffer to %rsi
                movq    $1, %rdx                # read only 1 char
                syscall

                jl      .error_read

                cmpq    $0, %rax                # if no char is loaded - eof
                je      .line_complete

                movb    current_char, %r11b

                cmpb    $'\n', %r11b            # if newline
                je      .line_complete

                movb    %r11b, line_buffer(,%r10,1)     # copy current char into the place at charcount_contained
                incq    %r10                            # increment charcount_contained
                jmp .load_char
        .line_complete:
        # null terminate the line
        leaq    line_buffer, %rax
        movq    $0, (%rax,%r10,1)

        movq    %r10, %rax      # return number of chars loaded in rax
        ret

print_rsi_rdx:
        # print an int with $format, first get the formatted string from sprintf and the do a write syscall to stdout
        # print the format string in %rsi which contains one int in %rdx
        push    %rdx                    # save the value 

        # Set up the format string and the integer value
        leaq    print_buffer, %rdi      # Load the address of the buffer into %rdi
        # Use sprintf to format the string and the integer value
        call sprintf                    # Call the sprintf function
        # Determine the number of bytes written to the buffer
        movq %rax, %rdx                 # Move the number of bytes written to %rdx
        
        # Set up the write syscall
        movq    $SYS_WRITE, %rax        # set the number of the "write" syscall to 4
        movq    $STDOUT, %rdi           # move the file descriptor (1 for stdout) to %ebx
        leaq    print_buffer, %rsi
        syscall

        pop %rdx
        ret
