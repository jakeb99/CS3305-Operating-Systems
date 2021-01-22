#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "types.h"

int num_accounts(FILE *fp);
int num_clients(FILE *fp);
int account_balance(char *line);
int read_input_file(char *filename, input_problem *inp);

void initialize_account_balances(FILE *fp, int *accounts, int na);
void initialize_client_actions(FILE *fp, client *clients, int nc, int na, input_problem *inp);

#endif