#include "headers.h"

struct users me,user;
bool running;
int semd, shmd, fd; // desecriptors
struct sembuf sb1,sb2;

int main() {
  running = true;
  char line[50];
  printf("\x1b[H\x1b[J"); //Clears screen
  printf("Please type in your choice from the options listed below:\n\n- Log in\n- Create new account\n- Exit\n\n");
  while(running) {
    printf("Type choice here: ");
    fgets(line, 50, stdin);
    printf("SDFKJNSD%s\n",line);
    if (strlen(line) != 0) {
      line[strlen(line)-1] = '\0';
    }
    display(line);
  }
  return 0;
}
