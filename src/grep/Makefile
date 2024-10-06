CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

all: ../cat/s21_cat.c s21_grep.c
	$(CC) $(CFLAGS) -o ../cat/s21_cat ../cat/s21_cat.c
	$(CC) $(CFLAGS) -o s21_grep s21_grep.c

s21_cat: ../cat/s21_cat.c
	$(CC) $(CFLAGS) -o ../cat/s21_cat ../cat/s21_cat.c

s21_grep: s21_grep.c
	$(CC) $(CFLAGS) -o s21_grep s21_grep.c

clean:
	rm -f ../cat/*.o ../cat/s21_cat *.o s21_grep
