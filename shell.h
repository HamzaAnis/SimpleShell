#ifndef SHELL_H

#define inputSize 100

int historyLimit = 100;
char *history[100];
int historyCount = 0;

void init();
void runCommand(char input[]);
void printError(char *message);
void addToHistory(char *input);
void printHistory();
void checkHistoryLimit();
void launchRecentCommand();

#endif //SHELL_H