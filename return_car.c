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
    if (strlen(input) != 0) {
      input[strlen(input)-1] = '\0';
    }
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int today_month = tm.tm_mon + 1;
    if (today_month == 2){
      today_month = 29;
    } else if(today_month % 2 == 1){
      today_month = 30;
    } else{
      today_month = 31;
    }
    int today_day = tm.tm_mday;
    bool pay = false;
    if (me.start_rent_day == 0){
      printf("You have currently rented out zero cars!");
    }
    else if(strcmp(input,"Y") == 0 || strcmp(input,"y") == 0){

      if(me.end_rent_month+me.end_rent_day == today_month + today_day){
        printf("Your car rental expires today! Would you like to confirm the return?\n(Y\\N): ");
      } else if(me.end_rent_month+me.end_rent_day < today_month + today_day){
        pay = true;
        printf("Your car is %d days late. Your fee is: %d. Would you like to confirm your payment?\n(Y\\N): ",(today_month + today_day - me.end_rent_month - me.end_rent_day),(today_month + today_day - me.end_rent_month - me.end_rent_day)*30);
      } else{
        printf("You still have %d days left until your rental expires. Would you like to still return the car?\n(Y\\N): ",(me.end_rent_month+me.end_rent_day - today_month - today_day));
      }
      fgets(input, SEG_SIZE, stdin);
      if(strcmp(input,"Y") == 0 || strcmp(input,"y") == 0){

        struct vehicle * car = &me.rented;
        struct calendar * calendar = &car->calendar;

        for (size_t i = 0; i < 3; i++) {
          if(i == 0 && calendar->unit1[me.start_rent_month+me.start_rent_day] == me.userid){
            for (size_t i = me.start_rent_month+me.start_rent_day; i < me.end_rent_month+me.end_rent_day; i++) {
              calendar->unit1[i] = 0;
            }
          }
          if(i == 0 && calendar->unit2[me.start_rent_month+me.start_rent_day] == me.userid){
            for (size_t i = me.start_rent_month+me.start_rent_day; i < me.end_rent_month+me.end_rent_day; i++) {
              calendar->unit2[i] = 0;
            }
          }
          if(i == 0 && calendar->unit3[me.start_rent_month+me.start_rent_day] == me.userid){
            for (size_t i = me.start_rent_month+me.start_rent_day; i < me.end_rent_month+me.end_rent_day; i++) {
              calendar->unit3[i] = 0;
            }
          }
        }
        if (pay){
          me.balance -= (today_month + today_day - me.end_rent_month - me.end_rent_day)*30;
        }
        me.rented = (struct vehicle){" ", " ", 0, 0, {0,0,0}};
        me.start_rent_month = 0;
        me.start_rent_day = 0;
        me.end_rent_month = 0;
        me.end_rent_day = 0;
      } else{
        return 1;
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
