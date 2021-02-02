#include <stdio.h>
#include <stdlib.h>

#include "user.h"
#include "interface.h"
#include "database.h"

#define STANDARD_FORMAT "%39s"
#define COMMAND_FORMAT "\n~~~~ %s ~~~~\n"
#define SEPARATOR "~~~~~~~~~~~~~~\n\n"

void chooseCommand(FILE *input, char *command) {
    printf(COMMAND_FORMAT, command);
    if (strcmp(command, "insert-m") == 0) {
        insert_m(input);
    } else if (strcmp(command, "insert-s") == 0) {
        insert_s(input);
    } else if (strcmp(command, "get-m") == 0) {
        get_m(input);
    } else if (strcmp(command, "get-s") == 0) {
        get_s(input);
    } else if (strcmp(command, "update-m") == 0) {
        update_m(input);
    } else if (strcmp(command, "update-s") == 0) {
        update_s(input);
    } else if (strcmp(command, "del-m") == 0) {
        del_m(input);
    } else if (strcmp(command, "del-s") == 0) {
        del_s(input);
    } else if (strcmp(command, "calc-m") == 0) {
        calc_m();
    } else if (strcmp(command, "calc-s") == 0) {
        calc_s(input);
    } else if (strcmp(command, "ut-m") == 0) {
        ut_m();
    } else if (strcmp(command, "ut-s") == 0) {
        ut_s();
    } else if (strcmp(command, "exit") == 0) {
        printBye();
        exit(0);
    } else {
        printf("Unknown command\n");
    }
    printf(SEPARATOR);
}

void printGreeting() {
    printf("This is a database emulator written is C.\n");
    printf("Available commands:\n");
    printf("\t---> get-m - get the record from the master file\n"
           "\tUsage: get-m username\n");
    printf("\t---> get-s - get the record from the slave file\n"
           "\tUsage: get-s username reaction\n");
    printf("\t---> del-m - delete the record from the master file\n"
           "\tUsage: del-m username\n");
    printf("\t---> del-s - delete the record from the slave file\n"
           "\tUsage: del-s username reaction\n");
    printf("\t---> update-m - update the record in the master file\n"
           "\tUsage: update-m username email email@gmail.com\n");
    printf("\t---> update-s - update the record in the slave file\n"
           "\tUsage: update-s username oldReaction type newReaction\n");
    printf("\t---> insert-m - insert the record to the master file\n"
           "\tUsage: insert-m username email\n");
    printf("\t---> insert-s - insert the record to the slave file\n"
           "\tUsage: insert-s username reaction\n");
    printf("\t---> calc-m - show records count from the master file\n"
           "\tUsage: calc-m\n");
    printf("\t---> calc-s - show records count from the slave file\n"
           "\tUsage: calc-s\n");
    printf("\t---> ut-m - show all records from the master file\n"
           "\tUsage: ut-m\n");
    printf("\t---> ut-s - show all records from the slave file\n"
           "\tUsage: ut-s username\n");
    printf("\t---> exit - exit from database.\n"
           "\tUsage: exit\n");
}

void printBye() {
    printf("Thanks for using the database. Bye-bye!\n");
}

void insert_m(FILE *input) {
    char username[USERNAME_LENGTH];
    char email[USERNAME_LENGTH];
    fscanf(input, STANDARD_FORMAT, username);
    fscanf(input, STANDARD_FORMAT, email);
    insertUser(username, email);
    printf("User \"%s\" successfully inserted into master file.\n", username);
}

void insert_s(FILE *input) {
    char username[USERNAME_LENGTH];
    char reactionType[TYPE_LENGTH];
    fscanf(input, STANDARD_FORMAT, username);
    fscanf(input, STANDARD_FORMAT, reactionType);
    int position = getUserPosition(username);
    if (position == -1) {
        printf("User \"%s\" not found.\n", username);
        return;
    }
    int reactionPosition = getReactionPositionByUser(position, reactionType);
    if (reactionPosition != -1) {
        printf("Duplicate reaction \"%s\" for user \"%s\"", reactionType, username);
        return;
    }
    insertReactionToUser(position, reactionType);
    printf("Reaction \"%s\" successfully inserted into slave file.\n", reactionType);
}

