/* COMP 3430
 * PROF: JIM YOUNG
 * REBECCA TIESSEN
 *
 * This file holds the linked list that the logs are stored in when created. There are standard insert and search/delete methods,
 * as well as traversals that use a static Node to keep track of the current place in the list. 
 *
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "log.h"
#include "llist.h"

static Node *top = NULL;
static Node *traverseNode = NULL;
static Node *toDelete = NULL;
static int logCount = 0;

bool insert(Log *newLog){
   bool success = false;
   Node *newNode = NULL;

   if(newLog != NULL){
      newNode = (Node *)malloc(sizeof(Node));
      if(newNode != NULL){
         newNode->log = newLog;
         newNode->next = top;
	 top = newNode;

	 logCount++;
	 success = true;
      }
      else{
         free(newNode);
         newNode = NULL;
      }
   }
   return success;
}

bool searchAndRemove(Log *log){ 
   bool deleted = false;
   Node *curr = top;
   Node *prev = NULL;

   if(curr != NULL){
      while(curr != NULL && curr->log != log){
         prev = curr;
	 curr = curr->next;
      }
      if(curr != NULL){
         if(prev != NULL){
	    prev->next = curr->next;
	 }
	 else{
	   top = curr->next;
	 }
	 free(curr->log);
	 curr->log = NULL;
	 free(curr);
	 curr = NULL;
	 deleted = true;
	 logCount--;
      }
   }
   return deleted;
}

Log *firstItem(){
   Log *first = NULL;
   if(top != NULL){
      traverseNode = top->next;
      first = top->log;
   }
   return first;
}

Log *nextAvailableLog(){ //for frog search method 
   Log *next = NULL;

   if(traverseNode != NULL){
      next = traverseNode->log;
      traverseNode = traverseNode->next;
   }
   return next;
}

bool listIsEmpty(){
   return top == NULL;
}

Log *cleanupNextLog(){ //for cleanupLogs method
   Log *potentialDelete = NULL;
   if(toDelete == NULL){
      toDelete = top;
   }
   if(toDelete != NULL){
      potentialDelete = toDelete->log;
      toDelete = toDelete->next;
   }
   return potentialDelete;
}

void deleteList(){
   Node *temp = NULL;
   while(top != NULL){
      temp = top;
      top = top->next;
      free(temp->log);
      free(temp);

   }
}
