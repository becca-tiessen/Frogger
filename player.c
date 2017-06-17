/* COMP 3430
 * PROF: JIM YOUNG
 * REBECCA TIESSEN
 * 
 * Player.c includes all the logic required for the frog to move around, animate, and die if it hits the water. 
 * It also keeps track of the number of safe pods you've made it to.
 *
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/select.h> //for keyboard functinonality
#include <stdlib.h>

#include "player.h"
#include "threadwrappers.h"
#include "console.h"
#include "gameglobals.h"
#include "log.h"
#include "llist.h"
#include "frogger.h"

#define PLAYER_ANIM_TILES 2
#define PLAYER_HEIGHT 2
#define F_START_ROW 21
#define F_START_COL 40
#define VERTICAL_JUMP 4
#define SIDE_JUMP 1
#define HOME_JUMP 3

static char* PLAYER_GRAPHIC[PLAYER_ANIM_TILES][PLAYER_HEIGHT+1] = {
  {"@@",
   "<>"},
  {"--",
   "<>"}
};
static const int SAFE_POD_1 = 0;
static const int SAFE_POD_2 = 18;
static const int SAFE_POD_3 = 36;
static const int SAFE_POD_4 = 54;
static const int SAFE_POD_5 = 72;

Frog *frog; //global frog

//---PROTOTYPES---------------------------------------------------------
static void updatePrevious();
static void drawFrog();
//---METHODS------------------------------------------------------------//

void initializePlayer(){
   frog = (Frog *)malloc(sizeof(Frog));
   createFrog();
   createThread(&tids[getThreadCount()], animateFrog, NULL);
   createThread(&tids[getThreadCount()], initMovement, NULL);
} 

void *animateFrog(){
   while(!isGameOver()){
      lockMutex(&playerLock);
      if(frog->animateState == first)
         frog->animateState = second;
      else
         frog->animateState = first;
      unlockMutex(&playerLock);
      drawFrog();
      sleepTicks(frog->blinkSpeed); 
   }
   pthread_exit(NULL);
}

void *initMovement(){
   fd_set set;

   while(!isGameOver()){
      FD_ZERO(&set);
      FD_SET(STDIN_FILENO, &set);
      struct timespec timeout = getTimeout(1); /* duration of one tick */
      int ret = pselect(FD_SETSIZE, &set, NULL, NULL, &timeout, NULL);
 
      if(ret == -1){
         exit(1);
      }
      else if(ret == 0){
         continue; //pselect timed out, no chars were entered
      }
      else{
         char c = getchar();
         if(c == QUIT){
	    endGame("quitters never prosper");
	 }
	 else{
	    moveFrog(c);
	 }
      }
   }
   pthread_exit(NULL);
}

void moveFrog(char c){
   if(c == LEFT_KEY && frog->currPos[1] > LEFT_EDGE){
      lockMutex(&playerLock);
      updatePrevious();
      frog->currPos[1] -= SIDE_JUMP;
      unlockMutex(&playerLock);

   }else if(c == RIGHT_KEY && frog->currPos[1] < RIGHT_EDGE-frog->width){ //screen size - width of frog
      lockMutex(&playerLock);
      updatePrevious();
      frog->currPos[1] += SIDE_JUMP;
      unlockMutex(&playerLock);

   }else if(c == UP_KEY && homeFree()){ //safe!!
      lockMutex(&playerLock);
      updatePrevious();
      frog->currPos[0] -= HOME_JUMP;
      unlockMutex(&playerLock);
      drawFrog();
      sleepTicks(10);
      moveHome();
      checkWin();

   }else if(c == DOWN_KEY && frog->currPos[0] < F_START_ROW-frog->height){
      lockMutex(&playerLock);
      updatePrevious();
      frog->currPos[0] += VERTICAL_JUMP;
      unlockMutex(&playerLock);
      
   }else if(c == UP_KEY && frog->currPos[0] > 8){
      lockMutex(&playerLock);
      updatePrevious();
      frog->currPos[0] -= VERTICAL_JUMP;
      unlockMutex(&playerLock);
   }
   
   drawFrog();
   isFrogOnAnyLog();
   checkDead();
}

