# define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <time.h>

char * getOptimalPath(char *path){
    FILE * paths = popen("find /dev/input/by-id","r");
    FILE * realPaths;
    char command[1024];
    char *line = NULL;
    char *realPath = NULL;
    size_t len = 0;

    while (getline(&line, &len, paths) != -1) {
        // reset string
        memset(command,0,sizeof(command));

        strcat(command,"realpath ");

        // get the real path (follow links)
        strcat(command,line);

        realPaths = popen(command,"r");

        getline(&realPath, &len, realPaths);
        realPath = strsep(&realPath,"\n");

        if(strcmp(realPath,path) == 0){
            return strsep(&line,"\n");
        }
    }
    return path;
}
char * keyboardId(){
    char *finalPath;
    char *line = NULL;
    size_t len = 0;

    // get the number of lines of the file
    FILE * lineNumber = popen("find /dev/input/event* | wc -l","r");
    int argc = 0;
    getline(&line, &len, lineNumber);
    sscanf(line,"%d",&argc);

    // get each event path in /dev/input/event*
    FILE * events = popen("find /dev/input/event*","r");

    struct input_event ev[64];
    int fd[argc],rd,idev,value, size = sizeof (struct input_event);
    char *argv[argc];

    for (int i = 0; i < argc; i++) {
        getline(&line, &len, events);
        line = strsep(&line,"\n");
        argv[i] = malloc(sizeof *line);
        strcpy(argv[i],line);
    }

    int ndev=0;
    while(ndev<argc && (fd[ndev] = open(argv[ndev], O_RDONLY|O_NONBLOCK)) != -1){
        ndev++;
    }

    while (1){
        for(idev=0; idev<argc; idev++){
            if( (rd=read(fd[idev], ev, size * 64)) >= size){
                value = ev[0].value;
                if (value != ' ' && ev[1].value == 1 && ev[1].type == 1){
                    finalPath = getOptimalPath(argv[idev]);
                    return finalPath;
                }
            }
        }
        usleep(10);
    }
}
int main(int argc, char *argv[]){
    if(argc > 1){
        printf("USAGE:\n");
        printf("run skrkid as root then press a key from the keyboard you whant to identify\n");
    }
    if(getuid() != 0){
        printf(" \n");
        exit(0);
    }
    printf("%s\n",keyboardId());
    return 0;
}
