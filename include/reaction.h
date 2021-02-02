#ifndef DATABASE_REACTION_H
#define DATABASE_REACTION_H

#include <stdio.h>
#include <stdbool.h>

#define SLAVE_FILE "../data/SP.fl"

#define TYPE_LENGTH 40

struct Reaction {
    char type[TYPE_LENGTH];
    int nextReactionPosition;
    bool isActive;
};

#endif //DATABASE_REACTION_H