void moveHome(){
   char **tile = PLAYER_GRAPHIC[frog->animateState];
   setHomePosition();
   lockMutex(&screenLock);
   sleepTicks(50);
   consoleDrawImage(frog->currPos[0], frog->currPos[1], tile, frog->height);
   unlockMutex(&screenLock);
   drawFrog();
}

bool homeFree(){
   int podLocations[] = {SAFE_POD_1, SAFE_POD_2, SAFE_POD_3, SAFE_POD_4, SAFE_POD_5};
   bool home = false;
   int i;
   int podCount = 0;
   for(i = 0; i < NUM_PODS && !home; i++){
      if(frog->currPos[0] == 5 && frog->currPos[1] > podLocations[i] && frog->currPos[1] < podLocations[i] + POD_WIDTH - frog->width && 
        !frog->podFull[podCount]){
         home = true;
	 frog->podFull[i] = true;
      }
   }
   return home;
}

void isFrogOnAnyLog(){
   Log *currLog = NULL;
   bool onLog = false;

   lockMutex(&listLock);
   lockMutex(&playerLock);
   while(!isGameOver() && currLog == NULL){
      currLog = firstItem();
   }
   while(!isGameOver() && currLog != NULL){
      if((frog->currPos[0] > currLog->startRow) && ((frog->currPos[0]+frog->height) < currLog->startRow+currLog->height) &&
       ((frog->currPos[1]+frog->width) < (currLog->currCol+currLog->width)) && (frog->currPos[1] > currLog->currCol)){
	 currLog->hasFrog = true;
	 frog->onLog = true;
	 onLog = true;
      }
      else{
         currLog->hasFrog = false;
      }
      currLog = nextAvailableLog();
   }
   unlockMutex(&playerLock);
   unlockMutex(&listLock);

   lockMutex(&playerLock);
   if(!onLog){
      frog->onLog = false;
   }
   unlockMutex(&playerLock);
}

static void drawFrog(){
   char **tile = PLAYER_GRAPHIC[frog->animateState];
   lockMutex(&screenLock);
   lockMutex(&playerLock);
   consoleClearImage(frog->prevPos[0], frog->prevPos[1], frog->height, frog->width);
   consoleDrawImage(frog->currPos[0], frog->currPos[1], tile, frog->height);
   unlockMutex(&playerLock);
   unlockMutex(&screenLock);
}

void setHomePosition(){
   frog->prevPos[0] = frog->currPos[0] = F_START_ROW;
   frog->prevPos[1] = frog->currPos[1] = F_START_COL;
}

void checkWin(){
   int winCount = 0;
   int i;
   for(i = 0; i < NUM_PODS; i++){
      if(frog->podFull[i])
         winCount++;
   }
   if(winCount == NUM_PODS){ //all spaces are filled
      endGame("you're a champ");
   }
}

void checkDead(){
   if(!inSafeZone()){
      lockMutex(&playerLock);
      frog->dead = true;
      unlockMutex(&playerLock);
   }

   if(frog->dead){
      sleepTicks(10);
      moveHome();
      lockMutex(&playerLock);
      frog->dead = false;
      unlockMutex(&playerLock);
   }
}

bool inSafeZone(){
   bool safe = false;
   if(frog->onLog || frog->currPos[0] > START_BANK || frog->currPos[0] < SAFE_BANK){
      safe = true;
   }
   return safe;
}

void createFrog(){
   char **tile = PLAYER_GRAPHIC[0];
   setHomePosition();

   lockMutex(&playerLock);
   frog->blinkSpeed = 30;
   frog->animateState = first;
   frog->height = PLAYER_HEIGHT;
   frog->width = strlen(tile[0]);
   frog->dead = false;
   frog->onLog = false;
   int i;
   for(i = 0; i < NUM_PODS; i++){
      frog->podFull[i] = false;
   }
   unlockMutex(&playerLock);
}

Frog *getFrog(){
   return frog;
}

static void updatePrevious(){
   frog->prevPos[0] = frog->currPos[0];
   frog->prevPos[1] = frog->currPos[1];
}
