#----------------------------------------------------------------
#
#  4190.308 Computer Architecture (Fall 2022)
#
#  Project #3: Image Resizing in RISC-V Assembly
#
#  November 20, 2022
# 
#  Seongyeop Jeong (seongyeop.jeong@snu.ac.kr)
#  Jaehoon Shim (mattjs@snu.ac.kr)
#  IlKueon Kang (kangilkueon@snu.ac.kr)
#  Wookje Han (gksdnrwp@snu.ac.kr)
#  Jinsol Park (jinsolpark@snu.ac.kr)
#  Systems Software & Architecture Laboratory
#  Dept. of Computer Science and Engineering
#  Seoul National University
#
#----------------------------------------------------------------

####################
# void bmpresize(unsigned char *imgptr, int h, int w, int k, unsigned char *outptr)
####################

	.globl bmpresize
bmpresize: #0x80000094 init address
    #a0: imgptr=0x8001003c, a1:h -> h>>k, a2:w -> w*3, a3:k, a4:outptr = 0x80018000
    #t0: l, t1: i, t2: j, t3: 짬통, t4: w_out으로 씁시다,,
    srl a1, a1, a3
    srl t3, a2, a3 #t3 <- w>>k
    slli t4, t3, 1 #t4 <- (w>>k)*2
    add t4, t4, t3 #t4 <- (w>>k)*2 + (w>>k) = w_out;
    slli t3, a2, 1
    add a2, a2, t3 #a2 <- w*2 + w

    #do-while Loop of i beg
    li t1, 0
    MainLoopi:
    #do-while Loop of j beg
    li t2, 0
    MainLoopj:


    #do-while Loop of l beg
    li t0, 0
    MainLoopl:

    addi sp, sp, -48
    sw t0, 0(sp) #RGB -> 0(sp)
    sw ra, 4(sp)
    sw a0, 8(sp)
    sw a1, 12(sp)
    sw a2, 16(sp)
    sw a3, 20(sp)
    sw a4, 24(sp)
    sw t1, 28(sp)
    sw t2, 32(sp)
    sw t3, 36(sp)
    sw t4, 40(sp)

    mv a0, t2 #a0 <- j
    call DivBy3
    lw a3, 20(sp) #a3 <- k
    sll a0, a0, a3 #a0 <- (j/3) << k
    mv a2, a0 #a2 <- (j/3) << k
    lw a0, 8(sp) #a0 <- imgptr
    lw a1, 28(sp) #a1 <- i
    sll a1, a1, a3 #a1 <- i<<k
    lw a4, 16(sp) #a4 <- w
    call GetAvg
    sw a0, 44(sp) #44(sp) <- val save
    lw t0, 40(sp) #t0 <- w_out
    andi t1, t0, 3 #t1 <- w_out & 3
    li t2, 4
    sub t1, t2, t1 #t1 <- 4 - w_out & 3
    andi t1, t1, 3 #t1 <- padsize = (4 - w_out & 3)&3
    lw a0, 28(sp) #a0 <- i
    add a1, t0, t1 #a1 <- (w_out + padsize)
    call Mul
    #a0 <- i*(w_out + padsize)
    lw t0, 32(sp) #t0 <- j
    add a0, a0, t0
    lw t0, 0(sp) #t0 <- l
    add a1, a0, t0 #a1 <- i*(w_out + padsize)+j+l
    lw a2, 44(sp)
    lw a0, 24(sp)
    call Write

    lw t0, 0(sp)
    lw ra, 4(sp)
    lw a0, 8(sp)
    lw a1, 12(sp)
    lw a2, 16(sp)
    lw a3, 20(sp)
    lw a4, 24(sp)
    lw t1, 28(sp)
    lw t2, 32(sp)
    lw t3, 36(sp)
    lw t4, 40(sp)
    addi sp, sp, 48
    #update l, compare logic
    addi t0, t0, 1
    li t3, 3
    bne t0, t3, MainLoopl
    #Loop l end


    #update j, compare logic
    addi t2, t2, 3
    bne t2, t4, MainLoopj
    #Loop j end
#a0: imgptr, a1:h -> h>>k, a2:w -> w*3, a3:k, a4:outptr
#t0: l, t1: i, t2: j, t3: 짬통, t4: w_out으로 씁시다,,

    #second Loop for j beg
    andi t3, t4, 3 #t3 <- w_out & 3
    li t0, 4 #t4 <- 4
    sub t3, t0, t3 #t1 <- 4 - w_out & 3
    andi t3, t3, 3 #t3 <- padsize = (4 - w_out & 3)&3
    add t3, t3, t4 #t3 <- w_out + padsize
    beq t2, t3, MainLoopj3
    MainLoopj2:
    addi sp, sp, -48
    sw t0, 0(sp)
    sw ra, 4(sp)
    sw a0, 8(sp)
    sw a1, 12(sp)
    sw a2, 16(sp)
    sw a3, 20(sp)
    sw a4, 24(sp)
    sw t1, 28(sp)
    sw t2, 32(sp)
    sw t3, 36(sp)
    sw t4, 40(sp)
    mv a0, t1 #a0 <- i
    mv a1, t3 #a1 <- w_out + padsize
    call Mul
    lw t2, 32(sp)
    mv a1, a0
    add a1, a1, t2
    mv a0, a4
    li a2, 0
    call Write #여기서 t2 값이 바뀌는구나?
    lw t0, 0(sp)
    lw ra, 4(sp)
    lw a0, 8(sp)
    lw a1, 12(sp)
    lw a2, 16(sp)
    lw a3, 20(sp)
    lw a4, 24(sp)
    lw t1, 28(sp)
    lw t2, 32(sp)
    lw t3, 36(sp)
    lw t4, 40(sp)
    addi sp, sp, 48

    addi t2, t2, 1
    bne t2, t3, MainLoopj2
    MainLoopj3:
    #second Loop for j end



    #update i, compare logic
    addi t1, t1, 1
    bne t1, a1, MainLoopi
    #Loop i end
    ret;


