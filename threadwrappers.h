/* The header file for threadwrappers.c
*/
#ifndef THREADWRAPPERS_H
#define THREADWRAPPERS_H

/*Creates a pthread_t and increments thread count */
void createThread(pthread_t *thread, void *(*func)(void *), void *param);

/* Safely join a pthread_t and decrements thread count*/
void joinThread(pthread_t thread);

/* Safely creates a log pthread_t */
void createLogThread(pthread_t *thread, void *(*func)(void *), void *param);

/* Safely joins a log pthread_t */
void joinLogThread(pthread_t thread);

/* Locks a mutex variable */
void lockMutex(pthread_mutex_t *lock);

/* Unlocks a mutex variable */
void unlockMutex(pthread_mutex_t *lock);

/* Prints error and exits */
void printError();

#endif
