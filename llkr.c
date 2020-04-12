#include <stdio.h>
#include <libinput.h>
#include <libudev.h>
#include <libevdev/libevdev.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
int main(){
    const short largo_ev = 24;

    struct timeval test;
    test.tv_sec = 1886657291;
    test.tv_usec = 967109;

    struct input_event dummy;
    // tipo de acción (tecla)
    dummy.type=4;
    // tecla presionada
    dummy.code=4;
    //tecla A
    dummy.value=31;
    
    struct input_event d;
    // tipo de acción (tecla)
    d.type=0;
    // tecla presionada
    d.code=0;
    //tecla A
    d.value=0;


    struct input_event press;
    // tipo de acción (tecla)
    press.type=EV_KEY;
    // tecla presionada
    press.code=30;
    //tecla A
    press.value=1;

    struct input_event release;
    // tecla presionada
    release.type=EV_KEY;
    //tecla A
    release.code=30;
    // tipo de acción (tecla)
    release.value=0;

    const char *command = "sudo uinput -d /dev/input/event3";
    const char *type = "w";
/*
    sleep(1);
    fwrite(&dummy, sizeof(dummy), 1, stdout);
    fwrite(&press, sizeof(press), 1, stdout);
    fwrite(&d, sizeof(dummy), 1, stdout);
    fwrite(&release, sizeof(release), 1, stdout);
    sleep(1);
    */
    /*

       FILE *c = popen(command,type);
       fwrite(&press,1,largo_ev,c);
       int resultado = pclose(c);


       c = popen(command,type);
       fwrite(&release,1,largo_ev,c);
       resultado = pclose(c);
       sleep(2);
       */
    struct input_event event;

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        if (event.type == EV_KEY){
            event.time.tv_sec =0;
            event.time.tv_usec =0;
            event.code=30;

            //printf("segundos %ld",event.time.tv_sec);
            //printf("\nnano segundos %ld",event.time.tv_usec);
            //printf("\ncode %d",event.code);
            //printf("\n key type %d",event.type);
            //printf("\nvalue %d",event.value);
        }
            printf("segundos %ld \n",event.time.tv_sec);
            printf("nano segundos %ld \n",event.time.tv_usec);
            printf("code %d\n",event.code);
            printf("type %d\n",event.type);
            printf("value %d\n\n",event.value);
        
    }
}

