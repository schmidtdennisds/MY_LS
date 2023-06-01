all : my_ls

my_ls : my_ls.c my_string.c
	gcc -Wall -Wextra -Werror -o my_ls my_ls.c my_string.c -I.

clean:
	rm -f *.o

fclean: clean
	rm -f my_ls

re: fclean all