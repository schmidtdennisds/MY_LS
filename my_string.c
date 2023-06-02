#include "my_string.h"

int my_str_len(char* str) {
    int count = 0;
    while (str[count]) {
        count++;
    }
    return count;
}

int my_str_eql(char* str1, char* str2) {
    if (my_str_len(str1) != my_str_len(str2)) {
        return 0;
    }
    int index = 0;
    while (str1[index]) {
        if (str1[index] != str2[index]) {
            return 0;
        }
        index++;
    }
    return 1;
}

char* my_str_cat(char *str1, const char *str2)
{
    int len = my_str_len(str1);
    int index;
    for (index = 0; str2[index] != '\0'; index++)
    {
        str1[len + index] = str2[index];
    }
    str1[len + index] = '\0';
    return str1;
}

void print_str_array(char str_array[][MAX_LEN], int str_count) {
    for (int i = 0; i < str_count; i++) {
         printf("%s\n", str_array[i]);
    }
}

int my_str_cmp(char* strA, char* strB) {
    if (my_str_eql(strA, strB)) {
        return 0;
    }

    bool is_a_shorter = true;
    int min_len = my_str_len(strA);
    if (my_str_len(strB) < min_len) {
        min_len = my_str_len(strB);
        is_a_shorter = false;
    }
    
    for (int i = 0; i < min_len; i++) {
        if (strA[i] > strB[i]) {
            return 1;
        } else if (strA[i] < strB[i]) {
            return -1;
        }
    }

    if(is_a_shorter){
        return -1;
    } else {
        return 1;
    }
}

void my_str_copy(char dest[MAX_LEN], char*src) {
    int index = 0;
    while (src[index]) {
        dest[index] = src[index];
        index++;
    }
    if (index < MAX_LEN - 1) {
        dest[index] = '\0';
    }
}