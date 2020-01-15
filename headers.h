#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>

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
  char model[50];
  char color[20];
  int seatNumber;
  int cost;
};

struct day {
  int car1;
  int car2;
  int car3;
};

struct users {
  char username[20];
  char password[20];
  char rented[50];
};

/*struct month {
  int numDays;
  struct day days[numDays];
  char name[20];
}; */

int makeUser();
int makePassword();
void display(char * choice);
void displayMenu();
void rent();
void logout();
int verifyUser();
void viewAccount();
char ** parse_args(char * line, char * delimiter);
char * removeSpace(char * line);
/*int setUpCars();
int viewCars()
int removeCars();
int execute();*/
