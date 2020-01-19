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
#define KEY2 24603
#define SEG_SIZE 1024
#define struct vehicle car1 = {"Toyota", "Blue", 5, 200};
#define struct vehicle car2 = {"Jeep", "Green", 5, 700};
#define struct vehicle car3 = {"Buick", "Gray", 5, 300};
#define struct vehicle car4 = {"Ford", "White", 5, 600};
#define struct vehicle car5 = {"BMW", "Blue", 5, 1000};
#define struct vehicle car6 = {"Volkswagen", "Red", 2, 500};
#define struct vehicle car7 = {"Subaru", "Gray", 8, 1200};
#define struct vehicle car8 = {"Tesla", "Black", 5, 1000};
#define struct vehicle car9 = {"Toyota", "Blue", 5, 500};
#define struct vehicle car10 = {"Lexus", "Blue", 8, 1500};
#define struct vehicle cars[10] = {car1, car2, car3, car4, car5, car6, car7, car8, car9, car10};

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
  int balance;
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
