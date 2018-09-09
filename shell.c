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
        printf("$");
        char input[100] = "\0";
        fgets(input, sizeof(input), stdin);
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
                    printf("\nThe path can not have spaces in them!\n");
                    break;
                }
                pathLength++;
            }
            char *directoryPath = (char *)malloc(pathLength * sizeof(char));
            strncpy(directoryPath, input + cdLength, pathLength - 1);
            // printf("Path is %s\n", directoryPath);
            if (chdir(directoryPath) == -1)
            {
                printf("Error while changing the directory!\n");
            }
        }
    }
}
