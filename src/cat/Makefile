CC=gcc
FLAGS=-Wall -Werror -Wextra -std=c11 -g

all: clean s21_cat test_cat

s21_cat:
		$(CC) $(FLAGS) s21_cat.c -o s21_cat

clean:
		rm -rf s21_cat *.cfg log.txt 

test_cat:
		chmod +x t_cat.sh
		./t_cat.sh

linter:
		cp ../../materials/linters/.clang-format clang-format
		clang-format -n *.c


valgrind:
		valgrind --leak-check=yes ./s21_cat -n gg.txt 

sanitize: clean sanitizer test_cat

sanitizer:
		$(CC) $(FLAGS) -fsanitize=address -g s21_cat.c -o s21_cat