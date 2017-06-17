/* The header file for frogger.c
*/

#ifndef FROGGER_H
#define FROGGER_H

#define MAX_LIVES 4

/* Initializes player and logs, as well as the refresh screen and update lives
   methods. Sets a condition variable to wait for gameover */
void startGame();

/* Calls the console refresh method */
void *refreshScreen();

/* Checks if the frog is dead and updates the player lives */
void *updateLives();

/* Displays an end of game message and signals the condition variable, and sets
   game over to true. */
void endGame(char *endMessage);

#endif
