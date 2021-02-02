#include <stdio.h>
#include <string.h>

#include "../include/database.h"

void clearDatabase() {
    FILE *masterIndex = fopen(MASTER_INDEX, "w");
    fclose(masterIndex);

    FILE *masterFile = fopen(MASTER_FILE, "w");
    fclose(masterFile);

    FILE *slaveFile = fopen(SLAVE_FILE, "w");
    fclose(slaveFile);
}

int userFileSize() {
    FILE *masterFile = fopen(MASTER_FILE, "r");
    fseek(masterFile, 0, SEEK_END);
    long long res = ftell(masterFile);
    fclose(masterFile);
    return (int) res;
}

int reactionFileSize() {
    FILE *slaveFile = fopen(SLAVE_FILE, "r");
    fseek(slaveFile, 0, SEEK_END);
    long long res = ftell(slaveFile);
    fclose(slaveFile);
    return (int) res;
}

struct UserIndex getUserIndex(size_t position) {
    FILE *masterFile = fopen(MASTER_INDEX, "r+");
    fseek(masterFile, position, SEEK_SET);
    struct UserIndex userIndex;
    fread((char *) &userIndex, sizeof(userIndex), 1, masterFile);
    fclose(masterFile);
    return userIndex;
}

void updateUserIndex(size_t position, struct UserIndex userIndex) {
    FILE *masterIndex = fopen(MASTER_INDEX, "r+");
    fseek(masterIndex, position, SEEK_SET);
    fwrite((char *) &userIndex, sizeof(userIndex), 1, masterIndex);
    fclose(masterIndex);
}

int findUser(char *name) {
    FILE *masterIndex = fopen(MASTER_INDEX, "r");
    struct UserIndex userIndex;
    int resultPosition = -1;
    while (true) {
        int position = (int) ftell(masterIndex);
        if (fread((char *) &userIndex, sizeof(userIndex), 1, masterIndex) != 1)
            break;
        if (userIndex.isActive == true && strcmp(userIndex.username, name) == 0) {
            resultPosition = position;
            break;
        }
    }
    fclose(masterIndex);
    return resultPosition;
}

int findFreeUser() {
    FILE *masterIndex = fopen(MASTER_INDEX, "r");
    int position = -1;
    while (true) {
        int currentPosition = (int) ftell(masterIndex);
        struct UserIndex userIndex;
        if (fread((char *) &userIndex, sizeof(userIndex), 1, masterIndex) != 1)
            break;
        if (userIndex.isActive == false) {
            position = currentPosition;
            break;
        }
    }
    fclose(masterIndex);
    return position;
}

void insertUser(char *name, char *email) {
    int freePosition = findFreeUser();
    if (freePosition != -1) {
        int data_place;

        struct UserIndex freeIndex = getUserIndex(freePosition);
        data_place = (int) freeIndex.position;
        freeIndex.isActive = true;
        strcpy(freeIndex.username, name);
        updateUserIndex(freePosition, freeIndex);

        struct User updatedUser;
        strcpy(updatedUser.email, email);
        updatedUser.firstReactionPosition = -1;

        FILE *userData = fopen(MASTER_FILE, "r+");
        fseek(userData, data_place, SEEK_SET);
        fwrite((char *) &updatedUser, sizeof(updatedUser), 1, userData);
        fclose(userData);

        return;
    }

    struct UserIndex newUserIndex;
    strcpy(newUserIndex.username, name);
    newUserIndex.position = userFileSize();
    newUserIndex.isActive = true;
    FILE *au_id = fopen(MASTER_INDEX, "a");
    fwrite((char *) &newUserIndex, sizeof(newUserIndex), 1, au_id);
    fclose(au_id);

    struct User newUser;
    strcpy(newUser.email, email);
    newUser.firstReactionPosition = -1;

    FILE *newUserData = fopen(MASTER_FILE, "a");

    fwrite((char *) &newUser, sizeof(newUser), 1, newUserData);
    fclose(newUserData);
}

int getUserPosition(char *name) {
    FILE *masterIndex = fopen(MASTER_INDEX, "r");
    struct UserIndex userIndex;
    int result = -1;
    while (fread((char *) &userIndex, sizeof(userIndex), 1, masterIndex) == 1) {
        if (userIndex.isActive && strcmp(userIndex.username, name) == 0) {
            result = (int) userIndex.position;
            break;
        }
    }
    fclose(masterIndex);
    return result;
}

