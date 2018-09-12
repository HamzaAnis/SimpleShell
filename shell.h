#ifndef SHELL_H

#define inputSize 100

int historyLimit = 100;
char *history[100];
int historyCount = 0;
void init();
void printError(char *message);
void addToHistory(char *input);
void printHistory();
#endif //SHELL_H