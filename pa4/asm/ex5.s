#==========================================================================
#
#   Project #4: Extending a 5-stage Pipelined RISC-V Processor
#
#   November 28, 2022
# 
#   Seongyeop Jeong (seongyeop.jeong@snu.ac.kr)
#   Jaehoon Shim (mattjs@snu.ac.kr)
#   IlKueon Kang (kangilkueon@snu.ac.kr)
#   Wookje Han (gksdnrwp@snu.ac.kr)
#   Jinsol Park (jinsolpark@snu.ac.kr)
#
#   Systems Software & Architecture Laboratory
#   Dept. of Computer Science and Engineering
#   Seoul National University
#
#==========================================================================

# Example 2: Load-use hazard

    .text
    .align  2
    .globl  _start
_start:                         # code entry point
    lui     sp, 0x80020
    li t0, 1
    push t0
    addi t0, t0, 1
    push t0
    addi t0, t0, 1
    push t0
    addi t0, t0, 1
    push t0
    addi t0, t0, 1
    push t0
    pop t0
    pop t1
    add a0, t0, t1
    pop t2
    add a1, sp, sp
    pop t3
    pop t4
    ebreak
    


