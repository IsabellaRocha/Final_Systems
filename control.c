#include "headers.h"

struct users me,user;
int semd, shmd, fd; // desecriptors
union semun us1,us2;
struct sembuf sb[2];

int setUpCars(){
  printf("creating cars...\n\n");
  // creating semaphore
  semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if (semd < 0) {
    printf("ayo");
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  printf("semaphore created\n");
  semctl(semd, 0, SETVAL, us1);

  //creating shared memory
  shmd = shmget(MEMKEY, sizeof(struct vehicle) * 10 , IPC_CREAT | 0644);
  if (shmd < 0){
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  struct calendar calendars[10];
  for (size_t i = 0; i < 10; i++) {
    struct calendar calendar = {0,0,0};
    calendars[i] = calendar;

  }
  struct vehicle car1 = {"Toyota", "Blue", 5, 200, calendars[0]};
  struct vehicle car2 = {"Jeep", "Green", 5, 700, calendars[1]};
  struct vehicle car3 = {"Buick", "Gray", 5, 300, calendars[2]};
  struct vehicle car4 = {"Ford", "White", 5, 600, calendars[3]};
  struct vehicle car5 = {"BMW", "Blue", 5, 1000, calendars[4]};
  struct vehicle car6 = {"Volkswagen", "Red", 2, 500, calendars[5]};
  struct vehicle car7 = {"Subaru", "Gray", 8, 1200, calendars[6]};
  struct vehicle car8 = {"Tesla", "Black", 5, 1000, calendars[7]};
  struct vehicle car9 = {"Honda", "Silver", 5, 400, calendars[8]};
  struct vehicle car10 = {"Lexus", "Blue", 8, 1500, calendars[9]};

  struct vehicle *cars = (struct vehicle*) shmat(shmd, 0, 0);
  struct vehicle allcars[10] = {car1, car2, car3, car4, car5, car6, car7, car8, car9, car10};

  for(size_t i = 0; i < 10; i++) {
    memcpy(&cars[i], &allcars[i], sizeof(struct vehicle));
  }

  shmdt(cars); //All cars start off as available

  printf("shared memory created\n");
  //creating file
}
int setUpUsers(){
  printf("creating login system...\n\n");
  // creating semaphore
  semd = semget(SEM2KEY, 1, IPC_CREAT | 0644);
  if (semd < 0) {
    printf("ayo");
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  printf("semaphore created\n");
  semctl(semd, 0, SETVAL, us2);

  //creating shared memory
  shmd = shmget(MEM2KEY, 100 * sizeof(struct users) , IPC_CREAT | 0644);
  if (shmd < 0){
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  printf("shared memory created\n");

}
int viewAvailableCars(){
  //catching for errors
  printf("trying to get in...\n");
  semd=semget(SEMKEY,1,0);
  if(semd < 0){
    printf("semaphore Error: %s\n", strerror(semd));
    return -1;
  }
  semop(semd,&sb[0],1);

  shmd=shmget(MEMKEY,sizeof(struct vehicle) * 10,0);
  if(shmd < 0){
    printf("shared memory Error: %s\n", strerror(shmd));
    return -1;
  }

  struct vehicle* availableCars = shmat(shmd, 0, 0);
  printf("\x1b[H\x1b[J");
  printf("Available Cars (Info for each car is listed as model, color, number of seats, and cost):\n\n");
  for(size_t i = 0; i < 10 && strcmp(availableCars[i].model, " ") != 0; i++) {
    printf("- %s, %s, %d seats, rental cost per day: $%d\n", availableCars[i].model, availableCars[i].color, availableCars[i].seatNumber, availableCars[i].cost);
  }
  printf("\nType 'back' to go back to the menu, or type rent if you'd like to \'rent\' out a car\n\n");

  shmdt(availableCars);
  sb[0].sem_op=1;
  semop(semd,&sb[0],1);
}

int removeCars(){
      // Print Contents
      shmd = shmget(MEMKEY, sizeof(struct vehicle) * 10 , 0);
      if (shmd< 0){
        printf("sharedy memory error %d: %s\n", errno, strerror(errno));
        return -1;
      }
      shmctl(shmd, IPC_RMID, 0);
      printf("shared memory removed\n");
      semd = semget(SEMKEY, 1, 0);
      if (semd< 0) {
        printf("error %d: %s\n", errno, strerror(errno));
        return -1;
      }
      semop(semd, &sb[0], 1);
      semctl(semd, IPC_RMID, 0);
      printf("semaphore removed\n");
}
int removeUsers(){
      // Print Contents
      shmd = shmget(MEM2KEY, sizeof(struct users) * 100 , 0);
      if (shmd< 0){
        printf("sharedy memory error %d: %s\n", errno, strerror(errno));
        return -1;
      }
      shmctl(shmd, IPC_RMID, 0);
      printf("shared memory removed\n");
      semd = semget(SEM2KEY, 1, 0);
      if (semd< 0) {
        printf("error %d: %s\n", errno, strerror(errno));
        return -1;
      }
      semop(semd, &sb[1], 1);
      semctl(semd, IPC_RMID, 0);
      printf("semaphore removed\n");
      remove("users.txt");
      printf("file removed\n");
}

int execute (char *args[]){
  int debug = 0;
  if(strcmp(args[1],"-c")==0){
    debug = setUpCars();
    debug = setUpUsers();
  } else if(strcmp(args[1],"-r")==0){
    debug = removeUsers();
    debug = removeCars();
  } else if(strcmp(args[1],"-va")==0){
    debug = viewAvailableCars();
  } else{
      printf("command not found\n");
      debug = -1;
  }
  return debug;
}

int main(int argc, char *argv[]) {
  us1.val=1;
  us2.val=1;
  sb[0].sem_num=0;
  sb[0].sem_op =-1;
  sb[0].sem_flg = SEM_UNDO;
  sb[1].sem_num=0;
  sb[1].sem_op =-1;
  sb[1].sem_flg = SEM_UNDO;
  if(argc <= 1) {
    printf("%s\n", "You may access this Cars by using the following flags...");
    printf("%s\n", "\"-c\" to create Cars and User Login System");
    printf("%s\n", "\"-r\" to remove Cars and Users");
    printf("%s\n", "\"-va\" to read Cars's contents");
  } else {
    int executed = execute(argv);
  }
  return 0;
}
