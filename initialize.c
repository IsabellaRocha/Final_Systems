#include "headers.h"

int semd, shmd, shmd2, fd; // desecriptors
union semun us;
struct sembuf sb;

int setUpCars(){
  printf("creating Cars...\n\n");
  // creating semaphore
  semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if (semd < 0) {
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  printf("semaphore created\n");
  semctl(semd, 0, SETVAL, us);
  //creating shared memory
  shmd = shmget(KEY, sizeof(int) , IPC_CREAT | 0644);
  if (shmd < 0){
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  shmd2 = shmget(KEY2, sizeof(int) , IPC_CREAT | 0644);
  if (shmd2 < 0){
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  printf("shared memory created\n");
  //creating file
}

void viewAvailableCars(){
  struct vehicle* availableCars = shmat(shmd, 0, 0);
  int idx = 0;
  printf("\x1b[H\x1b[J");
  printf("Available Cars (Info for each car is listed as model, color, number of seats, and cost):\n\n")
  for(idx; availableCars[idx] != NULL; idx++) {
    printf("- %s, %s, %d, $%d\n", availableCars[idx].model, availableCars[idx].color, availableCars[idx].seatNumber, availableCars[idx].cost);
  }
  printf("\nType 'back' to go back to the menu, or type the model of the car in order to rent it out\n\n");
}

void viewRentedCars() {
  struct vehicle* rentedCars = shmat(shmd, 0, 0);
  int idx = 0;
  printf("\x1b[H\x1b[J");
  printf("Rented Cars (Info for each car is listed as model, color, number of seats, and cost):\n\n")
  for(idx; rentedCars[idx] != NULL; idx++) {
    printf("- %s, %s, %d, $%d\n", rentedCars[idx].model, rentedCars[idx].color, rentedCars[idx].seatNumber, rentedCars[idx].cost);
  }
  printf("\nType 'back' to go back to the menu\n\n");
}

int removeCars(){
      // Print Contents
      shmd = shmget(KEY, sizeof(int), 0);
      if (shmd< 0){
        printf("sharedy memory error %d: %s\n", errno, strerror(errno));
        return -1;
      }
      shmctl(shmd, IPC_RMID, 0);

      shmd2 = shmget(KEY2, sizeof(int), 0);
      if (shmd2< 0){
        printf("sharedy memory error %d: %s\n", errno, strerror(errno));
        return -1;
      }
      shmctl(shmd2, IPC_RMID, 0);

      printf("shared memory removed\n");


      semd = semget(KEY, 1, 0);
      if (semd< 0) {
        printf("error %d: %s\n", errno, strerror(errno));
        return -1;
      }
      semop(semd, &sb, 1);
      semctl(semd, IPC_RMID, 0);
      printf("semaphore removed\n");
}

int execute (char *args[]){
  int debug = 0;
  if(strcmp(args[1],"-c")==0){
    debug = createCars();
  } else if(strcmp(args[1],"-r")==0){
    debug = removeCars();
  } else if(strcmp(args[1],"-v")==0){
    debug = viewCars();
  } else{
      printf("command not found\n");
      debug = -1;
  }
  return debug;
}

int main(int argc, char *argv[]) {
  us.val=1;
  sb.sem_num=0;
  sb.sem_op =-1;
  if(argc <= 1) {
    printf("%s\n", "You may access this Cars by using the following flags...");
    printf("%s\n", "\"-c\" to create Cars");
    printf("%s\n", "\"-r\" to remove Cars");
    printf("%s\n", "\"-v\" to read Cars's contents");
  } else {
    int executed = execute(argv);
  }
  return 0;
}
