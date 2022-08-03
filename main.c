#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include "variaveis.h"
#define PLACA_04

// A normal C function that is executed as a thread
// when its name is specified in pthread_create()

int main()
{
    semaphore_1_3();
    return 0;
}