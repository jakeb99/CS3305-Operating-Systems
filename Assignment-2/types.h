#ifndef TYPES_H
#define TYPES_H

typedef struct input_problem
{
    int num_accounts;
    int num_clients;
    int *accounts;
    struct client *clients;

} input_problem;

typedef struct client
{
    char *action_input;

} client;

#endif
