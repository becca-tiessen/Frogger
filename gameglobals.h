/* The header file for gameglobals.c
*/

#ifndef GAMEGLOBALS_H
#define GAMEGLOBALS_H

#include <stdbool.h> 

//---GLOBALS-------------------------------------//
#define GAME_ROWS 24
#define GAME_COLS 80
#define NUM_THREADS 64
#define LEFT_EDGE 0
#define RIGHT_EDGE 80
#define NUM_PODS 5
#define POD_WIDTH 7
#define START_BANK 20
#define SAFE_BANK 4
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define UP_KEY 'w'
#define DOWN_KEY 's'
#define QUIT 'q'

enum state {first, second};

pthread_cond_t condLock;
pthread_mutex_t refreshLock, screenLock, playerLock, threadCountLock, listLock, mainLock;
pthread_t tids[NUM_THREADS];

/* Draws the initial game screen */
bool drawScreen();

/* Initializes all mutexes and cond variable */
void initLocks();

/* Destroys the mutexes and cond variable */
void destroyLocks();

/* Sets game over to true */
void setGameOver();

/* Checks if game over is true */
bool isGameOver();

/* Returns the current thread count */
int getThreadCount();

/* Increments the number of threads */
void incrementThreadCount();

/*Decrements the number of threads */
void decrementThreadCount();

/* Sleeps */
void sleepLoop(int ticks);

#endif
