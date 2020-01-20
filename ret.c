#include "headers.h"
int shmd, shmd2, semd;
struct sembuf sb;
int my_write();
struct users me;

int main() {
  sb.sem_num = 0;
  //sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  my_write();
  return 0;
}

int my_write() {
    semd = semget(SEMKEY, 1, 0);
    if (semd < 0) {
        printf("semaphore error: %s", strerror(errno));
        return 1;
    }
    semop(semd, &sb, 1);
    shmd = shmget(MEMKEY, sizeof(char*), 0);
    if (shmd < 0) {
        printf("memory error: %s", strerror(errno));
        return 1;
    }

    struct vehicle* availablecars = (struct vehicle*) shmat(shmd, 0, 0);

    char input[SEG_SIZE];
    printf("Hi! Can you confirm that you are here to return your car? (Y\\N): ")
    fgets(input, SEG_SIZE, stdin);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int today_month = tm.tm_mon + 1;
    int today_day = tm.tm_mday;

    if(strcmp(input,"Y") == 0 || strcmp(input,"y") == 0){
      struct vehicle = me.rented;

      if(0){
        printf("Your car is %d day late. Your fee is: %d. Would you like to confirm your payment? (Y\\N): ")
        fgets(input, SEG_SIZE, stdin);
      } else if(0){
        printf("You still have %d days left until your rental expires. Would you like to still return the car? (Y\\N): ")
        fgets(input, SEG_SIZE, stdin);
      } else{
        printf("Can you confirm the return of your car? (Y\\N): ")
      }
    } else {
      return;
    }
    
    shmdt(cars);

    sb.sem_op = 1;
    semop(semd, &sb, 1);
    printf("\n");
    return 0;
}
