#ifndef DATABASE_USER_H
#define DATABASE_USER_H

#include <stdio.h>
#include <stdbool.h>


#define USERNAME_LENGTH 40
#define EMAIL_LENGTH 40


#define MASTER_INDEX "../data/S.ind"
#define MASTER_FILE "../data/S.fl"


struct User {
    char email[EMAIL_LENGTH];
    int firstReactionPosition;
};

struct UserIndex {
    char username[USERNAME_LENGTH];
    size_t position;
    bool isActive;
};

#endif //DATABASE_USER_H