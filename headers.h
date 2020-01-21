#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SEMKEY 24602
#define MEMKEY 50298
#define SEM2KEY 17602
#define MEM2KEY 54268
#define SEG_SIZE 1024

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};

struct calendar {
  int unit1[365];
  int unit2[365];
  int unit3[365];
};

struct vehicle {
  char model[50];
  char color[20];
  int seatNumber;
  int cost;
  struct calendar calendar;
};

struct users {
  int userid;
  char username[50];
  struct vehicle rented;
  int balance;
  int start_rent_day;
  int end_rent_day;
  int start_rent_month;
  int end_rent_month;
};

extern struct users me,user;
extern bool running;
extern int semd, shmd, shmd2, fd; // desecriptors
extern struct sembuf sb1,sb2;
/*struct month {
  int numDays;
  struct day days[numDays];
  char name[20];
}; */

int makeUser();
int display(char * choice);
void displayMenu();
int return_car();
int rent();
void logout();
int verifyUser();
void viewAccount();
char ** parse_args(char * line, char * delimiter);
char * removeSpace(char * line);
