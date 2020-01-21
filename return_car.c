#include "headers.h"

int return_car() {
    sb1.sem_num = 0;
    sb1.sem_op = -1;
    sb1.sem_flg = SEM_UNDO;

    semd = semget(SEMKEY, 1, 0);
    if (semd < 0) {
        printf("semaphore error: %s", strerror(errno));
        return 1;
    }
    semop(semd, &sb1, 1);
    shmd = shmget(MEMKEY, sizeof(struct vehicle) * 10, 0);
    if (shmd < 0) {
        printf("memory error: %s", strerror(errno));
        return 1;
    }

    struct vehicle* cars = shmat(shmd, 0, 0);

    char input[SEG_SIZE];
    printf("Hi! Can you confirm that you are here to return your car? (Y\\N): ");
    fgets(input, SEG_SIZE, stdin);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int today_month = tm.tm_mon + 1;
    int today_day = tm.tm_mday;

    bool flag = false;
    if(strcmp(input,"Y") == 0 || strcmp(input,"y") == 0){
      struct vehicle * car = &me.rented;
      struct calendar * calendar = &car->calendar;
      for (size_t i = 0; i < 3; i++) {
        if(i == 0){
          for (size_t i = 0; i < 365; i++) {
            if(calendar->unit1.[i] == me.userid && !flag){

              flag = true;
            } if (flag && calendar->unit1.[i] != me.userid){

            }
          }
        }
      }
      for()
      if(0){
        printf("Your car is %d days late. Your fee is: %d. Would you like to confirm your payment? (Y\\N): ",5,6);
        fgets(input, SEG_SIZE, stdin);
      } else if(0){
        printf("You still have %d days left until your rental expires. Would you like to still return the car? (Y\\N): ",6);
        fgets(input, SEG_SIZE, stdin);
      } else{
        printf("Can you confirm the return of your car? (Y\\N): ");
      }
    } else {
      return 1;
    }

    shmdt(cars);

    sb1.sem_op = 1;
    semop(semd, &sb1, 1);
    printf("\n");
    return 0;
}
