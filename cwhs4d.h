//
// Created by foxkj on 10/2/23.
//

#ifndef CWHS4D_CWHS4D_H
#define CWHS4D_CWHS4D_H

char* setPrompt(int argc, char *const *argv);
char *getCommand();
int argCount(const char *command);
char **splitToNullTermArray(char *command, int n, char *by);
int checkWait(int argumentCount, char **argumentArray);
void executeOn(char **argumentArray, int shouldWait, int fd);

char * strArrayToStr(char **pString, int n);


#endif //CWHS4D_CWHS4D_H
