#include "headers.h"
struct users me;
bool running = true;
int semd, shmd, shmd2, fd; // desecriptors
union semun us;
struct sembuf sb;

int main() {
  printf("\x1b[H\x1b[J"); //Clears screen
  char line[50];

  memset(me.username, '\0', sizeof(me.username)); //Sets all values to null
  me.userid = 0;
  me.rented = (struct vehicle){" ", " ", 0, 0, {0,0,0}};
  me.balance = 0;

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


void displayMenu() {
  printf("\x1b[H\x1b[J");
  printf("Please type in your choice from the options listed below: \n\n- View available cars (Select this if you also wish to rent a car)\n- View rented cars\n- View my account\n- Return a car\n- Log out\n\n");
}

int display(char * choice) {
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

  if(strcmp(choice, "create new account") == 0 || strcmp(choice, "Create new account") == 0  ) {
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
  }
  if(strcmp(choice, "view available cars") == 0) {
    int status;
    if(fork() == 0) {
      char* args[] = {"./control", "-va", NULL};
      int execute = execvp("./control", args);
      if(execute < 0) {
        printf("Error: %s", strerror(errno));
        return 1;
      }
    }
    else {
      wait(&status);
    }
  }
  if(strcmp(choice, "view rented cars") == 0) {
    int status;
    if(fork() == 0) {
      char* args[] = {"./control", "-vr", NULL};
      int execute = execvp("./control", args);
      if(execute < 0) {
        printf("Error: %s", strerror(errno));
        return 1;
      }
    }
    else {
      wait(&status);
    }
  }
  if(strcmp(choice, "rent") == 0) {
    int status;
    if(fork() == 0) {
      char* args[] = {"./rent", NULL};
      int execute = execvp("./rent", args);
      if(execute < 0) {
        printf("Error: %s", strerror(errno));
        return 1;
      }
    }
    else {
      wait(&status);
    }
  }
  if(strcmp(choice, "return a car") == 0) {
    int status;
    if(fork() == 0) {
      char* args[] = {"./ret", NULL};
      int execute = execvp("./ret", args);
      if(execute < 0) {
        printf("Error: %s", strerror(errno));
        return 1;
      }
    }
    else {
      wait(&status);
    }
  }
  return 0;
}

void logout() {
  memset(me.username, '\0', 20);
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
  //read first line of users.txt that will store the number of userss
  int num_users = 0;

  int fd = open("users.txt", O_RDWR|O_APPEND);
  char input[50];
  fgets(input, 50, stdin);
  printf("\x1b[H\x1b[J");
  char * checker;
  if ((checker = strchr(input, '\n')) != NULL) {
    *checker = '\0';
  }
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

  //after username and passwords are confirmed, store username's other info in shared memory
  semd = semget(SEM2KEY, 1, 0);
  if (semd < 0) {
      printf("semaphore error: %s", strerror(errno));
      return 1;
  }
  semop(semd, &sb, 1);
  shmd = shmget(MEM2KEY, sizeof(struct users) * 100, 0);
  if (shmd < 0) {
      printf("memory error: %s", strerror(errno));
      return 1;
  }

  struct users * users = (struct users*) shmat(shmd, 0, 0);
  struct vehicle car= {" ", " ", 0, 0, {0,0,0}};
  struct users user= {.userid = 0,.rented = car,.balance = 5000};
  strcpy(user.username,input);
  users[num_users] = user;

  strcpy(me.username, input);
  strncat(input, ",", 1);
  write(fd, input, strlen(input));

  printf("\x1b[H\x1b[J");
  printf("Password: ");
  char input2[SEG_SIZE];
  fgets(input2, SEG_SIZE, stdin);
  write(fd, input2, strlen(input2));
  me.balance = 5000;

  //close semaphores and shared memory

  shmdt(users);

  sb.sem_op = 1;
  semop(semd, &sb, 1);

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
        //me.userid =
        //me.rented =
        //me.balance =
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
  printf("Current car: %s\n", me.rented.model);
  printf("Current balance: $%d\n", me.balance);
  printf("Type 'back' to go back to the menu\n\n");
}
