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

    struct vehicle * cars = (struct vehicle*) shmat(shmd, 0, 0);

    char str_start_date[SEG_SIZE];
    char str_end_date[SEG_SIZE];
    char car[SEG_SIZE];
    printf("Hi! During which days are you interested in borrowing a car?\n")
    printf("We ask that you enter dates in the format of MM-DD\nStarting date:");
    fgets(start_date, SEG_SIZE, stdin);
    printf("Ending date:");
    fgets(end_date, SEG_SIZE, stdin);

    char * str_start_date = parse_args(str_start_date,'-');
    char * str_end_date = parse_args(str_end_date,'-');

    int start_month = atoi(str_start_date[0])
    int start_day = atoi(str_start_date[1])
    int end_month = atoi(str_end_date[0])
    int end_day = atoi(str_end_date[1])

    if(start_month == 2){
      start_month = start_month * 29;
    } else if(start_month%2 == 1){
      start_month = start_month * 30;
    } else{
      start_month = start_month * 31;
    }

    if(end_month == 2){
      end_month = end_month * 29;
    } else if(end_month%2 == 1){
      end_month = end_month * 30;
    } else{
      end_month = end_month * 31;
    }

    int start_date = start_month + start_day;
    int end_date = end_month + end_day;
    printf("The following cars are available during those days:\n");

    for (size_t i = 0; i < 10; i++) {
      struct calendar * availablility = cars[i].calendar;
      int available = 0;
      for (size_t i = start_date; i < end_date + 1 && availablility->unit1[i] == 0; i++) {
        if(i == end_date){
          available = 1;
        }
      }
      if (available){
        printf("%s\n",cars[i].model);
      } else{
        available = 0;
        for (size_t i = start_date; i < end_date + 1 && availablility->unit2[i] == 0; i++) {
          if(i == end_date){
            available = 1;
          }
        }
        if (available){
          printf("%s\n",cars[i].model);
        } else{
          available = 0;
          for (size_t i = start_date; i < end_date + 1 && availablility->unit2[i] == 0; i++) {
            if(i == end_date){
              available = 1;
            }
          }
          if (available){
            printf("%s\n",cars[i].model);
          }
        }
      }
    }
    struct vehicle * chosen_car = NULL;
    while(chosen_car == NULL){
      printf("Please type in the model of the car you'd like to rent: ");
      fgets(car, SEG_SIZE, stdin);
      int i = 0;
      for( i = 0; i < 10; i++){
        if (strcmp(cars[i].model, input) == 0){
          chosen_car = cars[i];
        }
      }
      if(chosen_car == NULL){
        printf("Error: You've entered a model that does not exist in our database.\n");
      }
    }
    int cost = chosen_car.cost * (end_date - start_date);

    for( i = 0; strcmp(cars[i].model, input) == 0; i++) {
        memcpy(&me.rented, &cars[i], sizeof(struct vehicle));
        me.balance -= cars[i].cost;
        availablility = cars[i].calendar;
        if (availablility->unit1[month + day] == 0){
          availablility->unit2[month + day] = me.userid;
        } else if (availablility->unit2[month + day] == 0){
          availablility->unit2[month + day] = me.userid;
        } else if (availablility->unit3[month + day] == 0){
          availablility->unit3[month + day] = me.userid;
        }
    }



    for(size_t i = 0; strcmp(cars[i].model, input) == 0; i++) {
        memcpy(&me.rented, &cars[i], sizeof(struct vehicle));
        me.balance -= cars[i].cost;
        availablility = cars[i].calendar;
        if (availablility->unit1[month + day] == 0){
          availablility->unit2[month + day] = me.userid;
        } else if (availablility->unit2[month + day] == 0){
          availablility->unit2[month + day] = me.userid;
        } else if (availablility->unit3[month + day] == 0){
          availablility->unit3[month + day] = me.userid;
        }
    }

    shmdt(cars);

    sb.sem_op = 1;
    semop(semd, &sb, 1);
    printf("\n");
    return 0;
}
