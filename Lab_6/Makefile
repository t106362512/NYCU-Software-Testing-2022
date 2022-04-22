filename = main
filename_full = $(filename).cpp
asan_bin = asan-$(filename)

.PHONY: all clean

all: ${filename_full}
	g++ -std=c++11 -fsanitize=address $(filename_full) -o $(asan_bin)
	g++ -std=c++11 $(filename_full) -o $(filename)

asan:
	./$(asan_bin)

asan_return:
	ASAN_OPTIONS=detect_stack_use_after_return=1 ./$(asan_bin)

valg: $(filename)
	valgrind ./$(filename)

clean: 
	rm $(filename) $(asan_bin)