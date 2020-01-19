#include "headers.h"
int shmd, shmd2, semd;
struct sembuf sb;
int my_write();

int main() {
  sb.sem_num = 0;
  //sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  my_write();
  return 0;
}

int my_write() {
    semd = semget(KEY, 1, 0);
    if (semd < 0) {
        printf("Error: %s", strerror(errno));
        return 1;
    }
    semop(semd, &sb, 1);
    shmd = shmget(KEY, sizeof(char*), 0);
    shmd2 = shmget(KEY2, sizeof(char*), 0);
    if (shmd < 0) {
        printf("Error: %s", strerror(errno));
        return 1;
    }

    struct vehicle* availableCars= shmat(shmd, 0, 0);
    struct vehicle* rentedCars= shmat(shmd2, 0, 0);

    char input[SEG_SIZE];
    printf("Please type in the model of which car you'd like to rent: ");
    fgets(input, SEG_SIZE, stdin);
    int idx = 0;
    for(idx; availableCars[idx] != NULL; idx++) {
      if(strcmp(availableCars[idx].model, input) == 0) {
        rentedCars[idx] = availableCars[idx];
        me.balance -= availableCars[idx].cost;
        struct vehicle temp = {"", "", 0, 0};
        availableCars[idx] = temp;
      }
    }
    shmdt(availableCars);
    shmdt(rentedCars);

    sb.sem_op = 1;
    semop(semd, &sb, 1);
    printf("\n");
    return 0;
}
