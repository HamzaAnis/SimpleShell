#ifndef SHELL_H

#define inputSize 100
#define historyLimit 100
#define maxArgument 5

char *history[historyLimit];
extern int historyCount;
extern int initalCommandNumber;

struct command
{
    struct command *nextCommand;
    char *args[maxArgument];
    int count;
};

struct commandLine
{
    struct command *head;
    int count;
};
void init();
void runCommand(char input[]);
void printError(char *message);
void addToHistory(char *input);
void printHistory();
void printHistoryString(char *string);
void checkHistoryLimit();
void clearHistory();
void launchRecentCommand();
void launchRecentCommandWithString(char *string);
void freeMemory(char **history);
int validateCharArray(char *numberArray);
void printLimitedHistory(int count);
int isExecutable(char *fielPath);
void removeNewlineTrailing(char *string);
void getCommands(struct commandLine *cmdL, char *input);
void commandsExecute(struct commandLine *cmdL, char *input);
void launchExecutable(struct command *cmd, int *fd, int index, int count);
#endif //SHELL_H