struct User getUser(int link) {
    FILE *masterFile = fopen(MASTER_FILE, "r");
    fseek(masterFile, link, SEEK_SET);
    struct User user;
    fread((char *) &user, sizeof(user), 1, masterFile);
    fclose(masterFile);
    return user;
}

int getReactionPositionByUser(int userPosition, char *reactionType) {
    int firstReaction;
    struct User user = getUser(userPosition);
    firstReaction = (int) user.firstReactionPosition;

    FILE *slaveFile = fopen(SLAVE_FILE, "r");
    int resultPosition = -1;
    struct Reaction reaction;
    while (firstReaction != -1) {

        fseek(slaveFile, firstReaction, SEEK_SET);
        fread((char *) &reaction, sizeof(reaction), 1, slaveFile);

        if (reaction.isActive && strcmp(reaction.type, reactionType) == 0) {
            resultPosition = firstReaction;
            break;
        }
        firstReaction = reaction.nextReactionPosition;
    }
    fclose(slaveFile);
    return resultPosition;
}

struct Reaction getReaction(int link) {
    FILE *slaveFile = fopen(SLAVE_FILE, "r+");
    fseek(slaveFile, link, SEEK_SET);
    struct Reaction reaction;
    fread((char *) &reaction, sizeof(reaction), 1, slaveFile);
    fclose(slaveFile);
    return reaction;
}

void insertReaction(struct Reaction reaction) {
    FILE *slaveFile = fopen(SLAVE_FILE, "a");
    fseek(slaveFile, 0, SEEK_END);
    fwrite((char *) &reaction, sizeof(reaction), 1, slaveFile);
    fclose(slaveFile);
}

void updateUser(int userPosition, struct User user) {
    FILE *masterFile = fopen(MASTER_FILE, "r+");
    fseek(masterFile, userPosition, SEEK_SET);
    fwrite((char *) &user, sizeof(user), 1, masterFile);
    fclose(masterFile);
}

void updateReaction(size_t reactionPosition, struct Reaction reaction) {
    FILE *slaveFile = fopen(SLAVE_FILE, "r+");
    fseek(slaveFile, reactionPosition, SEEK_SET);
    fwrite((char *) &reaction, sizeof(reaction), 1, slaveFile);
    fclose(slaveFile);
}

int getFreeReaction() {
    FILE *slaveFile = fopen(SLAVE_FILE, "r+");
    int result = -1;
    while (true) {
        int currentPosition = (int) ftell(slaveFile);
        struct Reaction reaction;
        if (fread((char *) &reaction, sizeof(reaction), 1, slaveFile) != 1)
            break;
        if (reaction.isActive == false) {
            result = currentPosition;
            break;
        }
    }
    fclose(slaveFile);
    return result;
}

void insertReactionToUser(int userPosition, char *reactionType) {
    struct User user = getUser(userPosition);
    int firstReaction = user.firstReactionPosition;
    int freeReaction = getFreeReaction();

    if (freeReaction != -1) {
        user.firstReactionPosition = freeReaction;
        struct Reaction newReaction;
        newReaction.isActive = true;

        strcpy(newReaction.type, reactionType);

        newReaction.nextReactionPosition = firstReaction;

        updateReaction(freeReaction, newReaction);
    } else {
        user.firstReactionPosition = reactionFileSize();
        struct Reaction newReaction;
        newReaction.isActive = true;

        strcpy(newReaction.type, reactionType);

        newReaction.nextReactionPosition = firstReaction;
        insertReaction(newReaction);
    }
    updateUser(userPosition, user);
}

void updateUserEmail(int userPosition, const char *newEmail) {
    struct User user = getUser(userPosition);
    strcpy(user.email, newEmail);
    updateUser(userPosition, user);
}

void updateReactionType(int reactionPosition, const char *newType) {
    struct Reaction reaction = getReaction(reactionPosition);
    strcpy(reaction.type, newType);
    updateReaction(reactionPosition, reaction);
}

void deleteReactions(int position) {
    struct User user = getUser(position);
    int firstReaction = user.firstReactionPosition;
    while (firstReaction != -1) {
        struct Reaction currentReaction = getReaction(firstReaction);
        currentReaction.isActive = false;
        updateReaction(firstReaction, currentReaction);
        firstReaction = currentReaction.nextReactionPosition;
    }
}

