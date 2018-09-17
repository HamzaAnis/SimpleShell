#ifndef SHELL_H

#define inputSize 100
#define historyLimit 100
#define maxArgument 5

char *history[historyLimit];
extern int historyCount;
extern int initalCommandNumber;

void init();
void runCommand(char input[]);
void printError(char *message);
void addToHistory(char *input);
void printHistory();
void printHistoryString(char *string);
void checkHistoryLimit();
void clearHistory();
void launchRecentCommand();
void freeMemory(char **history);
int validateCharArray(char *numberArray);
void printLimitedHistory(int count);
int isExecutable(char *fielPath);
void removeNewlineTrailing(char *string);
void launchExecutable(char *executablePath, char **args);
char **getArguments(char *input);
#endif //SHELL_H