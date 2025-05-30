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

PREFIX		= riscv32-unknown-elf-
CC		= $(PREFIX)gcc
CXX		= $(PREFIX)g++
AS		= $(PREFIX)as
OBJDUMP		= $(PREFIX)objdump

PYRISC		= /mnt/c/2022-SNU-Computer_Architecture/pyrisc/sim/snurisc.py      # <-- Change this line
PYRISCOPT	= -l 2

INCDIR		=
LIBDIR		=
LIBS		=

CFLAGS		= -Og -march=rv32i -mabi=ilp32 -static 
ASLFAGS		= -march=rv32i -mabi=ilp32 -static
LDFLAGS		= -T./link.ld -nostdlib -nostartfiles
OBJDFLAGS	= -D --section=.text --section=.data

TARGET		= bmpresize
ASRCS		= bmpresize-main.s bmpresize.s bmpresize-test.s
OBJS		= $(ASRCS:.s=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBDIR) $(LIBS)

.s.o:
	$(CC) -c $(CFLAGS) $(INCDIR) $< -o $@

#.c.s:
#	$(CC) $(CFLAGS) $(INCDIR) -S $< -o $@

objdump: $(TARGET)
	$(OBJDUMP) $(OBJDFLAGS) $(TARGET) > $(TARGET).objdump

run: $(TARGET)
	$(PYRISC) $(PYRISCOPT) $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).objdump $(OBJS) *~ a.out
