#ifndef DATABASE_DATABASE_H
#define DATABASE_DATABASE_H

#include <stdio.h>
#include <string.h>

#include "user.h"
#include "reaction.h"

#define HEADER_FORMAT "=============%s=============\n"

void clearDatabase();

int userFileSize();

int reactionFileSize();

struct UserIndex getUserIndex(size_t position);

void updateUserIndex(size_t position, struct UserIndex userIndex);

int findUser(char *name);

int findFreeUser();

void insertUser(char *name, char *email);

int getUserPosition(char *name);

struct User getUser(int link);

int getReactionPositionByUser(int userPosition, char *reactionType);

struct Reaction getReaction(int link);

void insertReaction(struct Reaction reaction);

void updateUser(int userPosition, struct User user);

void updateReaction(size_t reactionPosition, struct Reaction reaction);

int getFreeReaction();

void insertReactionToUser(int userPosition, char *reactionType);

void updateUserEmail(int userPosition, const char *newEmail);

void updateReactionType(int reactionPosition, const char *newType);

void deleteReactions(int position);

void deleteUser(int position);

int findAndDeleteReaction(int userPosition, char *reactionType);

int getActiveUsersCount();

int getActiveReactionsCount();

int getReactionsCountForUser(int position);

void printUserIndex();

void printUsers();

void printUsersData();

void printReactionsData();


#endif //DATABASE_DATABASE_H
