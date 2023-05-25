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

void swap(char* str1, char* str2) {
    char temp[MAX_LEN];
    my_str_copy(temp, str1);
    my_str_copy(str1, str2);
    my_str_copy(str2, temp);
}

void bubbleSort(char array[][MAX_LEN], int str_count) {
    for (int i = 0; i < str_count - 1; i++) {
        for (int j = 0; j < str_count - i - 1; j++) {
            if (my_str_cmp(array[j], array[j+1]) > 0) {
                swap(array[j], array[j + 1]);
            }
        }
    }
}

void setAllFilesAndDirsCount(int* fileCount, int* dirCount, int countFlags, int ac, char** av) {
    DIR* dir;
    for (int indexOperand = countFlags+1; indexOperand < ac; indexOperand++) {
        dir = opendir(av[indexOperand]);
        if (dir == NULL) {
            *fileCount = *fileCount + 1;
        } else {
            *dirCount = *dirCount + 1;
        }
        closedir(dir);
    }
}

void fillFileAndDirArrays(char files[][MAX_LEN], char dirs[][MAX_LEN], int countFlags, int ac, char** av) {
    DIR* dir;
    int fileIndex = 0;
    int dirIndex = 0;
    for (int indexOperand = countFlags+1; indexOperand < ac; indexOperand++) {
        dir = opendir(av[indexOperand]);
        if (dir == NULL) {
            my_str_copy(files[fileIndex], av[indexOperand]);
            fileIndex++;
        } else {
            my_str_copy(dirs[dirIndex], av[indexOperand]);
            dirIndex++;
        }
        closedir(dir);
    }
}

void setDirCount(int* dirCount, char* dirStr, bool flagA) {
    struct dirent* entry;
    DIR* dir = opendir(dirStr);

    while ((entry = readdir(dir)) != NULL) {
        char* name = entry->d_name;
        if (!flagA && name[0] == '.') {
            continue;
        }
        *dirCount = *dirCount + 1;
    }
    closedir(dir);
}

void fillDirArray(char files[][MAX_LEN], char* dirStr, bool flagA) {
    struct dirent* entry;
    DIR* dir = opendir(dirStr);
    int dirIndex = 0;

    while ((entry = readdir(dir)) != NULL) {
        char* name = entry->d_name;
        if (!flagA && name[0] == '.') {
            continue;
        }
        my_str_copy(files[dirIndex], name);
        dirIndex++;
    }
    closedir(dir);
}

void printDirEntries(char* dirStr, bool flagA) {
    int dirCount = 0;
    setDirCount(&dirCount, dirStr, flagA);
    char dir_array[dirCount][MAX_LEN];
    fillDirArray(dir_array, dirStr, flagA);
    bubbleSort(dir_array, dirCount);
    print_str_Array(dir_array, dirCount);
}

int main(int ac, char** av) {
    bool flagA = false;
    bool flagT = false;
    int allFilesCount = 0;
    int allDirsCount = 0;

    setFlags(ac, av, &flagA, &flagT);
    int countFlags = (int) flagA + (int) flagT;
    //printf("Count: %d, Flag A ist set: %d, Flag T is set: %d\n",countFlags, flagA, flagT);

    bool hasOperands = ac > countFlags + 1;
    if (hasOperands) {
        setAllFilesAndDirsCount(&allFilesCount, &allDirsCount, countFlags, ac, av);
        char allFiles_array[allFilesCount][MAX_LEN];
        char allDirs_array[allDirsCount][MAX_LEN];
        fillFileAndDirArrays(allFiles_array, allDirs_array, countFlags, ac, av);
        bubbleSort(allFiles_array, allFilesCount);
        print_str_Array(allFiles_array, allFilesCount);
        if (allFilesCount > 0) {
            printf("\n");
        }

        bubbleSort(allDirs_array, allDirsCount);
        for (int i = 0; i < allDirsCount; i++) {
            char* dir = allDirs_array[i];
            if (allDirsCount > 1) {
                printf("%s:\n", dir);
            }
            printDirEntries(dir, flagA);
            if (i < allDirsCount - 1) {
                printf("\n");
            } 
        }

    } else {
        printDirEntries(".", flagA);
    }
}