#include "headers.h"

struct users me,user;
bool running;
int semd, shmd, shmd2, fd; // desecriptors
union semun us;
struct sembuf sb;

int main() {
  running = true;
  printf("\x1b[H\x1b[J"); //Clears screen
  char line[50];

  printf("Please type in your choice from the options listed below:\n\n- Log in\n- Create new account\n- Exit\n\n");

  while(running) {
    printf("Type choice here: ");
    fgets(line, 50, stdin);
    char * checker;
    if ((checker = strchr(line, '\n')) != NULL) {
      *checker = '\0';
    }
    display(line);
  }
  return 0;
}
