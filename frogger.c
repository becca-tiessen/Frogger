/* COMP 3430
 * PROF: JIM YOUNG
 * REBECCA TIESSEN
 *
 * This file takes care of the main game logic, and setting up initializer 
 * methods. It also keeps track of player lives
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "console.h"
#include "frogger.h"
#include "threadwrappers.h"
#include "player.h"
#include "gameglobals.h"
#include "log.h"
#include "llist.h"

//-------------------------------------------------------------------//
int main(int argc, char**argv) {
  startGame();
  printf("done!\n");
}

void startGame(){
   //initialize all mutexes and condLock
   initLocks();
   if(drawScreen()){
      createThread(&tids[getThreadCount()], updateLives, NULL);
      createThread(&tids[getThreadCount()], refreshScreen, NULL);
      initializePlayer();
      initializeLogs();

      lockMutex(&mainLock);
      while(!isGameOver()){
         pthread_cond_wait(&condLock, &mainLock);
      }
      unlockMutex(&mainLock);
   }
   finalKeypress();
   int i;
   for(i = 0; i < getThreadCount(); i++){
      joinThread(tids[i]);
   }
   destroyLocks();
   free(getFrog());
   deleteList();
   consoleFinish();
}

void *refreshScreen(){
   while(!isGameOver()){
      lockMutex(&screenLock);
      consoleRefresh();
      unlockMutex(&screenLock);
      sleepTicks(1); 
   }
   pthread_exit(NULL);
}

void *updateLives(){
   Frog *frog = getFrog();
   int lifeCount = MAX_LIVES;
   char strLives[2];

   while(frog == NULL){
      frog = getFrog(); //deals with initialization of thread before frog is created
   }
   while(!isGameOver()){
      if(frog->dead){
         lifeCount--;
         sprintf(strLives, "%d", lifeCount);
         lockMutex(&screenLock);
         putString(strLives, 0, 42, 1);
         unlockMutex(&screenLock);

	 lockMutex(&playerLock);
	 frog->dead = false;
	 unlockMutex(&playerLock);
      }
      sleepTicks(1);
      if(lifeCount <= 0){
	 endGame("GAME OVER");
      }
   }
   pthread_exit(NULL);
}

void endGame(char *endMsg){
   lockMutex(&screenLock);
   putBanner(endMsg);
   disableConsole(1);
   unlockMutex(&screenLock);

   lockMutex(&mainLock);
   setGameOver();
   pthread_cond_signal(&condLock);
   unlockMutex(&mainLock);
}
