#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>

#include <string.h>
#include <math.h>
#include <time.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
// #include <sys/ipc.h>


#include "patients.h"
#include "menu.h"
#define MSG_LENGTH 32
#define VACC_CAP 10
#define RATIO 10

void simulate(Patients* p);