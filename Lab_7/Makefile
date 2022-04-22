# CC ?= gcc
CC = afl-cc
C_FILES = $(wildcard *.c)
OBJS += $(C_FILES:.c=.o)
CFLAGS = -Wall -g


all: bmpgrayscale
%.o: %.c
	AFL_USE_ASAN=1 $(CC) $(CFLAGS) -c -o $@ $<
bmpgrayscale: $(OBJS) 
	AFL_USE_ASAN=1 $(CC) $(CFLAGS) -o $@ $(OBJS)
check: bmpgrayscale
	$(RM) out.bmp
	./bmpgrayscale test.bmp out.bmp
clean:
	$(RM) bmpgrayscale *.o *.gch out.bmp >/dev/null 2>/dev/null || true
afl: all
	@ install -d in
	@ cp test.bmp in/a.bmp
	CC=$(CC) AFL_USE_ASAN=1 afl-fuzz -i in -o out -m none -- ./bmpgrayscale @@ a.bmp
