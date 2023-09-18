CC = gcc -Wall -Werror -Wextra -std=c11
WFB = ../../build/

OS := $(shell uname)
ifeq ($(OS), Linux)
	WF = ../../build/Linux
	LIBS = -lrt -lm -lsubunit -lpthread
else
	WF = ../../build/macOS/
	LIBS = 
endif


all: build

build:
	@mkdir -p $(WFB)
	@mkdir -p $(WF)
	$(CC)  -static-libgcc *.c -o $(WF)cat $(LIBS)

# valgrind: test
	# CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./res_test

# leaks: test
	# leaks -atExit -- ./../../cat

rebuild: clear build

clang-format:
	clang-format -i -style=Google *.c *.h
	clang-format -n -style=Google *.c *.h
	# clang-format -i *.c *.h
	# clang-format -n *.c *.h

clean: clear

clear:
	rm -rf $(WF)cat