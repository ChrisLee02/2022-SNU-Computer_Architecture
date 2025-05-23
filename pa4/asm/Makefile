#==========================================================================
#
#   The PyRISC Project
#
#   Makefile
#
#   Jin-Soo Kim
#   Systems Software and Architecture Laboratory
#   Seoul National University
#   http://csl.snu.ac.kr
#
#==========================================================================

.SUFFIXES:  .objdump

PREFIX      = riscv32-unknown-elf-
CC	        = $(PREFIX)gcc
CXX	        = $(PREFIX)g++
AS	        = $(PREFIX)as
OBJDUMP	    = $(PREFIX)objdump

PYRISC      = ../sim/snurisc.py
PYRISCOPT   = -l 1

INCDIR      = 
LIBDIR      =
LIBS        =

CFLAGS      = -Og -march=rv32i -mabi=ilp32 -static 
ASLFAGS     = -march=rv32i -mabi=ilp32 -static 
LDFLAGS     = -T./link.ld -nostdlib -nostartfiles
OBJDFLAGS   = -D --section=.text --section=.data

TARGET      = fib sum100 loaduse forward branch ex1 ex2 ex3 ex4 ex5
ASRCS       = fib.s sum100.s loaduse.s forward.s branch.s ex1.s ex2.s ex3.s ex4.s ex5.s
OBJS        = $(ASRCS:.s=.o)
DUMPS       = $(ASRCS:.s=.objdump)

all: $(TARGET)

fib: fib.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBDIR) $(LIBS)
	$(OBJDUMP) $(OBJDFLAGS) $@ > $@.objdump    

sum100: sum100.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBDIR) $(LIBS)
	$(OBJDUMP) $(OBJDFLAGS) $@ > $@.objdump    

loaduse: loaduse.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBDIR) $(LIBS)
	$(OBJDUMP) $(OBJDFLAGS) $@ > $@.objdump    

forward: forward.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBDIR) $(LIBS)
	$(OBJDUMP) $(OBJDFLAGS) $@ > $@.objdump    

branch: branch.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBDIR) $(LIBS)
	$(OBJDUMP) $(OBJDFLAGS) $@ > $@.objdump    

ex1: ex1.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBDIR) $(LIBS)
	$(OBJDUMP) $(OBJDFLAGS) $@ > $@.objdump    

ex2: ex2.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBDIR) $(LIBS)
	$(OBJDUMP) $(OBJDFLAGS) $@ > $@.objdump    

ex3: ex3.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBDIR) $(LIBS)
	$(OBJDUMP) $(OBJDFLAGS) $@ > $@.objdump    

ex4: ex4.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBDIR) $(LIBS)
	$(OBJDUMP) $(OBJDFLAGS) $@ > $@.objdump    

ex5: ex5.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBDIR) $(LIBS)
	$(OBJDUMP) $(OBJDFLAGS) $@ > $@.objdump

.s.o:
	$(CC) -c $(CFLAGS) $(INCDIR) $< -o $@

.c.s:
	$(CC) $(CFLAGS) $(INCDIR) -S $< -o $@

run: fib
	$(PYRISC) $(PYRISCOPT) $<

clean:
	$(RM) $(TARGET) $(DUMPS) $(OBJS) *~ a.out



