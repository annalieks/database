#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H

void chooseCommand(FILE *input, char *command);

void printGreeting();

void printBye();

void insert_m(FILE* input);

void insert_s(FILE* input);

void get_m(FILE* input);

void get_s(FILE* input);

void update_m(FILE* input);

void update_s(FILE* input);

void del_m(FILE* input);

void del_s(FILE* input);

void calc_m();

void calc_s(FILE* input);

void ut_m();

void ut_s();

#endif //DATABASE_INTERFACE_H
