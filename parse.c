#include "headers.h"

char ** parse_args( char * line, char * delimiter ){ //For reading through the txt file of users
    char * parse = removeSpace(line);
    char ** args = calloc(sizeof(char *),1000);
  //  char * args[1000];
    int c = 0;
    while (parse != NULL){
        args[c] = strsep(&parse, delimiter);
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
    char * parse = calloc(sizeof(char),50);
    strcpy(parse, newLine);
    return parse;
}

// int createMem() {
//   int status;
//   if(fork() == 0) {
//     char* args[] = {"./initialize", "-c", NULL};
//     int execute = execvp("./initialize", args);
//     if(execute < 0) {
//       printf("Error: %s", strerror(errno));
//       return 1;
//     }
//   }
//   else {
//     wait(&status);
//   }
// }
