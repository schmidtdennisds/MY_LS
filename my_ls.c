#include "my_ls.h"

// sets the flags and returns the count of flags as arguments given to the program
int setFlags(int ac, char** av, bool* flagA, bool* flagT) {
    if (ac == 2) {
        if (my_str_eql(av[1], "-a")) {
            *flagA = true;
        } else if (my_str_eql(av[1], "-t")) {
            *flagT = true;
        } else if (my_str_eql(av[1], "-at") || my_str_eql(av[1], "-ta")) {
            *flagA = true;
            *flagT = true;
            return 1;
        }
    } else if (ac >= 3) {
        // checking for the first argument and setting flag
        if (my_str_eql(av[1], "-a")) {
            *flagA = true;
        } else if (my_str_eql(av[1], "-t")) {
            *flagT = true;
        } else if (my_str_eql(av[1], "-at") || my_str_eql(av[1], "-ta")) {
            *flagA = true;
            *flagT = true;
            return 1;
        }

        // checking for the second argument and setting flag
        if (my_str_eql(av[2], "-a") && !*flagA) {
            *flagA = true;
        } else if (my_str_eql(av[2], "-t") && !*flagT) {
            *flagT = true;
        }
        
    } 
    return (int) *flagA + (int) *flagT;
}

void swap(char* str1, char* str2) {
    char temp[MAX_LEN];
    my_str_copy(temp, str1);
    my_str_copy(str1, str2);
    my_str_copy(str2, temp);
}

void timelex_sort(char* dirStr, char* first, char* second) {

    // Building the path to the first and seconf file
    struct stat buffer;
    struct stat buffer2;
    char dirStrCopy[MAX_LEN] = "";
    char dirStrCopy2[MAX_LEN] = "";
    my_str_cat(dirStrCopy, dirStr);
    my_str_cat(dirStrCopy, "/");
    my_str_cat(dirStrCopy2, dirStr);
    my_str_cat(dirStrCopy2, "/");

    // Getting the seconds and the nano seconds of the files
    my_str_cat(dirStrCopy, first);
    lstat(dirStrCopy, &buffer);
    __time_t sec = buffer.st_mtim.tv_sec;
    __time_t nsec = buffer.st_mtim.tv_nsec;

    my_str_cat(dirStrCopy2, second);
    lstat(dirStrCopy2, &buffer2);
    __time_t sec2 = buffer2.st_mtim.tv_sec;
    __time_t nsec2 = buffer2.st_mtim.tv_nsec;
    
    if (sec2 > sec) {
        swap(first, second);
    } else if (sec2 == sec && nsec2 > nsec) {
        swap(first, second);
    } else if (sec2 == sec && nsec2 == nsec) {
        if (my_str_cmp(first, second) > 0) {
            swap(first, second);
        }   
    }
}

void lex_or_timelex_sort(char* dirStr, char array[][MAX_LEN], int str_count, bool flagT) {
    for (int i = 0; i < str_count - 1; i++) {
        for (int j = 0; j < str_count - i - 1; j++) {
            if (flagT) {
               timelex_sort(dirStr, array[j], array[j+1]);
            } else {
                if (my_str_cmp(array[j], array[j+1]) > 0) {
                    swap(array[j], array[j + 1]);
                }
            }
        }
    }
}

void setAllFilesAndDirsCount(int* fileCount, int* dirCount, int countFlags, int ac,  char av[][MAX_LEN]) {
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

void fillFileAndDirArrays(char files[][MAX_LEN], char dirs[][MAX_LEN], int countFlags, int ac, char av[][MAX_LEN]) {
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

void printDirEntries(char* dirStr, bool flagA, bool flagT) {
    int dirCount = 0;
    setDirCount(&dirCount, dirStr, flagA);
    char dir_array[dirCount][MAX_LEN];
    fillDirArray(dir_array, dirStr, flagA);
    lex_or_timelex_sort(dirStr, dir_array, dirCount, flagT);
    print_str_array(dir_array, dirCount);
}

int main(int ac, char** av) {
    bool flagA = false;
    bool flagT = false;
    int allFilesCount = 0;
    int allDirsCount = 0;

    
    int countFlags = setFlags(ac, av, &flagA, &flagT);
    //printf("Count: %d, Flag A ist set: %d, Flag T is set: %d\n",countFlags, flagA, flagT);

    bool hasOperands = ac > countFlags + 1;
    if (hasOperands) {
        char arguments[ac][MAX_LEN];
        for (int i = 0; i < ac; i++) {
            my_str_copy(arguments[i], av[i]);
        }
        setAllFilesAndDirsCount(&allFilesCount, &allDirsCount, countFlags, ac, arguments);
        char allFiles_array[allFilesCount][MAX_LEN];
        char allDirs_array[allDirsCount][MAX_LEN];
        fillFileAndDirArrays(allFiles_array, allDirs_array, countFlags, ac, arguments);

        lex_or_timelex_sort(".", allFiles_array, allFilesCount, flagT);      

        print_str_array(allFiles_array, allFilesCount);
        if (allFilesCount > 0) {
            printf("\n");
        }

        lex_or_timelex_sort(".", allDirs_array, allDirsCount, flagT);
        for (int i = 0; i < allDirsCount; i++) {
            char* dir = allDirs_array[i];
            if (allDirsCount > 1) {
                printf("%s:\n", dir);
            }
            printDirEntries(dir, flagA, flagT);
            if (i < allDirsCount - 1) {
                printf("\n");
            } 
        }

    } else {
        printDirEntries(".", flagA, flagT);
    }
}