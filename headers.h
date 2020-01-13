#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <conio.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>

#define KEY 24602
#define SEG_SIZE 1024

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};

struct vehicle {
  char[50] model;
  char[20] color;
  int seatNumber;
  int cost;
}

struct day {
  int car1;
  int car2;
  int car3;
}

struct month {
  int numDays;
  struct day days[numDays];
  char[20] name;
}

int makeUser();
void display(char * choice);
void rent();
void logout();
int verifyUser();
