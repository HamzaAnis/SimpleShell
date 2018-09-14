#ifndef SHELL_H

#define inputSize 100
#define historyLimit 100

char *history[historyLimit];
int historyCount = 0;
int initalCommandNumber = 0;

void init();
void runCommand(char input[]);
void printError(char *message);
void addToHistory(char *input);
void printHistory();
void printHistoryString(char *string);
void checkHistoryLimit();
void clearHistory();
void launchRecentCommand();
void freeMemory(void **history);

#endif //SHELL_H