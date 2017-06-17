/* COMP 3430
 * PROF: JIM YOUNG
 * REBECCA TIESSEN
 *
 * This file is a wrapper for the pthread methods in order to check for errors. An error will print and the program will exit if 
 * a method errors.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "threadwrappers.h"
#include "gameglobals.h"

void createThread(pthread_t *thread, void *(*func)(void *), void *param){
   int ret;
   ret = pthread_create(thread, NULL, func, param);
   if(ret){
      printError();
   }
   lockMutex(&threadCountLock);
   incrementThreadCount();
   unlockMutex(&threadCountLock);
}

void joinThread(pthread_t thread){
  int ret;
  ret = pthread_join(thread, NULL);
  if(ret){
     printError();
  }
  lockMutex(&threadCountLock);
  decrementThreadCount();
  unlockMutex(&threadCountLock);
}

void createLogThread(pthread_t *thread, void *(*func)(void *), void *param){
   int ret;
   ret = pthread_create(thread, NULL, func, param);
   if(ret)
      printError();
}

void joinLogThread(pthread_t thread){
   int ret;
   ret = pthread_join(thread, NULL);
   if(ret)
      printError();
}

void lockMutex(pthread_mutex_t *lock){
   int ret;
   ret = pthread_mutex_lock(lock);
   if(ret){
      printError();
   }
}

void unlockMutex(pthread_mutex_t *lock){
   int ret;
   ret = pthread_mutex_unlock(lock);
   if(ret){
      printError();
   }
}

void printError(){
   fprintf(stderr, "THREAD ERROR\n");
   exit(1);
}
