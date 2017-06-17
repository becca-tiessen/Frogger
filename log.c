/* COMP 3430
 * PROF: JIM YOUNG
 * REBECCA TIESSEN
 *
 * This file takes care of animating, moving, and spawning logs. The frog is allowed to jump on the logs to get to the home bank.
 *
*/

#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "console.h"
#include "threadwrappers.h"
#include "gameglobals.h"
#include "llist.h"
#include "player.h"

#define LOG_ANIM_TILES 2 
#define LOG_HEIGHT 4
#define NUM_ROWS 4

Frog *frog;
static int log_width;
static char* LOG_GRAPHIC[LOG_ANIM_TILES][LOG_HEIGHT+1] = {
   {"/======================\\",
    "|                      |",
    "|                      |",
   "\\======================/"},
   {"/======================\\",
    "-                      +",
    "+                      -",
   "\\======================/"}
};
//---PROTOTYPES------------------------------------------------//
static void drawLog();
static void setLogWidth();
//---METHODS---------------------------------------------------//
void initializeLogs(){
   frog = getFrog();
   setLogWidth();

   enum row rows[] = {one, two, three, four};
   int i;
   for(i = 0; i < NUM_ROWS; i++){
      void *row = malloc(sizeof(int));
      *((int*)row) = rows[i];
      createThread(&tids[getThreadCount()], runLogs, (void *)row);
   }

   createThread(&tids[getThreadCount()], cleanUpLogs, NULL);
}

void *runLogs(void *startRow){
   int *row = (int *)startRow;
   while(!isGameOver()){
      Log *newLog = (Log *)malloc(sizeof(Log));
      logStartup(newLog, row);
      lockMutex(&listLock);
      insert(newLog);
      unlockMutex(&listLock);

      createLogThread(&(newLog->threadID), logController, (void *)newLog);
      sleepTicks((rand()%200)+150); //random log generation speed
     }
   pthread_exit(NULL);
}

void *logController(void *currLog){
   Log *log = (Log *)currLog; 
   while(!isGameOver() && !log->dead){
      if(log->hasFrog){
	 moveFrogAndLog(log);
	 moveFrogAndLog(log);
      }
      else{
         moveLog(log);
         moveLog(log);
         animateLog(log);
      }
      sleepTicks(log->speed);
   }
   pthread_exit(NULL);
}

void moveLog(Log *log){
   if(log->direction == left){
      log->prevCol = log->currCol;
      log->currCol--;
   }
   else{
      log->prevCol = log->currCol;
      log->currCol++;
   }
   drawLog(log);
   checkIsDead(log);
}

void animateLog(Log *log){
   if(log->animateState == first)
      log->animateState = second;
   else
      log->animateState = first;
   drawLog(log);
}

static void drawLog(Log *log){
   char** tile = LOG_GRAPHIC[log->animateState];
   
   lockMutex(&screenLock);
   consoleClearImage(log->startRow, log->prevCol, LOG_HEIGHT, log_width);
   consoleDrawImage(log->startRow, log->currCol, tile, LOG_HEIGHT);
   unlockMutex(&screenLock);
}

void moveFrogAndLog(Log *log){
   moveLog(log);
   animateLog(log);
   if(log->direction == left)
      moveFrog(LEFT_KEY);
   else
      moveFrog(RIGHT_KEY);
}

void *cleanUpLogs(){
   Log *curr = NULL;
   Log *logToDelete = NULL;
   while(!isGameOver()){
      lockMutex(&listLock);
      curr = cleanupNextLog();
      unlockMutex(&listLock);
      while(curr != NULL && !isGameOver()){
         if(curr->dead){
            joinLogThread(curr->threadID);
            logToDelete = curr;
         }

         if(logToDelete != NULL){
            lockMutex(&listLock);
            searchAndRemove(logToDelete);
            unlockMutex(&listLock);
            logToDelete = NULL;
         }
         lockMutex(&listLock);
         curr = cleanupNextLog();
         unlockMutex(&listLock);
	 sleepTicks(1);
      }
      unlockMutex(&listLock);
      sleepLoop(100);
   }
   pthread_exit(NULL);
}

void logStartup(Log *log, int *startRow){ 
   log->startRow = (enum row)*startRow;
   setLogSpeed(log, startRow);
   setDirection(log);
   log->width = log_width;
   log->height = LOG_HEIGHT;
   log->animateState = first;

   if(log->direction == left){
      log->prevCol = log->currCol = RIGHT_EDGE;
   }
   else{
      log->prevCol = log->currCol = LEFT_EDGE-log->width;
   }
   log->dead = false;
   log->hasFrog = false;
}

void setDirection(Log *log){
   if(log->startRow%8 == 0) //alternate every other row
      log->direction = left;
   else
      log->direction = right;
}

void setLogSpeed(Log *log, int *startRow){
   enum row currRow = (enum row)*startRow;
   if(currRow == one)
      log->speed = 5;
   else if(currRow == two)
      log->speed = 7;
   else if(currRow == three)
      log->speed = 10; 
   else
      log->speed = 12;
}

void checkIsDead(Log *log){
   if(log->currCol > RIGHT_EDGE || log->currCol < LEFT_EDGE-log->width){
      log->dead = true;
   }
}

void setLogWidth(){
   char **tile = LOG_GRAPHIC[0];
   log_width = strlen(tile[0]);
}

