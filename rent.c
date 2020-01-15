#include "headers.h"

int semd, shmd, fd; // desecriptors
union semun us;
struct sembuf sb;

int setUpCars(){
  printf("creating Cars...\n\n");
  // creating semaphore
  semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if (semd < 0) {
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  printf("semaphore created\n");
  semctl(semd, 0, SETVAL, us);
  //creating shared memory
  shmd = shmget(SHMKEY, sizeof(int) , IPC_CREAT | 0644);
  if (shmd < 0){
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  printf("shared memory created\n");
  //creating file
  fd = open("Cars.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
  if (fd < 0){
    printf("error %d: %s\n", errno, strerror(errno));
    return -1;
  }
  close(fd);
  printf("file created\n");

}

int viewCars(){
  fd = open("Cars.txt",O_RDONLY);
  if ( fd < 0 ){
    printf("error: %s\n", strerror(fd));
    return -2;
  }
  char Cars[1000]="\0";
  read(fd,Cars,1000);
  printf("printing the Cars so far...\n");
  printf("%s\n",Cars);
  close(fd);
}

int removeCars(){
      // Print Contents
      viewCars();
      shmd = shmget(SHMKEY, sizeof(int), 0);
      if (shmd< 0){
        printf("sharedy memory error %d: %s\n", errno, strerror(errno));
        return -1;
      }
      shmctl(shmd, IPC_RMID, 0);

      printf("shared memory removed\n");

      remove("Cars.txt");
      printf("file removed\n");

      semd = semget(SEMKEY, 1, 0);
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
