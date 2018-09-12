#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>

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
        return;
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
            historyCount = -1;
        }
        addToHistory(input);
        printHistory();
    }
    else if (strncmp(input, "!!", 2) == 0)
    {
        launchRecentCommand();
    }
    else
    {
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

void printHistory()
{
    int i;
    for (i = 0; i < historyCount; i++)
    {
        printf("%d %s", i, history[i]);
    }
    printf("\n");
}

void checkHistoryLimit()
{
    if (historyCount == 100)
    {
        printf("info: 100 commands executed, removing oldest enteries\n");
        historyCount = -1;
    }
}