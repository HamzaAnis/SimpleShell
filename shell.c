#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>

void init()
{
    while (1)
    {
        checkHistoryLimit();
        printf("$");
        char input[inputSize] = "\0";
        // memset(input, '\0', 100);
        fgets(input, sizeof(input), stdin);
        removeNewlineTrailing(input);
        runCommand(input);
    }
}

void runCommand(char input[])
{
    if (strncmp(input, "exit", 4) == 0)
    {
        freeMemory(history);
        exit(0);
    }
    else if (strncmp(input, "cd", 2) == 0)
    {
        int i = 0;
        int pathLength = 0;
        int cdLength = sizeof("cd");

        for (i = cdLength; input[i] != '\0'; i++)
        {
            if (input[i] == 32)
            {
                printError("path can not have spaces in them");
                break;
            }
            pathLength++;
        }
        char *directoryPath = (char *)malloc(pathLength * sizeof(char));
        strncpy(directoryPath, input + cdLength, pathLength);
        // printf("Path is %s\n", directoryPath);
        if (chdir(directoryPath) == -1)
        {
            printError("invalid directory path");
        }
        addToHistory(input);
    }
    else if (strncmp(input, "history", 7) == 0)
    {
        if (strncmp(input, "history -c", 9) == 0)
        {
            clearHistory();
            return;
        }
        else if ((int)strlen(input) > (int)strlen("history") + 1)
        {
            char numberArray[5];
            int historySize = strlen("history");
            strncpy(numberArray, input + historySize + 1, strlen(input) - historySize - 1);
            // printf("A number is entered: %s\n", number);
            if (validateCharArray(numberArray) == 1)
            {
                int count;
                sscanf(numberArray, "%d", &count);
                // printf("The number is %d\n", count);
                printLimitedHistory(count);
            }
            else
            {
                printError("the number is not valid");
            }
            return;
        }
        addToHistory(input);
        printHistory();
    }
    else if (strncmp(input, "!!", 2) == 0)
    {
        launchRecentCommand();
    }
    else if (strncmp(input, "!", 1) == 0)
    {
        int stringLength = 0;
        int i = 0;
        for (i = sizeof("!"); input[i] != '\0'; i++)
        {
            stringLength++;
        }
        char *string = (char *)malloc(stringLength * sizeof(char));
        strncpy(string, input + 1, stringLength + 1);
        string[stringLength + 1] = '\0';
        printHistoryString(string);
    }
    else
    {
        addToHistory(input);
        removeNewlineTrailing(input);
        commandsExecute(input);
        return;
        //When the other command is entered
        if (!(strcmp(input, "\n") == 0))
        {
            addToHistory(input);
        }
    }
}

void launchRecentCommand()
{
    if (historyCount != 0)
    {
        // printf("Recent command was %s\n", history[historyCount - 1]);
        printf("$%s\n", history[historyCount - 1]);
        runCommand(history[historyCount - 1]);
    }
    else
        printError("there is no commands in the history\n");
}

void printError(char *message)
{
    printf("error: %s\n", message);
}

void addToHistory(char input[])
{
    char *tempArray = (char *)malloc(inputSize * sizeof(char));
    memcpy(tempArray, input, inputSize);
    history[historyCount] = tempArray;
    historyCount++;
}

void clearHistory()
{
    initalCommandNumber += historyCount;
    historyCount = 0;
}

void printHistory()
{

    if (historyCount != 0)
    {
        int i;
        int commandNumber = initalCommandNumber;

        for (i = 0; i < historyCount; i++)
        {
            printf("%d %s\n", commandNumber, history[i]);
            commandNumber++;
        }
    }
    else
    {
        printError("there is no commands in the history");
    }
}

void printHistoryString(char *string)
{
    int i;
    int commandNumber = initalCommandNumber;
    int flag = 0;
    for (i = 0; i < historyCount; i++)
    {
        if (strncmp(history[i], string, strlen(string)) == 0)
        {
            printf("%d %s\n", commandNumber, history[i]);
            flag = 1;
        }
        commandNumber++;
    }
    if (flag == 0)
    {
        char message[50];
        sprintf(message, "%s do not match any string in history", string);
        printError(message);
    }
}

