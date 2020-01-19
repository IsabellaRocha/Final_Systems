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
    semd = semget(KEY, 1, 0);
    if (semd < 0) {
        printf("Error: %s", strerror(errno));
        return 1;
    }
    semop(semd, &sb, 1);
    shmd = shmget(KEY, sizeof(char*), 0);
    if (shmd < 0) {
        printf("Error: %s", strerror(errno));
        return 1;
    }
    shmd2 = shmget(KEY2, sizeof(char*), 0);
    if (shmd2 < 0) {
        printf("Error: %s", strerror(errno));
        return 1;
    }

    struct vehicle* availableCars= shmat(shmd, 0, 0);
    struct vehicle* rentedCars= shmat(shmd2, 0, 0);

    int idx = 0;
    for(idx; strcmp(rentedCars[idx].model, " ") != 0; idx++) {
      if(strcmp(rentedCars[idx].model, me.rented.model) == 0) {
        memcpy(&availableCars[idx], &rentedCars[idx], sizeof(struct vehicle));
        struct vehicle temp = {"Empty", "Empty", 0, 0};
        memcpy(&rentedCars[idx], &temp, sizeof(struct vehicle));
        memcpy(&me.rented, &temp, sizeof(struct vehicle));
      }
    }
    shmdt(availableCars);
    shmdt(rentedCars);

    sb.sem_op = 1;
    semop(semd, &sb, 1);
    printf("\n");
    return 0;
}
