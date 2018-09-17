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
        strncpy(directoryPath, input + cdLength, pathLength - 1);
        // printf("Path is %s\n", directoryPath);
        if (chdir(directoryPath) == -1)
        {
            printError("invalid directory path");
        }
        addToHistory(input);
    }
    else if (strncmp(input, "history", 6) == 0)
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
            strncpy(numberArray, input + historySize + 1, strlen(input) - historySize - 2);
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
        strncpy(string, input + 1, stringLength);
        printHistoryString(string);
    }
    else
    {
        removeNewlineTrailing(input);
        if (isExecutable(input) == 1)
        {
            // printf("This is an executable!\n");
            launchExecutable(input);
            return;
        }
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
        printf("$%s", history[historyCount - 1]);
        runCommand(history[historyCount - 1]);
    }
    else
        printError("there is no commands in the history");
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
            printf("%d %s", commandNumber, history[i]);
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
            printf("%d %s", commandNumber, history[i]);
            flag = 1;
        }
        commandNumber++;
    }
    if (flag == 0)
    {
        char message[50];
        sprintf(message, "%s do not matach any string in history", string);
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
        printf("%d %s", commandNumber, history[i]);
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

void launchExecutable(char *executablePath)
{
    int pid;
    pid = fork();
    if (pid < 0)
    {
        printError("forking failed");
        return;
    }
    else if (pid == 0)
    {
        execv(executablePath, (char *[]){"", NULL});
        printError("can not execute binary");
        exit(EXIT_FAILURE);
    }
    else
    {
        int wStatus;
        int status;
        while ((wStatus = wait(&status)) != -1 &&
               wStatus != pid)
        {
        }
        if (wStatus == -1)
        {
            printError("");
            perror(NULL);
        }
    }
}

char **getargs(char *input)
{
    char *token;
    char **args = malloc(sizeof(char *) * maxArgument);

    if (!args)
    {
        printError("malloc failed");
    }

    token = strtok(input, " ");
    int count = 0;

    while (token != NULL)
    {
        args[count++] = token;
        if (count == maxArgument)
            break;
        token = strtok(NULL, " ");
    }
    args[count] = NULL;

    return args;
}