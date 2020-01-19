#include "headers.h"

struct users me;
bool running = true;

int main() {
  printf("\x1b[H\x1b[J"); //Clears screen
  char line[50];

  memset(me.username, '\0', 20); //Sets all values to null
  memset(me.password, '\0', 20);
  memset(me.rented, '\0', 50);
  me.balance = 0;

  printf("Please type in your choice from the options listed below:\n\n- Log in\n- Create new account\n- Exit\n\n");

  while(running) {
    printf("Type choice here: ");
    fgets(line, 50, stdin);
    printf("\x1b[H\x1b[J");
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
    char ** args = malloc(1000 * sizeof(char *));
  //  char * args[1000];
    int c = 0;
    char * token;
    while (parse != NULL){
        token = strsep(&parse, delimiter);
        args[c] = token;
        c++;
    }
    args[c] = NULL;
    free(parse);
    return args;
}

char * removeSpace(char * line) {
    char arg[1000];
    char newLine[1000];
    strcpy(arg, line);
    int idx = 0;
    int cur = 0;
    bool space = false;
    while(idx < strlen(line) && arg[idx] == ' ') { //Get rid of leading spaces
        idx++;
    }
    while(idx < strlen(line)) {
        if(arg[idx] != ' ') {
            space = false;
            newLine[cur] = arg[idx];
            cur++;
            idx++;
        }
        if(arg[idx] == ' ' && space){
            idx++;
        }
        if(arg[idx] == ' ' && !space) {
            space = true;
            newLine[cur] = arg[idx];
            idx++;
            cur++;
        }
    }
    if(cur > 0 && newLine[cur - 1] == ' ') { //In case there's a space after the last charcter
    newLine[cur - 1] = '\0';
}
newLine[cur] = '\0'; //Null to terminate execvp
char * parse = malloc(50 * sizeof(char));
strcpy(parse, newLine);
return parse;
}

void displayMenu() {
  printf("\x1b[H\x1b[J");
  printf("Please type in your choice from the options listed below: \n\n- View available cars (Select this if you also wish to rent a car)\n- View rented cars\n- View my account\n- Log out\n\n");
}

void display(char * choice) {
  choice = removeSpace(choice);

  if(strcmp(choice, "exit") == 0) {
    running = false;
  }

  if(strcmp(choice, "log in") == 0) {
    printf("\x1b[H\x1b[J");
    printf("Username: ");
    if(verifyUser()) {
      displayMenu();
    }
  }

  if(strcmp(choice, "create new account") == 0) {
    printf("\x1b[H\x1b[J");
    printf("Username: ");
    if(makeUser()) {
      displayMenu();
    }
    else {
      printf("Please type in your choice from the options listed below:\n\n- Log in\n- Create new account\n- Exit\n\n");
    }
  }

  if(strcmp(choice, "view my account") == 0) {
    printf("\x1b[H\x1b[J");
    viewAccount();
  }

  if(strcmp(choice, "back") == 0) {
    displayMenu();
  }

  if(strcmp(choice, "log out") == 0) {
    printf("\x1b[H\x1b[J");
    logout();
  }/*
  if(strcmp(choice, "view available cars") == 0) {
    printf("\x1b[H\x1b[J");
    int idx = 0;
    for(idx; availableCars[idx] != NULL; idx++) {
      printf("%s\n", availableCars[idx].model);
    }
  }
  if(strcmp(choice, "view rented cars") == 0) {
    printf("\x1b[H\x1b[J");
    int idx = 0;
    rent();
    for(idx; rentedCars[idx] != NULL; idx++) {
      printf("%s\n", rentCars[idx].model);
    }
  }
  }*/
}


void rent();

void logout() {
  memset(me.username, '\0', 20);
  memset(me.password, '\0', 20);
  memset(me.rented, '\0', 50);
  printf("Please type in your choice from the options listed below:\n\n- Log in\n- Create new account\n- Exit\n\n");
}

int makeUser() {
  bool exists = true;
  if(fopen("users.txt", "r") == NULL) {
    exists = false;
    int fd = open("users.txt", O_CREAT|O_TRUNC, 0744);
    if (fd < 0) {
      printf("Error: %s", strerror(errno));
      return 1;
    }
    close(fd);
  }

  int fd = open("users.txt", O_RDWR|O_APPEND);
  char input[SEG_SIZE];
  fgets(input, SEG_SIZE, stdin);
  printf("\x1b[H\x1b[J");
  char * checker;
  if ((checker = strchr(input, '\n')) != NULL) {
    *checker = '\0';
  }
  if(exists) {
    char ** userID;
    char check[SEG_SIZE];
    check[0] = '\0';
    read(fd, check, SEG_SIZE);
    if (strlen(check) != 0) {
      *(strrchr(check, '\n') + 1) = '\0';
    }
    userID = parse_args(check, "\n");

    int idx = 0;
    while(userID[idx] != NULL) {
      char **args = parse_args(userID[idx], ",");
      if(strcmp(input, args[0]) == 0) {
        printf("\x1b[H\x1b[J");
        printf("Username has already been taken, please try again\n\n");
        return 0;
      }
      idx++;
    }
  }

  strcpy(me.username, input);
  strncat(input, ",", 1);
  write(fd, input, strlen(input));

  printf("\x1b[H\x1b[J");
  printf("Password: ");
  char input2[SEG_SIZE];
  fgets(input2, SEG_SIZE, stdin);
  write(fd, input2, strlen(input2));
  strcpy(me.password, input2);
  me.balance = 5000;

  close(fd);
  return 1;
}

int verifyUser() {
  char ** userID;
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
  userID = parse_args(check, "\n");
  char input[SEG_SIZE];
  fgets(input, SEG_SIZE, stdin);
  char * checker;
  if ((checker = strchr(input, '\n')) != NULL) {
    *checker = '\0';
  }

  int idx = 0;
  while(userID[idx] != NULL) {
    char **args = parse_args(userID[idx], ",");
    if(strcmp(input, args[0]) == 0) {
      printf("\x1b[H\x1b[J");
      printf("Password: ");
      char input2[SEG_SIZE];
      fgets(input2, SEG_SIZE, stdin);
      char * checker;
      if ((checker = strchr(input2, '\n')) != NULL) {
        *checker = '\0';
      }
      if(strcmp(input2, args[1]) == 0) {
        strcpy(me.username, input);
        strcpy(me.password, input2);
        return 1;
      }
    }
    idx++;
  }
  printf("\x1b[H\x1b[J");
  printf("Invalid account\n\n");
  printf("Please type in your choice from the options listed below:\n\n- Log in\n- Create new account\n- Exit\n\n");
  return 0;
}

void viewAccount() {
  printf("Username: %s\n", me.username);
  printf("Password: %s\n", me.password);
  printf("Current car: %s\n", me.rented);
  printf("Current balance: $%d\n", me.balance);
  printf("Type 'back' to go back to the menu\n\n");
}
