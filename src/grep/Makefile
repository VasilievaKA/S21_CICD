CC=gcc
FLAGS=-Wall -Werror -Wextra -std=c11 -g

all: clean s21_grep test_grep

s21_grep:
		$(CC) $(FLAGS) -D_GNU_SOURCE s21_grep.c -o s21_grep

clean:
		rm -rf s21_grep *.cfg log.txt 

test_grep:
		chmod +x t_grep.sh
		./t_grep.sh

linter:
		cp ../../materials/linters/.clang-format clang-format
		clang-format -n *.c
docker:
		sh ../../materials/build/run.sh

valgrind:
		valgrind --leak-check=yes ./s21_grep -v "abc" pattern.txt 

sanitize: clean sanitizer test_grep

sanitizer:
		$(CC) $(FLAGS) -fsanitize=address -g s21_grep.c -o s21_grep