GCC = gcc
CFLAGS = -c -Wall -Werror -Wextra -std=c11 
SOURCE = $(wildcard s21_*.c)
HEADER = $(wildcard s21_*.h)
OBJECTS = $(patsubst %.c,%.o, ${SOURCE})
TESTS_SOURCE = ./tests2/*.c
TESTS_OBJECTS = $(patsubst %.c,%.o, ${TESTS_SOURCE})
UNAME_S := $(shell uname -s)
# SAN = -fsanitize=address -fsanitize=leak -g
ifeq ($(UNAME_S),Linux)
  OPEN_CMD = xdg-open
	ADD_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
	BREW = .
endif
ifeq ($(UNAME_S),Darwin)
	OPEN_CMD = open
	ADD_LIB =
	BREW := ${HOME}/homebrew
endif

PATH := $(BREW)/bin:$(PATH)

all: s21_string.a

s21_string.a: ${SOURCE}
	$(CC) $(CFLAGS) $(ADD_LIB) $(SOURCE)
	ar rc $@ $(OBJECTS)
	ranlib $@
	cp $@ lib$@
	-rm -rf *.o


test: s21_string.a
	 gcc  $(CFLAGS) ./tests2/*.c
	gcc -lcheck -fprofile-arcs -ftest-coverage $(SAN)  s21_string.a   *.o -o tests
	./tests 

main :
	gcc -c -g s21_sprintf.c
	gcc s21_sprintf.o -L . s21_string.a -o dd

gcov_report: ${SOURCE}
	gcc --coverage ${SOURCE} ${TESTS_SOURCE} -o test -lcheck 
	./test
	lcov -t "stest" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	$(OPEN_CMD) ./report/index.html



lcov:
ifeq ("", "$(shell PATH=$(PATH) which lcov)")
	$(error Need to install lcov)
endif


style: clean
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -i -n *.c *.h
	rm .clang-format

clean:
	-rm -rf *.o && rm -rf *.gcno
	-rm -rf *.a && rm -rf *.gcda
	-rm -rf *.info && rm -rf *.gcov
	-rm -rf ./test && rm -rf ./gcov_report
	-rm -rf ./report/
	-rm -rf tests

.PHONY: all clean check lcov

# valgrind: clean
# 	make s21_string.a
# 	gcc -g ${TESTS_SOURCE} s21_string.a -o test -lcheck -lcheck -lpthread -lm -D_GNU_SOURCE -lrt -lsubunit
# 	CK_FORK=no valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose --log-file=RESULT_VALGRIND.txt ./test


#gcc -c -Wall -Werror -Wextra -std=c11 -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE s21_insert.c s21_memchr.c s21_memcmp.c s21_memcpy.c s21_memmove.c s21_memset.c s21_sprintf.c s21_strcat.c s21_strchr.c s21_strcmp.c s21_strcpy.c s21_strcspn.c s21_strerror.c s21_strlen.c s21_strncat.c s21_strncmp.c s21_strncpy.c s21_strpbrk.c s21_strrchr.c s21_strspn.c s21_strstr.c s21_strtok.c s21_to_lower.c s21_to_upper.c s21_trim.c