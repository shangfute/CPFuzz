all: harness

harness: harness.c
	AFL_HARDEN=1 afl-gcc harness.c spi_controller.c -o fuzz-target


# afl-persistent: fuzzme.cc fuzz_fuzzme_afl_persistent.cc
# 	AFL_USE_ASAN=1 afl-clang-fast++ -c -g fuzzme.cc -o fuzzme.o
# 	AFL_USE_ASAN=1 afl-clang-fast++ -g fuzzme.o fuzz_fuzzme_afl_persistent.cc -o demo-afl-persistent

PHONY: clean
clean:
	rm -f fuzz-target *.o *.a *.so