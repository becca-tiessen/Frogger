/* COMP 3430
 * PROF: JIM YOUNG
 * REBECCA TIESSEN
 * 
 * This file includes methods used by all files in order to check for game over, thread count, and setting up locks.
 *
 */

#include <pthread.h>
#include <stdbool.h>
#include "gameglobals.h"
#include "console.h"

static bool gameOver = false;
static int threadCount = 0;
char *GAME_BOARD[] = {
"                                   Lives: 4",
"/------\\          /------\\          /------\\          /------\\          /------\\",
"|      |          |      |          |      |          |      |          |      |",
"+      +----------+      +----------+      +----------+      +----------+      +",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"",
"",
"",
"" };

bool drawScreen(){
   return consoleInit(GAME_ROWS, GAME_COLS, GAME_BOARD);
}

void initLocks(){
   // Mutex locks
   pthread_mutex_init(&screenLock, NULL);
   pthread_mutex_init(&playerLock, NULL);
   pthread_mutex_init(&mainLock, NULL);
   pthread_mutex_init(&threadCountLock, NULL);
   pthread_mutex_init(&listLock, NULL);
   // Conditional variable lock
   pthread_cond_init(&condLock, NULL);
}

void destroyLocks(){
   pthread_mutex_destroy(&screenLock);
   pthread_mutex_destroy(&playerLock);
   pthread_mutex_destroy(&mainLock);
   pthread_mutex_destroy(&threadCountLock);
   pthread_mutex_destroy(&listLock);

   pthread_cond_destroy(&condLock);

}

void setGameOver(){
   gameOver = true;
}

bool isGameOver(){
   return gameOver;
}

int getThreadCount(){
   return threadCount;
}

void incrementThreadCount(i){
   threadCount++;
}

void decrementThreadCount(){
   threadCount--;
}

void sleepLoop(int numTicks){
   int i;
   for(i = 0; i < numTicks && !isGameOver(); i++){
      sleepTicks(i);
   }
}