void printLimitedHistory(int count)
{
    if (count > historyCount)
    {
        count = historyCount;
    }
    int i;
    int commandNumber = initalCommandNumber;

    for (i = 0; i < count; i++)
    {
        printf("%d %s\n", commandNumber, history[i]);
        commandNumber++;
    }
}

void checkHistoryLimit()
{
    if (historyCount == historyLimit)
    {
        printf("info: 100 commands executed, removing oldest enteries\n");
        clearHistory();
    }
}

void freeMemory(char **history)
{
    int i;
    for (i = 0; i < historyLimit; ++i)
    {
        if (history[i] != NULL)
            free(history[i]);
    }
}

int validateCharArray(char *numberArray)
{
    int i;
    for (i = 0; numberArray[i] != '\0'; i++)
    {
        if (numberArray[i] < 48 || numberArray[i] > 57)
        {
            return 0;
        }
    }
    return 1;
}

int isExecutable(char *filePath)
{
    if ((access(filePath, X_OK) == 0) && (access(filePath, R_OK) == 0))
        return 1;
    else
        return 0;
}

void removeNewlineTrailing(char *string)
{
    int newLine = strlen(string) - 1;
    if (string[newLine] == '\n')
        string[newLine] = '\0';
}
void getCommands(struct commandLine *cmdL, char *input)
{
    struct command *pct = NULL;
    char *command;
    int loc = 0;
    command = strtok(input, "|");
    while (command != NULL)
    {
        char *token;
        loc = loc + strlen(command) + 1;

        struct command *cmd = (struct command *)calloc(1, sizeof(struct command));
        if (cmdL->head == NULL)
        {
            cmdL->head = cmd;
            pct = cmd;
        }
        else
        {
            pct->nextCommand = cmd;
            pct = cmd;
        }
        // printf("Command is %s\n", command);
        token = strtok(command, " ");
        while (token != NULL)
        {
            // printf("Token is %s\n", token);
            cmd->args[cmd->count] = (char *)malloc(strlen(token) + 1);
            strcpy(cmd->args[cmd->count], token);
            (cmd->count)++;
            token = strtok(NULL, " ");
        }
        (cmdL->count)++;
        command = strtok(input + loc, "|");
    }
}
void commandsExecute(char *input)
{
    int i = 0;
    int *fd = NULL;
    struct commandLine *cmdL = (struct commandLine *)calloc(1, sizeof(struct commandLine));
    struct command *cmd = NULL;
    getCommands(cmdL, input);
    // printf("Count was %d\n", cmdL->count);
    if (cmdL->count > 1)
        fd = (int *)malloc(2 * sizeof(int) * (cmdL->count - 1));
    cmd = cmdL->head;
    for (i = 0; i < cmdL->count - 1; i++)
    {
        pipe(fd + 2 * i);
    }
    for (i = 0; i < cmdL->count; i++)
    {
        launchExecutable(cmd, fd, i, cmdL->count);
        cmd = cmd->nextCommand;
        dup2(dup(0), 0);
        dup2(dup(1), 1);
    }
    if (fd != NULL)
        free(fd);
    cmd = cmdL->head;
    while (cmd != NULL)
    {
        int i;
        struct command *cmdArgs = cmd;
        for (i = 0; i < cmdArgs->count; i++)
        {
            free(cmdArgs->args[i]);
        }
        cmd = cmdArgs->nextCommand;
        free(cmdArgs);
    }
    free(cmdL);
}

void launchExecutable(struct command *cmd, int *fd, int index, int count)
{
    int pid = 0;
    pid = fork();
    if (pid < 0)
    {
        return;
    }
    else if (pid == 0)
    {
        int i;
        for (i = 0; i < 2 * (count - 1); i++)
        {
            if (i == 2 * (index - 1))
            {
                dup2(fd[i], 0);
            }
            else if (i == 2 * index + 1)
            {
                dup2(fd[i], 1);
            }
            close(fd[i]);
        }
        execv(cmd->args[0], cmd->args);
    }
    else
    {
        if ((fd != NULL) && (2 * index + 1 < 2 * (count - 1)))
        {
            close(fd[2 * index + 1]);
        }
        if ((fd != NULL) && (2 * (index - 1) >= 0))
        {
            close(fd[2 * (index - 1)]);
        }
        while (pid != waitpid(pid, NULL, WUNTRACED))
        {
        }
    }
}