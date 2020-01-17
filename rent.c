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
    printf("trying to get in\n");
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
    printf("Your addition: ");
    fgets(input, SEG_SIZE, stdin);
    write(fd, input, strlen(input));
    shmdt(input);
    close(fd);
    strcpy(line, input);
    sb.sem_op = 1;
    semop(semd, &sb, 1);
    printf("\n");
    return 0;
}
