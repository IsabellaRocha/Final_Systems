#include "headers.h"

struct vehicle* availableCars;
struct vehicle* rentedCars;
char ** me;

int main() {
  printf("\x1b[H\x1b[J"); //Clears screen
  char line[50];
  while(strcmp(line, "exit") != 0) {
    printf("Please choose whether you want to log in or create a new account: ");
    fgets(line, 50, stdin);
    printf("\x1b[H\x1b[J");
    char * checker;
    if ((checker = strchr(line, '\n')) != NULL) {
      *checker = '\0';
    }
    int run = display(line);
    if (run == 0) {
      printf("Not a valid option\n");
    }
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

char * removeSpace(char * line) {
    char arg[50];
    char newLine[50];
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

int display(char * choice) {
  if(strcmp(choice, "log in") == 0) {
  //  printf("\x1b[H\x1b[J");
    printf("Username: ");
    if(verifyUser()) {
      printf("\x1b[H\x1b[J");
      printf("Please type in your choice from the options listed below: \n\n- View available cars (Select this if you also wish to rent a car)\n- View rented cars\n- View my account\n- Log out\n\n");
    }
    if(!verifyUser()) {
      char line[50];
      printf("Please choose whether you want to log in or create a new account: ");
      fgets(line, 50, stdin);
      printf("\x1b[H\x1b[J");
      char * checker;
      if ((checker = strchr(line, '\n')) != NULL) {
        *checker = '\0';
      }
      display(line);
    }
  }
  if(strcmp(choice, "create new account") == 0) {
    printf("\x1b[H\x1b[J");
    printf("Username: ");
    if(makeUser()) {
      printf("\x1b[H\x1b[J");
      printf("Please type in your choice from the options listed below: \n\n- View available cars (Select this if you also wish to rent a car)\n- View rented cars\n- View my account\n- Log out\n\n");
    }
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
  if(strcmp(choice, "view my account") == 0) {
    printf("\x1b[H\x1b[J");
  }
  if(strcmp(choice, "log out") == 0) {
    printf("\x1b[H\x1b[J");
    logout();
    printf("Please choose whether you want to log in or create a new account:");
  }*/
  return 0;
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
  printf("\x1b[H\x1b[J");
  char * checker;
  if ((checker = strchr(input, '\n')) != NULL) {
    *checker = '\0';
  }
  strncat(input, ",", 1);
  printf("\x1b[H\x1b[J");
  write(fd, input, strlen(input));
  printf("Password: ");
  char input2[SEG_SIZE];
  fgets(input2, SEG_SIZE, stdin);
  write(fd, input2, strlen(input2));
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
  userID = parse_args(check, ",");
  if (strlen(check) != 0) {
    *(strrchr(check, '\n') + 1) = '\0';
  }
  char input[SEG_SIZE];
  fgets(input, SEG_SIZE, stdin);
  char * checker;
  if ((checker = strchr(input, '\n')) != NULL) {
    *checker = '\0';
  }
  char * username = &check[0];
  if(strcmp(input, username) == 0) {
    printf("\x1b[H\x1b[J");
    printf("Password: ");
    char input2[SEG_SIZE];
    fgets(input2, SEG_SIZE, stdin);
    char * checker;
    if ((checker = strchr(input2, '\n')) != NULL) {
      *checker = '\0';
    }
    char * password = &check[1];
    if(strcmp(input2, password) == 0) {
      return 1;
    }
  }
  printf("\x1b[H\x1b[J");
  return 0;
}
