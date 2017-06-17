/* The header file for log.c
*/

#ifndef LOG_H
#define LOG_H
#include <stdbool.h>
#include <pthread.h>
#include "gameglobals.h"

enum logDirection {left, right};
enum row {
   one = 4,
   two = 8,
   three = 12,
   four = 16,
};

typedef struct LOG Log;
struct LOG {
   pthread_t threadID;
   int speed;
   int prevCol, currCol;
   bool dead;
   int width;
   bool hasFrog;
   int height;
   enum row startRow;
   enum logDirection direction;
   enum state animateState;
};

/* Creates log spawner for each row */
void initializeLogs();

/* Manages the logs for each row. Adds logs to the list and initializes logs with 
   start values */ 
void *runLogs(void *startRow);

/* Decides whether to move the log by itself or the frog with the log, also sets
   log speed */
void *logController(void *currLog);

/* Moves log in the correct direction and checks if it's offscreen */
void moveLog(Log *log);

/* Sets animate state for the log */
void animateLog(Log *log);

/* Sets up log with default attributes */ 
void logStartup(Log *log, int *startRow);

/* Sets direction of log movement depending on row number */
void setDirection(Log *log);

/* Sets log speed based on current row */
void setLogSpeed(Log *log, int *startRow);

/* Checks if log is offscreen */
void checkIsDead(Log *log);

/* Traverses linked list to check for dead logs and cleans up associated memory.
   Also joins threads before deleting log */
void *cleanUpLogs();

/* Calls moveLog and moveFrog methods together */
void moveFrogAndLog(Log *log);

#endif
