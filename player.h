/* The header file for player.c
*/

#ifndef PLAYER_H
#define PLAYER_H
#include <stdbool.h>
#include "log.h"
#include "gameglobals.h"

typedef struct FROG Frog;

struct FROG {
int prevPos[2];
int currPos[2];
enum state animateState;
int blinkSpeed;
int height;
int width;
bool onLog;
bool dead;
bool podFull[5];
};

/* Creates threads required for movement and animation of the player */ 
void initializePlayer();

/* Keeps track of current animation state, draws the frog and set blink speed */
void *animateFrog();

/* Uses pselect to either timeout if no chars are entered or gets the character */ 
void *initMovement();

/* Depending on which character was entered, move the frog in 1 of 4 directions.
   If the frog is in the last row and jumps to a safe spot, save the graphic and 
   move the frog back to the beginning. Also checks for frog death and if the frog
   is on any logs. */
void moveFrog(char direction);

/* Sets up the frog's attributes */ 
void createFrog();

/* Goes through the linked list to see if the frog is on any one of the logs on
   screen. This helps the moveFrogWithLog method in the log.c */
void isFrogOnAnyLog();

/* Compares frog position with the safe pod positions to see if the frog has jumped
   to safety. If so, it sets the spot to true in the frog's podFull array.*/
bool homeFree();

/* Draws the frog back at the start bank */
void moveHome();

/* Sets the frog's home coordinates */
void setHomePosition();

/*Checks to make sure frog is on a log or on the home bank */ 
bool inSafeZone();

/* Checks to see if the frog is in a safe zone, and moves to the home
   if frog is dead */ 
void checkDead();

/* Checks to see if the frog has made it to all the safe pods */
void checkWin();

/* Returns the global frog */
Frog *getFrog();

#endif