void deleteUser(int position) {
    FILE *masterIndex = fopen(MASTER_INDEX, "r+");
    fseek(masterIndex, position, SEEK_SET);
    struct UserIndex user;
    fread((char *) &user, sizeof(user), 1, masterIndex);
    user.isActive = false;
    deleteReactions((int) user.position);
    fseek(masterIndex, position, SEEK_SET);
    fwrite((char *) &user, sizeof(user), 1, masterIndex);
    fclose(masterIndex);
}

int findAndDeleteReaction(int userPosition, char *reactionType) {
    int lastReaction = -1;
    int currentReaction = -1;

    struct User user = getUser(userPosition);
    currentReaction = (int) user.firstReactionPosition;

    FILE *slaveFile = fopen(SLAVE_FILE, "r");
    struct Reaction reaction;
    int status = -1;
    while (currentReaction != -1) {
        fseek(slaveFile, currentReaction, SEEK_SET);
        fread((char *) &reaction, sizeof(reaction), 1, slaveFile);

        if (reaction.isActive && strcmp(reaction.type, reactionType) == 0) {
            reaction.isActive = false;
            updateReaction(currentReaction, reaction);
            if (lastReaction == -1) {
                struct User user = getUser(userPosition);
                user.firstReactionPosition = reaction.nextReactionPosition;
                updateUser(userPosition, user);
            } else {
                struct Reaction previousReaction = getReaction(lastReaction);
                previousReaction.nextReactionPosition = reaction.nextReactionPosition;
                updateReaction(lastReaction, previousReaction);
            }
            status = 1;
            break;
        }
        lastReaction = currentReaction;
        currentReaction = reaction.nextReactionPosition;
    }
    fclose(slaveFile);
    return status;
}

int getActiveUsersCount() {
    FILE *masterIndex = fopen(MASTER_INDEX, "r");
    int result = 0;
    struct UserIndex userIndex;
    while (fread((char *) &userIndex, sizeof(userIndex), 1, masterIndex) == 1) {
        if (userIndex.isActive) ++result;
    }
    fclose(masterIndex);
    return result;
}

int getActiveReactionsCount() {
    FILE *slaveFile = fopen(SLAVE_FILE, "r");
    int result = 0;
    struct Reaction reaction;
    while (fread((char *) &reaction, sizeof(reaction), 1, slaveFile) == 1) {
        if (reaction.isActive) ++result;
    }
    fclose(slaveFile);
    return result;
}

int getReactionsCountForUser(int position) {
    int firstReaction = -1;
    {
        struct User user = getUser(position);
        firstReaction = user.firstReactionPosition;
    }
    int result = 0;
    while (firstReaction != -1) {
        ++result;
        struct Reaction reaction = getReaction(firstReaction);
        firstReaction = reaction.nextReactionPosition;
    }
    return result;
}

void printUserIndex() {
    FILE *masterIndex = fopen(MASTER_INDEX, "r");
    printf(HEADER_FORMAT, "User Index");
    int counter = 0;
    while (true) {
        struct UserIndex userIndex;
        if (fread((char *) &userIndex, sizeof(userIndex), 1, masterIndex) != 1)
            break;
        printf("=> Number %d\n", counter);
        printf("   Username = %s\n", userIndex.username);
        printf("   Position = %zu\n", userIndex.position);
        printf("   Active = %s\n", userIndex.isActive ? "true" : "false");

        counter++;
    }
    fclose(masterIndex);
}

void printUsers() {
    FILE *masterFile = fopen(MASTER_FILE, "r");
    printf(HEADER_FORMAT, "User");
    int counter = 0;
    while (true) {
        struct User user;
        if (fread((char *) &user, sizeof(user), 1, masterFile) != 1) break;
        printf("=> Number %d\n", counter);
        printf("   Email = %s\n", user.email);
        printf("   First reaction = %d\n", user.firstReactionPosition);
        counter++;
    }
    fclose(masterFile);
}

void printUsersData() {
    printUserIndex();
    printf("\n");
    printUsers();
}

void printReactionsData() {
    FILE *slaveFile = fopen(SLAVE_FILE, "r");
    printf(HEADER_FORMAT, "Reactions");
    int counter = 0;
    while (true) {
        struct Reaction reaction;
        if (fread((char *) &reaction, sizeof(reaction), 1, slaveFile) != 1)
            break;

        printf("=> Number %d\n", counter);
        printf("   Type = %s\n", reaction.type);
        printf("   Next reaction = %d\n", reaction.nextReactionPosition);
        printf("   Active = %s\n", reaction.isActive ? "true" : "false");

        counter++;
    }
    fclose(slaveFile);
}
