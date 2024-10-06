CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror

all: s21_cat.c ../grep/s21_grep.c
	$(CC) $(CFLAGS) -o s21_cat s21_cat.c
	$(CC) $(CFLAGS) -o ../grep/s21_grep ../grep/s21_grep.c

s21_cat: s21_cat.c
	$(CC) $(CFLAGS) -o s21_cat s21_cat.c

s21_grep: ../grep/s21_grep.c
	$(CC) $(CFLAGS) -o ../grep/s21_grep ../grep/s21_grep.c

clean:
	rm -f *.o s21_cat ../grep/*.o ../grep/s21_grep
