#include "headers.h"

struct vehicle* availableCars;
struct vehicle* rentedCars;

int main() {
  system("clear");
  char line[50];
  printf("Please choose whether you want to log in or create a new account: ");
  while(strcmp(line, "exit") != 0) {
    fgets(line, 50, stdin);
    char * checker;
    if ((checker = strchr(line, '\n')) != NULL) {
      *checker = '\0';
    }
    display(line);
  }
  return 0;
}

char ** parse_args( char * line, char * delimiter ){ //For reading through the txt file of users
    char * parse = removeSpace(line);
    char ** args = malloc(50 * sizeof(char *));
    int c = 0;
    char * token;
    while (parse != NULL){
        token = strsep(&parse, delimiter);
        args[c] = token;
        c++;
    }
    for (;c <= 10 - 1; c++){
        args[c] = NULL;
    }
    free(parse);
    return args;
}

void display(char * choice) {
  printf("%s", choice);
  if(strcmp(choice, "log in") == 0) {
    system("clear"); //Clears output screen, from conio.h
    printf("Username: ");
  }
  if(verifyUser()) {
    system("clear");
    printf("Please type in your choice from the options listed below: \n\n- View available cars (Select this if you also wish to rent a car)\n- View rented cars\n- View my account\n- Log out\n\n");
  }
  if(strcmp(choice, "create new account") == 0) {
    system("clear");
    printf("Username: ");
    if(makeUser()) {
      system("clear");
      printf("Password: ");
      if(makePassword()) {
        system("clear");
        printf("Please type in your choice from the options listed below: \n\n- View available cars (Select this if you also wish to rent a car)\n- View rented cars\n- View my account\n- Log out\n\n");
      }
    }
  }/*
  if(strcmp(choice, "view available cars") == 0) {
    system("clear");
    int idx = 0;
    for(idx; availableCars[idx] != NULL; idx++) {
      printf("%s\n", availableCars[idx].model);
    }
  }
  if(strcmp(choice, "view rented cars") == 0) {
    system("clear");
    int idx = 0;
    rent();
    for(idx; rentedCars[idx] != NULL; idx++) {
      printf("%s\n", rentCars[idx].model);
    }
  }
  if(strcmp(choice, "view my account") == 0) {
    system("clear");
  }
  if(strcmp(choice, "log out") == 0) {
    system("clear");
    logout();
    printf("Please choose whether you want to log in or create a new account:");
  }*/
}

void rent();

void logout();

int makeUser() {
  if(fopen("users.txt", "r") == NULL) {
    int fd = open("users.txt", O_CREAT|O_TRUNC, 0744);
    if (fd < 0) {
      printf("Error: %s", strerror(errno));
      return 1;
    }
    close(fd);
  }
  int fd = open("users.txt", O_WRONLY|O_APPEND);
  char input[SEG_SIZE];
  fgets(input, SEG_SIZE, stdin);
  char * checker;
  if ((checker = strchr(input, '\n')) != NULL) {
    *checker = '\0';
  }
  strncat(input, ",", 1);
  write(fd, input, strlen(input));
  close(fd);
  return 1;
}

int makePassword() {
  int fd = open("users.txt", O_WRONLY|O_APPEND);
  char input[SEG_SIZE];
  fgets(input, SEG_SIZE, stdin);
  write(fd, input, strlen(input));
  close(fd);
  return 1;
}

int verifyUser() {
  int fd = open("users.txt", O_RDONLY);
  if(fd < 0) {
    printf("Error: %s", strerror(errno));
    return 1;
  }
  char check[SEG_SIZE];
  check[0] = '\0';
  read(fd, check, SEG_SIZE);
  if (strlen(check) != 0) {
    *(strrchr(check, '\n') + 1) = '\0';
  }
  char input[SEG_SIZE];
  fgets(input, SEG_SIZE, stdin);
  if(strstr(input, check) != NULL) {
    system("clear");
    printf("Password: ");
    char input2[SEG_SIZE];
    fgets(input2, SEG_SIZE, stdin);
    if(strstr(input2, check) != NULL) {
      return 1;
    }
  }
  return 0;
}
