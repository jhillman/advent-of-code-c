#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct File {
    char name[16];
    int size;
};

struct Directory {
    char name[16];
    struct Directory *parent;

    struct Directory **directories;
    int directoryCount;
    
    struct File *files;
    int fileCount;

    int totalSize;
};

struct Directory *findDirectory(struct Directory *pwd, char *name) {
    if (pwd) {
        for (int i = 0; i < pwd->directoryCount; i++) {
            if (strcmp(name, pwd->directories[i]->name) == 0) {
                return pwd->directories[i];
            }
        }
    }

    return NULL;
}

int directorySize(struct Directory *directory) {
    int size = 0;

    for (int i = 0; i < directory->fileCount; i++) {
        size += directory->files[i].size;
    }

    for (int i = 0; i < directory->directoryCount; i++) {
        size += directorySize(directory->directories[i]);
    }

    return size;
}

void setDirectoryTotalSize(struct Directory *directory) {
    directory->totalSize = directorySize(directory);

    for (int i = 0; i < directory->directoryCount; i++) {
        setDirectoryTotalSize(directory->directories[i]);
    }
}

struct Directory *getRoot(char *input) {
    FILE *inputFile = fopen(input, "r");

    if (inputFile) {
        char line[32];
        char name[16];
        char *command;
        struct Directory *root;
        struct Directory *pwd = NULL;
        bool listing = false;
        int answer = 0;

        while(fgets(line, sizeof(line), inputFile)) {
            line[strcspn(line, "\n")] = 0;

            if (*line == '$') {
                listing = false;
                command = line + 2;

                if (strstr(command, "cd") == command) {
                    strcpy(name, command + 3);

                    if (strcmp(name, "..") == 0) {
                        pwd = pwd->parent;
                    } else if (pwd) {
                        pwd = findDirectory(pwd, name);
                    } else {
                        root = (struct Directory *)calloc(1, sizeof(struct Directory));
                        strcpy(root->name, name);

                        pwd = root;
                    }
                } else if (strstr(command, "ls") == command) {
                    listing = true;
                }
            } else if (listing) {
                if (strstr(line, "dir") == line) {
                    struct Directory *directory = (struct Directory *)calloc(1, sizeof(struct Directory));
                    strcpy(directory->name, line + 4);
                    directory->parent = pwd;

                    if (pwd->directoryCount == 0) {
                        pwd->directories = (struct Directory **)malloc(sizeof(struct Directory *));
                    } else {
                        pwd->directories = (struct Directory **)realloc(pwd->directories, (pwd->directoryCount + 1) * sizeof(struct Directory *));
                    }

                    pwd->directories[pwd->directoryCount++] = directory;
                } else {
                    struct File file;

                    sscanf(line, "%d", &file.size);
                    strcpy(file.name, strchr(line, ' ') + 1);

                    if (pwd->fileCount == 0) {
                        pwd->files = (struct File *)malloc(sizeof(struct File));
                    } else {
                        pwd->files = (struct File *)realloc(pwd->files, (pwd->fileCount + 1) * sizeof(struct File));
                    }

                    pwd->files[pwd->fileCount++] = file;
                }
            }
        }

        fclose(inputFile);

        setDirectoryTotalSize(root);

        return root;
    }

    return NULL;
}

void freeDirectory(struct Directory *directory) {
    if (directory->files) {
        free(directory->files);
    }

    if (directory->directories) {
        for (int i = 0; i < directory->directoryCount; i++) {
            freeDirectory(directory->directories[i]);
        }

        free(directory->directories);
    }

    free(directory);
}