GetVal:
    #a0: imgptr, a1: inputByte
    #t0: inputByteOrder, t1: Index, t2: val, t3: mask
    andi t0, a1, 3
    sub t1, a1, t0
    add a0, a0, t1
    lw t2, 0(a0) # imm value만 가능
    slli t1, t0, 3
    li t3, 0x000000FF
    sll t3, t3, t1
    and t2, t2, t3
    srl a0, t2, t1
    ret;

Write:
    #a0: outptr, a1: outputByte, a2: avgVal
    #t0: ByteOrder, t1: Index, t2: val, t3: mask
    andi t0, a1, 3
    sub t1, a1, t0
    add a0, a0, t1
    lw t2, 0(a0) # imm value만 가능
    slli t1, t0, 3
    li t3, 0x000000FF
    sll t3, t3, t1
    sll a2, a2, t1
    not t3, t3
    and t2, t2, t3
    or t2, t2, a2
    sw t2, 0(a0)
    ret;

GetAvg:
    #a0: imgptr, a1:y, a2:x,
    #a3:k, a4:w_in -> w_in_with_padding
    #RGB값은 sp(0)에 저장한 것으로 간주 - caller가 2개를 저장한다 치면, addi  sp, sp, -12 후 sp(0)에 RGB 저장
    #t0: sum, t1: i, t2: j, t3: 짬통
    li t0, 0
    mv t1, a4
    andi t1, t1, 3 #t1 <- w_in % 4
    li t2, 4 #t2 <- 4
    sub t1, t2, t1
    andi t1, t1, 3 #t1 = padsize
    add a4, a4, t1 #t1, t2는 여기서부터 free
    #a0: imgptr, a1:y, a2:x, a3:k, a4:w_in -> w_in_with_padding
    #sp(0): RGB, t0: sum, t1: i, t2: j

    #do-while Loop of i beg
    mv t1, a1
    Loopi:
    #do-while Loop of j beg
    mv t2, a2
    Loopj:
    addi sp, sp, -36
    sw ra, 0(sp)
    sw a0, 4(sp)
    sw a1, 8(sp)
    sw a2, 12(sp)
    sw a3, 16(sp)
    sw a4, 20(sp)
    sw t0, 24(sp)
    sw t1, 28(sp)
    sw t2, 32(sp)
    mv a0, t1
    mv a1, a4
    call Mul #i*w_in_with_padding
    #a0 <- i*w_in_with_padding인 상황
    lw t2, 32(sp) #t2 값 누락된 채로 썼네 시발
    lw a1, 36(sp) #a1 <- RGB ///여기서 RGB값이 ra로 들어가는 문제? -> for문 자체가 문제같음..
    add a1, a1, a0 #a1 <- RGB + i*w_in_with_padding
    slli t3, t2, 1 #t3 <- j*2
    add t3, t3, t2 #t3 <- j*2 + j
    add a1, a1, t3 #a1 <- RGB + i*w_in_with_padding + j*3
    lw a0, 4(sp) #현재 문제는 GetVal에서 발생함. 그 이유는 함수호출 reg 관리
    call GetVal
    mv t3, a0 #t3 <- GetVal result
    lw ra, 0(sp)
    lw a0, 4(sp)
    lw a1, 8(sp)
    lw a2, 12(sp)
    lw a3, 16(sp) #여긴 mul에서 안쓰긴해용..
    lw a4, 20(sp) #마찬가지임,,
    lw t0, 24(sp)
    lw t1, 28(sp)
    lw t2, 32(sp)
    addi sp, sp, 36
    add t0, t0, t3

    #update j, compare logic
    addi t2, t2, 1
    li t3, 1
    sll t3, t3, a3
    add t3, t3, a2
    bne t2, t3, Loopj
    #Loop j end

    #update i, compare logic
    addi t1, t1, 1
    li t3, 1
    sll t3, t3, a3
    add t3, t3, a1
    bne t1, t3, Loopi
    #Loop i end
    srl t0, t0, a3
    srl a0, t0, a3
    ret;

DivBy3:
    #a0 = num

    li t1, 0 #t1: sum
    li t2, 3 #t2: constant 3
    ble a0, t2, L2
    L1:
    srli t3, a0, 2 #t3 <- num>>2
    add t1, t1, t3 #sum = (num >> 2) + sum;
    andi t4, a0, 3 #t4 <- num&3
    add a0, t3, t4
    bgt a0, t2, L1
    L2:
    bne a0, t2, L3
    addi t1, t1, 1
    L3:
    mv a0, t1
    ret;


Mul:
    li t0, 1;
    slli t0, t0, 30; # t0 <- a = 1<<30
    li t1, 30; # t1 <- k = 30
    li t2, 0; # t2 <- sum = 0
    and t3, t0, a1
#while beg
    bne t3, x0, MulL2
    MulL1: # //while((a&j) == 0)
    srli t0, t0, 1 # >>
    addi t1, t1, -1
    and t3, t0, a1
    beq t3, x0, MulL1
    MulL2: # //while((a&j) == 0)
#while end
#while beg
    blt t1, x0, MulL4
    MulL3:
    #if beg
    and t3, t0, a1
    beq t3, x0, MulI1 #if a&j !=0 then sum+=i<<k
    sll t3, a0, t1 # <<
    add t2, t2, t3
    MulI1: #if end
    srli t0, t0, 1 # >>
    addi t1, t1, -1
    bge t1, x0, MulL3
    MulL4:
#while end
    addi a0, t2, 0
    ret;
