#include "headers.h"

int semd, shmd, fd; // desecriptors
union semun us;
struct sembuf sb;

int setUpCars(){
  printf("creating story...\n\n");
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

}

int viewCars(){
  fd = open("story.txt",O_RDONLY);
  if ( fd < 0 ){
    printf("error: %s\n", strerror(fd));
    return -2;
  }
  char story[1000]="\0";
  read(fd,story,1000);
  printf("printing the story so far...\n");
  printf("%s\n",story);
  close(fd);
}

int removeCars(){
      // Print Contents
      viewStory();
      shmd = shmget(SHMKEY, sizeof(int), 0);
      if (shmd< 0){
        printf("sharedy memory error %d: %s\n", errno, strerror(errno));
        return -1;
      }
      shmctl(shmd, IPC_RMID, 0);

      printf("shared memory removed\n");

      remove("story.txt");
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
    debug = createStory();
  } else if(strcmp(args[1],"-r")==0){
    debug = removeStory();
  } else if(strcmp(args[1],"-v")==0){
    debug = viewStory();
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
    printf("%s\n", "You may access this story by using the following flags...");
    printf("%s\n", "\"-c\" to create story");
    printf("%s\n", "\"-r\" to remove story");
    printf("%s\n", "\"-v\" to read story's contents");
  } else {
    int executed = execute(argv);
  }
  return 0;
}
