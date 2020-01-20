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

    char start_date[SEG_SIZE];
    char end_date[SEG_SIZE];
    char car[SEG_SIZE];
    printf("Hi! During which days are you interested in borrowing a car?\n")
    printf("We ask that you enter dates in the format of MM-DD\nStarting date:");
    fgets(start_date, SEG_SIZE, stdin);
    printf("Ending date:");
    fgets(end_date, SEG_SIZE, stdin);

    char * start_date = parse_args(start_date,'-');

    int month = atoi(start_date[0])
    if(month == 2){
      month = month * 29;
    } else if(month%2 == 1){
      month = month * 30;
    } else{
      month = month * 31;
    }

    printf("The following cars are available during those days:\n");
    for (size_t i = 0; i < count; i++) {
      struct calendar * availablility = cars[i].calendar;
      if (availablility->unit1[month + day] == 0){
        printf("%s\n",cars[i].model);
      } else if (availablility->unit2[month + day] == 0){
        printf("%s\n",cars[i].model);
      } else if (availablility->unit3[month + day] == 0){
        printf("%s\n",cars[i].model);
      }
    }
    printf("Please type in the model of the car you'd like to rent: ");
    fgets(car, SEG_SIZE, stdin);


    int day = atoi(start_date[1])
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
