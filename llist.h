/* The header file for llist.c
*/

#ifndef LLIST_H
#define LLIST_H
#include <stdbool.h>

typedef struct NODE Node;
struct NODE {
   Log *log;
   Node *next;
};

/* Inserts a log into the list */
bool insert(Log *log);

/* Searches for a log in the list and removes it */
bool searchAndRemove(Log *log);

/* Checks if list is empty */
bool listIsEmpty();

/* Returns the first log in the list to start traversal */
Log *firstItem();

/* Continues traversal with the next available log */ 
Log *nextAvailableLog();

/* Continues a traversal for the log cleanup */
Log *cleanupNextLog();

/* Cleans up log list at end of program */
void deleteList();

#endif

