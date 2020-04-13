// Es necesario instalar los programas intercept yay -S interception-tools
#include <stdlib.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>
#include <string.h>
#include <stdio.h>
#include "fnskr.h"
int main(void) {
    struct libevdev *dev = NULL;
    int fd;
    int rc = 1;

    fd = fopen("/dev/input/event0", 00|01);
    rc = libevdev_new_from_fd(fd, &dev);
    if (rc < 0) {
        fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(-rc));
        exit(1);
    }
    printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
    printf("Input device ID: bus %#x vendor %#x product %#x\n",
            libevdev_get_id_bustype(dev),
            libevdev_get_id_vendor(dev),
            libevdev_get_id_product(dev));

    struct input_event event;

    int teclas[8]={BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK};
    int pattern[8]={KEY_LEFTCTRL,KEY_C,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK};
    int lastKey=BLANK;
    FILE * target = fopen("/dev/input/event3","w");

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (fread(&event, sizeof(event), 1, stdin) == 1) {
            writeEvent(event,target);
        if (event.type == EV_KEY){
            event.code = 30;
            /*
            printf("%ld",event.time.tv_usec);

            if(event.value == TECLA_PRESIONADA){
                printf("presionaste la tecla %d\n",event.code);
                if(lastKey != event.code){append(teclas,event.code);}
                printList(teclas);
                lastKey=event.code;


            }else if(event.value == TECLA_SOLTADA){
                printf("soltaste la tecla %d\n",event.code);
                if(find(teclas,pattern)){
                    printf("Copiar!\n");
                    //fp = popen("sudo uinput -d /dev/input/event26", "w");
                    //fprintf(fp,&event);
                    //status = pclose(fp);

                    pop(teclas,event.code);
                    lastKey=BLANK;
                    return 1;
                }
                pop(teclas,event.code);
                lastKey=BLANK;


            }else if(event.value == TECLA_MANTENIDA){
                //printf("dejaste la tecla %d presionada\n",event.code);
                printf("teclas presionadas: \n");
                printList(teclas);
                if(find(teclas,pattern)){
                    printf("Copiar!\n");
                }
            }
            */
        }
        /*
        else{
        fwrite(&event, sizeof(event), 1, stdout);
        }
        */
    }
    fclose(target);
}
