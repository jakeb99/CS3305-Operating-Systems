#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "io.h"

/** 
 * counts number of accounts by counting
 * the number of lines that start with 'a'
 */
int num_accounts(FILE *fp)
{
    int count = 0;
    char c;

    for (c = getc(fp); c != EOF; c = getc(fp))
    {
        if (c == 'a')
        {
            count++;    // increment counter
            // loop to the end of the line
            for (c = getc(fp); c != '\n'; c = getc(fp)){}
        }

        // if c is 'c' we have reached the clients portion of the input file
        if (c == 'c')
            break;
    }
    fseek(fp, 0, SEEK_SET);     // point fp back to start of file
    return count;
}

/** 
 * counts number of clients by counting
 * the number of lines that start with 'c'
 */
int num_clients(FILE *fp)
{
    int count = 0;
    char c;

    for (c = getc(fp); c != EOF; c = getc(fp))
    {
        // if c is 'a' we go to the next line
        if (c == 'a')
        {
            // loop to the end of the line
            for (c = getc(fp); c != '\n'; c = getc(fp)){}
            continue; // get next character
        }

        if (c == 'c')
        {
            count++;    // increment counter
            // loop to the end of the line
            for (c = getc(fp); c != '\n'; c = getc(fp)){}
        }

    }
    fseek(fp, 0, SEEK_SET);     // point fp back to start of file
    return count;
}

/** 
 * Seperate each line using " " as a delimeter, return the last
 * token of the line which is the balance for the account.
 */
int account_balance(char *line)
{
    char *token = strtok(line, " ");    // break line into tokens

    // loop to the third token, which is the account balance
    for (int i = 0; i < 2; i++)
        token = strtok(NULL, " ");
        
    return atoi(token);     // convert balance to integer type and return
}

/** 
 * Sets the initial balances for each account.
 */
void initialize_account_balances(FILE *fp, int *accounts, int na)
{
    char *buffer = NULL;
    size_t len = 0;

    int ai = 0;     // account index
    // while account balances left to parse
    while (ai < na)
    {   
        // get the next line if there is one
        if (getline(&buffer, &len, fp) != -1)
        {
            int balance = account_balance(buffer);      // parse the line for the balance
            accounts[ai] = balance;                     // set the balance for the account
            ai++; 
        }
    }
    fseek(fp, 0, SEEK_SET);
    free(buffer);
}

/** 
 * Store transaction input in the array of client structures
 */
void initialize_client_actions(FILE *fp, client *clients, int nc, int na, input_problem *inp)
{
    char *buffer = NULL;
    size_t len = 0;

    // iterate through the account input lines
    for (int i = 0; i < na; i++)
        getline(&buffer, &len, fp);
    
    int ci = 0;     // client index

    // while client lines left
    int size;
    char *line;
    while (size = getline(&buffer, &len, fp) != -1)
    {   
        line = strdup(buffer);                                              // save string in buffer to line
        size = strlen(line);                                                // calculate length of string
        clients[ci].action_input = malloc(sizeof(char) * size);             // allocate enough memory to hold input line
        clients[ci].action_input = line;                                    // store the line of input in the client struct

        ci++;
    }
    free(buffer);
}

/** 
 * Parses the input file and stores neccessary info in the input_problem structure.
 */
int read_input_file(char *filename, input_problem *inp)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror(filename);
        exit(1);
    }

    // from the input file get the number of accounts and the number of clients
    int na = num_accounts(fp);
    int nc = num_clients(fp);

    int *accounts = malloc(sizeof(int) * na);           // allocate memory for accounts array
    initialize_account_balances(fp, accounts, na);      // add ballances to accounts array

    // initialize client info
    client *clients = malloc(sizeof(client) * nc);        // allocate memory for clients array

    // get transaction input
    initialize_client_actions(fp, clients, nc, na, inp);

    inp->num_accounts = na;
    inp->num_clients = nc;
    inp->accounts = accounts;
    inp->clients = clients;

    fclose(fp);
    
}

