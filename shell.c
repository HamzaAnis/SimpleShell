#include <stdio.h>
#include <stdlib.h>
#include "shell.h"
#include <string.h>
#include <ctype.h>

void init()
{
    while (1)
    {
        printf("$ ");
        char input[100] = "\0";
        fgets(input, sizeof(input), stdin);
        printf("input: %s|\n", input);
        if (strncmp(input, "exit", 4) == 0)
        {
            return;
        }
        else if (strncmp(input, "cd", 2) == 0)
        {
            int i = 0;
            for (i = 0; i < 100; i++)
            {
                if (input[i] != '\0')
                {
                    printf("=> %c\n", input[i]);
                }
            }

            int start = sizeof("cd");
            for (i = start; input[i] != '\0'; i++)
            {
                if (input[i] == 32)
                {
                    printf("\nThe path can not have spaces in them!\n");
                    break;
                }
            }
        }
    }
}
