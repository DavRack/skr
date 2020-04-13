#include <stdio.h>
#include <linux/input.h>
#include <libevdev/libevdev-uinput.h>
#define _GNU_SOURCE

int sendEvent(struct input_event event){
    FILE *fp = popen("sudo uinput -d /dev/input/event3","w");
    //recibe un evento y lo envia por medio del teclado solicitado
    //teclado por defecto es event3 
    //ecribe stdin con el contenido de event a uinput
    fwrite(&event,1,sizeof(event),fp);
    pclose(fp);

    return 1;
}

int main(int argc, char **argv){
}
