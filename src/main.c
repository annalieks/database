#include <stdio.h>
#include <string.h>

#include "../include/interface.h"
#include "../include/database.h"

#define COMMAND_LENGTH 20

int main() {
    clearDatabase();
    printGreeting();
    FILE *input = fopen("../data/commands.txt", "r");
    char command[COMMAND_LENGTH];
    while (fscanf(input, "%19s", command) == 1) {
        printf("Please, enter your command\n");
        chooseCommand(input, command);
    }
    fclose(input);

    do {
        printf("Please, enter your command\n");
        fscanf(stdin, "%19s", command);
        chooseCommand(stdin, command);
    } while (strcmp(command, "exit") != 0);
    return 0;
}
