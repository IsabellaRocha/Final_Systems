#include "headers.h"

struct vehicle* availableCars;
struct vehicle* rentedCars;

int main() {
  char line[50];
  printf("Please choose whether you want to log in or create a new account:");
  while(strcmp(line, "exit") != 0) {
    fgets(line, 50, stdin);
    display(line);
  }
  return 0;
}

void display(char * choice) {
  char * newChoice = tolower(choice);
  if(strcmp(newChoice, "log in") == 0) {
    printf("Username:");
  }
  if(strcmp(newChoice, "create new account") == 0) {
    printf("Username:");
  }
  if(strcmp(newChoice, "view available cars") == 0) {
    int idx = 0;
    for(idx; availableCars[idx] != NULL; idx++) {
      printf("%s\n", availableCars[idx].model);
    }
  }
  if(strcmp(newChoice, "view rented cars") == 0) {
    int idx = 0;
    rent();
    for(idx; rentedCars[idx] != NULL; idx++) {
      printf("%s\n", rentCars[idx].model);
    }
  }
  if(strcmp(newChoice, "view my account") == 0) {
  }
  if(strcmp(newChoice, "log out") == 0) {
    logout();
    printf("Please choose whether you want to log in or create a new account:");
  }
}

void rent();

void logout();
