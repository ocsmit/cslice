CC = gcc
CFLAGS = -g -Wall -O0 -std=c11 -Wextra -Wwrite-strings \
-Wno-parentheses -Wpedantic -Warray-bounds -Wconversion  -Wstrict-prototypes -Wnewline-eof

# Test for leaks with llvm
# Apple clang does not support sanitize
ifeq ($(CHECKLEAK),1)
	CC=/opt/homebrew/opt/llvm/bin/clang
	CFLAGS += -fsanitize=leak
endif
#LDFLAGS = -lm

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES)) # list *.c -> *.o
TARGET = test

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


build: $(OBJECTS)

test: build
	#make -C ./tests
	./bin

.PHONY: clean check
.SILENT: clean
clean:
	rm -rf build $(OBJECTS) $(TESTS) $(TARGET) test
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print` # Remove XCode junk


check:
	@echo Files with pontentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9] (str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)\
		|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true
