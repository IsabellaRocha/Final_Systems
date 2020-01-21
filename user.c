#include "headers.h"

void displayMenu() {
  printf("\x1b[H\x1b[J");
  printf("Please type in your choice from the options listed below: \n\n- View available cars (Select this if you also wish to rent a car)\n- View my account\n- Return a car\n- Log out\n\n");
}

int display(char * choice) {
  choice = removeSpace(choice);

  if(strcmp(choice, "exit") == 0) {
    running = false;
  }
  else if(strcmp(choice, "log in") == 0) {
    printf("\x1b[H\x1b[J");
    if(verifyUser()) {
      displayMenu();
    } else{
      printf("Please type in your choice from the options listed below:\n\n- Log in\n- Create new account\n- Exit\n\n");
    }
  }

  else if(strcmp(choice, "create new account") == 0 || strcmp(choice, "Create new account") == 0  ) {
    printf("\x1b[H\x1b[J");
    if(makeUser()) {
      displayMenu();
    }
    else {
      printf("Please type in your choice from the options listed below:\n\n- Log in\n- Create new account\n- Exit\n\n");
    }
  }

  else if(strcmp(choice, "view my account") == 0) {
    printf("\x1b[H\x1b[J");
    viewAccount();
  }

  else if(strcmp(choice, "back") == 0) {
    displayMenu();
  }

  else if(strcmp(choice, "log out") == 0) {
    printf("\x1b[H\x1b[J");
    logout();
  }
  else if(strcmp(choice, "view available cars") == 0) {
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
  else if(strcmp(choice, "rent") == 0) {
    rent();
    // int status;
    // if(fork() == 0) {
    //   char* args[] = {"./rent", NULL};
    //   int execute = execvp("./rent", args);
    //   if(execute < 0) {
    //     printf("Error: %s", strerror(errno));
    //     return 1;
    //   }
    // }
    // else {
    //   wait(&status);
    // }
  }
  else if(strcmp(choice, "return a car") == 0) {
    return_car();
    // int status;
    // if(fork() == 0) {
    //   char* args[] = {"./ret", NULL};
    //   int execute = execvp("./ret", args);
    //   if(execute < 0) {
    //     printf("Error: %s", strerror(errno));
    //     return 1;
    //   }
    // }
    // else {
    //   wait(&status);
    // }
  }
  return 0;
}

int makeUser() {
  char input[50];
  char buffer[SEG_SIZE];
  memset (input, 0, 50 );
  memset (buffer, 0, SEG_SIZE );
  bool exists = true;
  char* str_num_user;

  if(fopen("users.txt", "r") == NULL) {
    //printf("creating users.txt");
    exists = false;
    int fd = open("users.txt", O_CREAT|O_TRUNC|O_WRONLY, 0744);
    if (fd < 0) {
      printf("Error: %s", strerror(errno));
      return 1;
    }
    write(fd, "0", 1);
    close(fd);
  }
  //read first line of users.txt that will store the number of userss
  int fd = open("users.txt", O_RDWR);
  read(fd, str_num_user, 1);
  int num_users = atoi(str_num_user);
  printf("Username: ");
  //printf("%d\n",num_users);
  fgets(input, 50, stdin);
  printf("\x1b[H\x1b[J");
  if (strlen(input) != 0) {
    input[strlen(input)-1] = '\0';
  }
  if(exists) {
    char ** userID;
    char ** args;
    read(fd, buffer, 1);
    read(fd, buffer, SEG_SIZE);
    // printf("buffer %s\n",buffer);
    userID = parse_args(buffer, "\n");
    for (size_t i = 0; i < num_users; i++) {
        args = parse_args(userID[i], ",");
        if(strcmp(input, args[0]) == 0) {
          printf("\x1b[H\x1b[J");
          printf("Username has already been taken, please try again\n\n");
          return 0;
        }
    }
  }
  num_users += 1;

  sb2.sem_num=0;
  sb2.sem_op = -1;
  sb2.sem_flg = SEM_UNDO;
  // after username and passwords are confirmed, store username's other info in shared memory
  semd = semget(SEM2KEY, 1, 0);
  if (semd < 0) {
      printf("semaphore error: %s", strerror(errno));
      return 1;
  }
  semop(semd, &sb2, 1);
  shmd = shmget(MEM2KEY, sizeof(struct users) * 100, 0);
  if (shmd < 0) {
      printf("memory error: %s", strerror(errno));
      return 1;
  }

  struct users * users = (struct users*) shmat(shmd, 0, 0);
  user.userid = num_users;
  user.rented = (struct vehicle){" ", " ", 0, 0, {0,0,0}};
  user.balance = 5000;
  strcpy(user.username,input);
  users[num_users-1] = user;

  me.userid = num_users;
  me.rented = (struct vehicle){" ", " ", 0, 0, {0,0,0}};
  me.balance = 5000;
  strcpy(me.username,input);

  //printf("%s\n",buffer);
  // if (strlen(buffer) != 0) {
  //   buffer[strlen(buffer)-1] = '\0';
  // }
  //printf("%s\n",buffer);
  char update[SEG_SIZE];
  memset (update, 0, SEG_SIZE );
  sprintf(update, "%d\n", num_users);
  strcat(update, buffer);
  strcat(update, input);
  strcat(update, ",");

  printf("\x1b[H\x1b[J");
  printf("Password: ");
  memset (input, 0, 50 );
  fgets(input, 50, stdin);

  strcat(update,input);

  close(fd);
  fd = open("users.txt", O_RDWR | O_TRUNC);
  write(fd, update, strlen(update));
  me.balance = 5000;

  //close semaphores and shared memory

  shmdt(users);

  sb2.sem_op = 1;
  semop(semd, &sb2, 1);

  close(fd);
  return 1;
}

int verifyUser() {
  char ** userID;
  int fd = open("users.txt", O_RDONLY);
  if(fd < 0) {
    printf("Error: %s\n", strerror(errno));
    return 0;
  }

  char check[SEG_SIZE];
  check[0] = '\0';
  read(fd, check, 2);
  read(fd, check, SEG_SIZE);
  if (strlen(check) != 0) {
    check[strlen(check)-1] = '\0';
  }
  userID = parse_args(check, "\n");
  char input[SEG_SIZE];
  printf("Username: ");
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
        sb2.sem_num=0;
        sb2.sem_op =-1;
        sb2.sem_flg = SEM_UNDO;
        // after username and passwords are confirmed, store username's other info in shared memory
        semd = semget(SEM2KEY, 1, 0);
        if (semd < 0) {
            printf("semaphore error: %s", strerror(errno));
            return 1;
        }
        semop(semd, &sb2, 1);
        shmd = shmget(MEM2KEY, sizeof(struct users) * 100, 0);
        if (shmd < 0) {
            printf("memory error: %s", strerror(errno));
            return 1;
        }
        struct users * users = (struct users*) shmat(shmd, 0, 0);
        struct users * user = &users[idx];
        strcpy(me.username, user->username);
        me.userid = user->userid;
        me.rented = user->rented;
        me.balance = user->balance;

        shmdt(users);

        sb2.sem_op = 1;
        semop(semd, &sb2, 1);
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

void logout() {
  sb2.sem_num=0;
  sb2.sem_op = -1;
  sb2.sem_flg = SEM_UNDO;
  // after username and passwords are confirmed, store username's other info in shared memory
  semd = semget(SEM2KEY, 1, 0);
  if (semd < 0) {
      printf("semaphore error: %s", strerror(errno));
      return;
  }
  semop(semd, &sb2, 1);
  shmd = shmget(MEM2KEY, sizeof(struct users) * 100, 0);
  if (shmd < 0) {
      printf("memory error: %s", strerror(errno));
      return;
  }
  struct users * users = (struct users*) shmat(shmd, 0, 0);
  struct users * user = &users[me.userid];
  memcpy(&(user->rented), &me.rented, sizeof(struct vehicle));
  memcpy(&(user->balance), &me.balance, sizeof(me.balance));
  shmdt(users);

  sb2.sem_op = 1;
  semop(semd, &sb2, 1);
  memset(me.username, '\0', 20);
  printf("Please type in your choice from the options listed below:\n\n- Log in\n- Create new account\n- Exit\n\n");
}

void viewAccount() {
  printf("Username: %s\n", me.username);
  printf("Current car: %s\n", me.rented.model);
  printf("Current balance: $%d\n", me.balance);
  printf("Type 'back' to go back to the menu\n\n");
}
