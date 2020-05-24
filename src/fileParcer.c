#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
int main(){
    FILE *fp = fopen("/home/david/.config/skr/skr.config","r");
    if(!fp){exit(0);}
    int arg1,arg2,arg3,arg4;
    float arg5;
    char *token,*keyWord;
    char *line = NULL;
    size_t len = 0;
    // get line
    while (getline(&line, &len, fp) != -1) {
        keyWord = strsep(&line," ");
        if(strcmp(keyWord,"keyboard") == 0){
            token = strsep(&line,"\n");
            printf("nuevo teclado: %s %s\n",keyWord,token);
        }
        else if(strcmp(keyWord,"mkNewLayer") == 0){
            token = strsep(&line,"\n");
            sscanf(token,"%d",&arg1);
            printf("nueva capa: %s %d\n",keyWord,arg1); 
        }
        else if(strcmp(keyWord,"keyRemap") == 0){

            token = strsep(&line," ");
            sscanf(token,"%d",&arg1);

            token = strsep(&line," ");
            sscanf(token,"%d",&arg2);

            token = strsep(&line," ");
            sscanf(token,"%d",&arg3);

            token = strsep(&line,"\n");
            sscanf(token,"%d",&arg4);
            printf("nuevo keyReap: %s %d %d %d %d\n",keyWord,arg1,arg2,arg3,arg4);
        }
        else if(strcmp(keyWord,"script") == 0){

            token = strsep(&line," ");
            sscanf(token,"%d",&arg1);

            token = strsep(&line," ");
            sscanf(token,"%d",&arg2);

            token = strsep(&line," ");
            sscanf(token,"%f",&arg5);

            token = strsep(&line,"\n");
            printf("nuevo script: %s %d %d %f %s\n",keyWord,arg1,arg2,arg5,token);
        }
    }
    fclose(fp);
}
