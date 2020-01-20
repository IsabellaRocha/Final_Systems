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

char ** parse_args( char * line, char * delimiter ){ //For reading through the txt file of users
    char * parse = removeSpace(line);
    char ** args = malloc(1000 * sizeof(char *));
  //  char * args[1000];
    int c = 0;
    char * token;
    while (parse != NULL){
        token = strsep(&parse, delimiter);
        args[c] = token;
        c++;
    }
    args[c] = NULL;
    free(parse);
    return args;
}

char * removeSpace(char * line) {
    char arg[1000];
    char newLine[1000];
    strcpy(arg, line);
    int idx = 0;
    int cur = 0;
    bool space = false;
    while(idx < strlen(line) && arg[idx] == ' ') { //Get rid of leading spaces
        idx++;
    }
    while(idx < strlen(line)) {
        if(arg[idx] != ' ') {
            space = false;
            newLine[cur] = arg[idx];
            cur++;
            idx++;
        }
        if(arg[idx] == ' ' && space){
            idx++;
        }
        if(arg[idx] == ' ' && !space) {
            space = true;
            newLine[cur] = arg[idx];
            idx++;
            cur++;
        }
    }
    if(cur > 0 && newLine[cur - 1] == ' ') { //In case there's a space after the last charcter
    newLine[cur - 1] = '\0';
}
newLine[cur] = '\0';
char * parse = malloc(50 * sizeof(char));
strcpy(parse, newLine);
return parse;
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


    char *start_date1;
    char *end_date1;
    char car[SEG_SIZE];
    printf("Hi! During which days are you interested in borrowing a car?\n");
    printf("We ask that you enter dates in the format of MM-DD\nStarting date:");
    fgets(start_date1, SEG_SIZE, stdin);
    printf("\nEnding date:");
    fgets(end_date1, SEG_SIZE, stdin);

    char ** str_start_date = parse_args(start_date1,"-");
    char ** str_end_date = parse_args(end_date1,"-");

    int start_month = atoi(str_start_date[0]);
    int start_day = atoi(str_start_date[1]);
    int end_month = atoi(str_end_date[0]);
    int end_day = atoi(str_end_date[1]);

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

    int unit =0;
    for (size_t i = 0; i < 10; i++) {
      struct calendar * availablility = &cars[i].calendar;
      int available = 0;
      for (size_t i = start_date; i < end_date + 1 && availablility->unit1[i] == 0; i++) {
        if(i == end_date){
          available = 1;
        }
      }
      if (available){
        unit = 1;
        printf("%s\n",cars[i].model);
      } else{
        available = 0;
        for (size_t i = start_date; i < end_date + 1 && availablility->unit2[i] == 0; i++) {
          if(i == end_date){
            available = 1;
          }
        }
        if (available){
          unit = 2;
          printf("%s\n",cars[i].model);
        } else{
          available = 0;
          for (size_t i = start_date; i < end_date + 1 && availablility->unit2[i] == 0; i++) {
            if(i == end_date){
              available = 1;
            }
          }
          if (available){
            unit = 3;
            printf("%s\n",cars[i].model);
          } else{
            //if no cars are available, go back to the menu screen
            printf("Sorry, no cars are availble during those days.\n");
            return 1;
          }
        }
      }
    }
    //after showing what cars are available, let them choose
    struct vehicle * chosen_car = NULL;
    while(chosen_car == NULL){
      printf("Please type in the model of the car you'd like to rent: ");
      fgets(car, SEG_SIZE, stdin);
      int i = 0;
      for( i = 0; i < 10; i++){
        if (strcmp(cars[i].model, car) == 0){
          chosen_car = &cars[i];
        } else if (strcmp(cars[i].model, car) == "return"){
          //if user would like to cancel renting  car, go back to the menu screen
          return 1;
        }
      }
      if(chosen_car == NULL){
        printf("Error: You've entered a model that does not exist in our database.\n");
      }
    }

    char input[SEG_SIZE];
    int cost = chosen_car->cost * (end_date - start_date);
    printf("The final price for the rental is: %d\nWould you like to continue with your purchase? (Y\\N)\n", cost);
    fgets(input, SEG_SIZE, stdin);
    if(strcmp(input,"Y") == 0 || strcmp(input,"y") == 0){
      me.balance -= cost;
      if(unit == 1){
        for (size_t i = start_date; i < end_date; i++) {
          chosen_car->calendar.unit1[i] = me.userid;
        }
      } else if(unit == 2){
        for (size_t i = start_date; i < end_date; i++) {
          chosen_car->calendar.unit2[i] = me.userid;
        }
      } else if(unit == 3){
        for (size_t i = start_date; i < end_date; i++) {
          chosen_car->calendar.unit3[i] = me.userid;
        }
      }
      memcpy(&me.rented, &chosen_car, sizeof(struct vehicle));
    } else{
      printf("Your purchase has been cancelled\n");
      return 1;
    }

    shmdt(cars);

    sb.sem_op = 1;
    semop(semd, &sb, 1);
    printf("\n");
    return 0;
}
