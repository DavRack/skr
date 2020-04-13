#include <stdio.h>
#include <time.h>
#include <libinput.h>
#include <libudev.h>
#include <libevdev/libevdev.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include<fcntl.h> 
int sendEvent(struct input_event event){
    FILE *fp = popen("sudo uinput -d /dev/input/event3","w");
    //recibe un evento y lo envia por medio del teclado solicitado
    //teclado por defecto es event3 
    //ecribe stdin con el contenido de event a uinput
    fwrite(&event,1,sizeof(event),fp);
    pclose(fp);

    return 1;
}

int main(){
    int f;
    fcntl(f, F_SETFL, O_NONBLOCK);
    const short largo_ev = 24;

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

    FILE * fp = fopen("/dev/input/event3","w");
    //FILE * archivo;
    int limit = 10;
    
    //archivo = freopen("/dev/input/event3","w",fp);
    for(int i=0; i < limit; i++){
        fwrite(&dummy,1,sizeof(dummy),fp);
        fwrite(&press,1,sizeof(dummy),fp);
        fwrite(&d,1,sizeof(dummy),fp);

        fwrite(&dummy,1,sizeof(dummy),fp);
        fwrite(&release,1,sizeof(dummy),fp);
        fwrite(&d,1,sizeof(dummy),fp);
        fflush(fp);
        sleep(1);
    }
        fclose(fp);
    /*

       FILE *c = popen(command,type);
       fwrite(&press,1,largo_ev,c);
       int resultado = pclose(c);


       c = popen(command,type);
       fwrite(&release,1,largo_ev,c);
       resultado = pclose(c);
       sleep(2);
       */
    /*
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
    */
}

