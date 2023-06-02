#ifndef MY_STRING
#define MY_STRING

#include <stdbool.h>
#include <stdio.h>
#define MAX_LEN 256

int my_str_len(char* str);
int my_str_eql(char* str1, char* str2);
char* my_str_cat(char *str1, const char *str2);
void print_str_array(char str_array[][MAX_LEN], int str_count);
int my_str_cmp(char* strA, char* strB);
void my_str_copy(char dest[MAX_LEN], char*src);

#endif