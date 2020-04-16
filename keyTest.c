// Es necesario instalar los programas intercept yay -S interception-tools
#include <stdlib.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>
#include <string.h>
#include <stdio.h>

int main(void) {
    struct input_event event;

    FILE * input = popen("sudo intercept -g /dev/input/event3","r");

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (fread(&event, sizeof(event), 1, input) == 1) {
        if (event.type == EV_KEY){
            printf("Key code: %d \n",event.code);
            if(event.value == 0){
                printf("Tipo de evento: %d (Tecla soltada)\n",0);
            }
            if(event.value == 1){
                printf("Tipo de evento: %d (Tecla presionada)\n",1);
            }
            if(event.value == 2){
                printf("Tipo de evento: %d (Tecla mantenida)\n",2);
            }
            printf("\n\n\n");
        }
    }
    fclose(input);
}
