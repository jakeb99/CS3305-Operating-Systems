#define FILENAME_OUTPUT "assignment_6_output_file.txt"

#include<string.h>
#include<pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "io.h"
#include "types.h"

void *do_transactions(void *actions);

int *account_balances;
pthread_mutex_t lock;

/** 
 * This method performs the client transactions.
 * critical section contained in this method.
 */
void *do_transactions(void *actions)
{
	pthread_mutex_lock(&lock);  		// ENTRY REGION

	char *client_actions = actions;		// string of client transactions to make
	char *token;						// a token of the line of client actions

	int account1 = 0;					// primary account to widthraw from/deposite too/transfer from 
	int account2 = 0;					// secondary account to transfer too
	int amount = 0;						// amount for transaction
	int balCheck = 0;					// stores balance of the account we want to withdraw/transfer from

	token = strtok(client_actions, " ");		// get first token of string split by " "

	while (token != NULL)
	{
		token = strtok(NULL, " ");		// get next token

		if (token != NULL)
		{
			// make a deposite
			if (token[0] == 'd')
			{
				// get account num to deposite to
				token = strtok(NULL, " ");
				account1 = (int) token[1] - 48;		// convert char to integer

				// get amount to deposite
				token = strtok(NULL, " ");
				amount = atoi(token);				// convert to integer

				// Update account balance
				account_balances[account1 - 1] += amount;
			}
			// make a withdrawl
			else if (token[0] == 'w')
			{
				// get account num to withdraw from
				token = strtok(NULL, " ");
				account1 = (int) token[1] - 48;		// convert char to integer

				// get amount to withdraw
				token = strtok(NULL, " ");
				amount = atoi(token);				// convert to integer

				// check account has sufficient balance
				balCheck = account_balances[account1 - 1];
				if (amount > balCheck)
					continue;
				else
					account_balances[account1 - 1] -= amount;		// withdraw amount
			}
			// make a transfer
			else if (token[0] == 't')
			{
				// get account to transfer from
				token = strtok(NULL, " ");
				account1 = (int) token[1] - 48;		// convert char to integer

				// get account to transfer to
				token = strtok(NULL, " ");
				account2 = (int) token[1] - 48;		// convert char to integer

				// get ammount to transfer
				token = strtok(NULL, " ");
				amount = atoi(token);				// convert to integer

				// cheack account1 has sufficient balance
				balCheck = account_balances[account1 - 1];
				if (amount > balCheck)
					continue;
				else
				{
					account_balances[account1 - 1] -= amount;
					account_balances[account2 - 1] += amount;
				}
			}
			else
				continue;
		}
	}
	pthread_mutex_unlock(&lock); 		// EXIT REGION
}

int main(int argc, char* argv[])
{
	int i, err_thead;
	FILE *outFile;
	outFile = fopen(FILENAME_OUTPUT, "w");	// create output file to write results to

	if (argc != 2){
		printf("Usage: %s input.txt\n", argv[0]);
		exit(1);
	}

    input_problem inp;					// create structure to store input file data
    read_input_file(argv[1], &inp);		// parse the input file

	int na = inp.num_accounts;			// number of accounts from input file
	int nc = inp.num_clients;			// number of clients from input file
	account_balances = inp.accounts;	// global array of balances of the accounts
	client *clients = inp.clients;		// array of clients, client struct stores each clients 
										// number of transactions and transactions to make
	
	pthread_t threads[nc];
	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		printf("\n mutex init failed");
		return 1;
	}

	// create a thread for each client, calling the do_transactions function for each thread
	for (int i = 0; i < nc; i++)
		err_thead = pthread_create(&threads[i], NULL, &do_transactions, clients[i].action_input);
	if (err_thead != 0)
		printf("\n Error creating thread %d", i);

	// join threads
	for (int i = 0; i< nc; i++)
        pthread_join(threads[i], NULL);

	// print balances to stdout
	for (int i = 0; i < na; i++)
		printf("a%d b %d\n", i+1, account_balances[i]);

	// write balances to outfile
	for (int i = 0; i <na; i++)
		fprintf(outFile, "a%d b %d\n", i+1, account_balances[i]);

	// free memory
	free(account_balances);
	for (int i = 0; i < nc; i++)
		free(clients[i].action_input);
	free(clients);
	fclose(outFile);
	
	return 0;
}
