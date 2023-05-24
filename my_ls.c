#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>

#define MAX_LEN 256

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

void setFlags(int ac, char** av, bool* flagA, bool* flagT) {
    if (ac == 2) {
        if (my_str_eql(av[1], "-a")) {
            *flagA = true;
        } else if (my_str_eql(av[1], "-t")) {
            *flagT = true;
        }
    } else if (ac == 3) {
        // checking for the first argument and setting flag
        if (my_str_eql(av[1], "-a")) {
            *flagA = true;
        } else if (my_str_eql(av[1], "-t")) {
            *flagT = true;
        }

        // checking for the second argument and setting flag
        if (my_str_eql(av[2], "-a") && !*flagA) {
            *flagA = true;
        } else if (my_str_eql(av[2], "-t") && !*flagT) {
            *flagT = true;
        }
    }
}

void my_str_copy(char dest[MAX_LEN], char*src) {
    for (int i = 0; i < MAX_LEN; i++) {
        dest[i] = src[i];
    }
}

void print_str_Array(char str_array[][MAX_LEN], int str_count) {
    for (int i = 0; i < str_count; i++) {
        printf("%s\n", str_array[i]);
    }
}

int main(int ac, char** av) {
    bool flagA = false;
    bool flagT = false;
    struct dirent* entry;
    DIR* dir = opendir(".");
    int strCount = 0;

    setFlags(ac, av, &flagA, &flagT);
    // printf("Flag A ist set: %d, Flag T is set: %d\n", flagA, flagT);

    // Sets the count of the all the strings in the output of my_ls
    while ((entry = readdir(dir)) != NULL) {
        char* name = entry->d_name;
        if (!flagA && name[0] == '.') {
            continue;
        }
        strCount++;
    }
    closedir(dir);

    char str_array[strCount][MAX_LEN];

    opendir(".");
    int index = 0;
    while ((entry = readdir(dir)) != NULL) {
        char* name = entry->d_name;
        if (!flagA && name[0] == '.') {
            continue;
        }
        my_str_copy(str_array[index], name);
        index++;
    }
    closedir(dir);

    print_str_Array(str_array, strCount);
    
}