void get_m(FILE *input) {
    char username[USERNAME_LENGTH];
    fscanf(input, STANDARD_FORMAT, username);
    int position = getUserPosition(username);
    if (position == -1) {
        printf("User \"%s\" not found.\n", username);
        return;
    }
    struct User user = getUser(position);
    printf(HEADER_FORMAT, "Found User");
    printf("User: %s\n", username);
    printf("Email: %s\n", user.email);
}

void get_s(FILE *input) {
    char username[USERNAME_LENGTH];
    char reactionType[TYPE_LENGTH];
    fscanf(input, STANDARD_FORMAT, username);
    fscanf(input, STANDARD_FORMAT, reactionType);
    int position = getUserPosition(username);
    if (position == -1) {
        printf("User \"%s\" not found.\n", username);
        return;
    }
    int reactionPosition = getReactionPositionByUser(position, reactionType);
    if (reactionPosition == -1) {
        printf("User \"%s\" doesn't have a reaction \"%s\".\n", username, reactionType);
        return;
    }
    struct Reaction reaction = getReaction(reactionPosition);
    printf(HEADER_FORMAT, "Found Reaction");
    printf("Type: \"%s\"\n", reaction.type);
}

void update_m(FILE *input) {
    char username[USERNAME_LENGTH];
    char fieldName[20];
    fscanf(input, STANDARD_FORMAT, username);
    fscanf(input, "%19s", fieldName);
    if (strcmp(fieldName, "email") == 0) {
        char email[EMAIL_LENGTH];
        fscanf(input, STANDARD_FORMAT, email);
        int position = getUserPosition(username);
        if (position == -1) {
            printf("User \"%s\" not found.\n", username);
            return;
        }
        updateUserEmail(position, email);
        printf("User \"%s\" successfully updated.\n", username);
        return;
    }
    if (strcmp(fieldName, "name") == 0) {
        printf("Cannot change the index field.\n");
        return;
    }
    printf("No field \"%s\" found in User table.\n", fieldName);
}

void update_s(FILE *input) {
    char username[USERNAME_LENGTH];
    char reactionType[TYPE_LENGTH];
    char field[20];
    fscanf(input, STANDARD_FORMAT, username);
    fscanf(input, STANDARD_FORMAT, reactionType);
    fscanf(input, "%19s", field);
    int position = getUserPosition(username);
    if (strcmp(field, "type") == 0) {
        char newType[TYPE_LENGTH];
        fscanf(input, STANDARD_FORMAT, newType);
        if (position == -1) {
            printf("User \"%s\" not found.\n", username);
            return;
        }
        int reactionPosition = getReactionPositionByUser(position, reactionType);
        if (reactionPosition == -1) {
            printf("No reaction \"%s\" is found for user \"%s\".\n",
                   reactionType, username);
            return;
        }
        updateReactionType(reactionPosition, newType);
        printf("Reaction \"%s\" successfully updated for user \"%s\".\n",
               reactionType, username);
        return;
    }
    printf("No field \"%s\" found in User table.\n", field);
}

void del_m(FILE *input) {
    char username[USERNAME_LENGTH];
    fscanf(input, STANDARD_FORMAT, username);
    int position = findUser(username);
    if (position == -1) {
        printf("User \"%s\" not found.\n", username);
        return;
    }
    deleteUser(position);
    printf("User \"%s\" successfully deleted.\n", username);
}

void del_s(FILE *input) {
    char username[USERNAME_LENGTH];
    char reactionType[TYPE_LENGTH];
    fscanf(input, STANDARD_FORMAT, username);
    fscanf(input, STANDARD_FORMAT, reactionType);
    int position = getUserPosition(username);
    if (position == -1) {
        printf("User \"%s\" not found.\n", username);
        return;
    }
    int status = findAndDeleteReaction(position, reactionType);
    if (status == -1) {
        printf("No reaction \"%s\" is found for user \"%s\".\n", reactionType, username);
        return;
    }
    printf("Reaction \"%s\" of user \"%s\" successfully deleted.\n", reactionType, username);
}

void calc_m() {
    printf("Users count: %d.\n", getActiveUsersCount());
}

void calc_s(FILE *input) {
    char username[USERNAME_LENGTH];
    fscanf(input, STANDARD_FORMAT, username);
    int link = getUserPosition(username);
    if (link == -1) {
        printf("User \"%s\" not found.\n", username);
        return;
    }
    int reactionsCount = getReactionsCountForUser(link);
    printf("Reactions count for user \"%s\": %d.\n", username, reactionsCount);
}

void ut_m() {
    printUsersData();
}

void ut_s() {
    printReactionsData();
}