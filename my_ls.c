#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>

int my_str_len(char* str) {
    int count = 0;
    while (str[count]) {
        count++;
    }
    return count;
}

int my_str_cmp(char* str1, char* str2) {
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
        if (my_str_cmp(av[1], "-a")) {
            *flagA = true;
        } else if (my_str_cmp(av[1], "-t")) {
            *flagT = true;
        }
    } else if (ac == 3) {
        // checking for the first argument and setting flag
        if (my_str_cmp(av[1], "-a")) {
            *flagA = true;
        } else if (my_str_cmp(av[1], "-t")) {
            *flagT = true;
        }

        // checking for the second argument and setting flag
        if (my_str_cmp(av[2], "-a") && !*flagA) {
            *flagA = true;
        } else if (my_str_cmp(av[2], "-t") && !*flagT) {
            *flagT = true;
        }
    }
}

int main(int ac, char** av) {
    bool flagA = false;
    bool flagT = false;
    struct dirent* entry;
    DIR* dir = opendir(".");

    setFlags(ac, av, &flagA, &flagT);
    // printf("Flag A ist set: %d, Flag T is set: %d\n", flagA, flagT);
    
    while ((entry = readdir(dir)) != NULL) {
        char* name = entry->d_name;
        if (!flagA && name[0] == '.') {
            continue;
        }
        printf("%s\n", entry->d_name);
    }
}