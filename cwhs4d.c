
// Me: Can we have shell for dinner?
// Mom: No we have shell at home
// Shell at home:

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "cwhs4d.h"

#define MAX_COMMAND_LENGTH 256


int main(int argc, char **argv) {
    char *prompt = setPrompt(argc, argv);
    while (1) {
        printf("%s", prompt);

        char *command = getCommand();

        if (feof(stdin)) {
            printf("\n");
            _exit(0);
        }

        int argumentCount = argCount(command);
        char **argumentArray = splitToNullTermArray(command, argumentCount, " ");

        if (strcmp(argumentArray[0], "") == 0) {
            continue;
        }

        if (strcmp(argumentArray[0], "exit") == 0){
            _exit(0);
        }

        int shouldWait = checkWait(argumentCount, argumentArray);

        int fd = fork();
        executeOn(argumentArray, shouldWait, fd);

        for (int i = 0; i < argumentCount + 1; ++i) {
            free(argumentArray[i]);
        }
        free(argumentArray);
        free(command);
    }
}

char *getCommand() {
    char *command = calloc(MAX_COMMAND_LENGTH, sizeof(char));
    fgets(command, MAX_COMMAND_LENGTH, stdin);
    return command;
}

char *setPrompt(int argc, char *const *argv) {
    return argc > 1 ?
           (strcmp(argv[1], "-") == 0 ? "" : argv[1])
                    : "cwhs4d: ";
}

int checkWait(int argumentCount, char **argumentArray) {
    int shouldWait = 1;
    if (strcmp((argumentArray[argumentCount - 1]), "&") == 0) {
        free(argumentArray[argumentCount - 1]);
        argumentArray[argumentCount - 1] = NULL;
        shouldWait = 0;
    }
    return shouldWait;
}

void executeOn(char **argumentArray, int shouldWait, int fd) {
    if (fd == -1) {
        perror("cwhs4d_main:");
        _exit(errno);
    }
    if (fd == 0) {
        int returnVal = execvp(argumentArray[0], argumentArray);
        if (returnVal == -1) {
            perror("");
        }
    } else {
        if (shouldWait) {
            int donePid = 0;
            while (donePid != fd) {
                donePid = wait(NULL);
            }
        }
    }
}

int argCount(const char *command) {
    int argumentCount = 1;
    for (int i = 0; i < strlen(command); ++i) {
        if (command[i] == ' ') {
            argumentCount++;
        }
    }
    return argumentCount;
}

char **splitToNullTermArray(char *command, int n, char *by) {
    char *copyStr = strndup(command, strcspn(command, "\n"));
    char *startPtr = copyStr;
    char **strArray = calloc(n + 1, sizeof(char *));
    for (int i = 0; i < n; ++i) {
        int subStrLen = strcspn(startPtr, by);
        strArray[i] = strndup(startPtr, subStrLen);
        startPtr += subStrLen + 1;
    }
    strArray[n] = NULL;
    free(copyStr);
    return strArray;
}

char *strArrayToStr(char **pString, int n) {
    int length = 2;
    for (int i = 0; i < n; ++i) {
        if (i != 0) length += 2;
        length += strlen(pString[i]) + 2;
    }
    length += 1;
    char *toStr = calloc(length, sizeof(char));
    toStr[0] = '[';
    for (int i = 0; i < n; ++i) {
        if (i != 0) {
            strcat(toStr, ", ");
        }
        strcat(toStr, "\"");
        strcat(toStr, pString[i]);
        strcat(toStr, "\"");
    }
    strcat(toStr, "]");
    return toStr;
}


