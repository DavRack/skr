#include <stdio.h>
#include <time.h>
#include <libinput.h>
#include <libudev.h>
#include <libevdev/libevdev.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
# define BLANK -1
# define TECLA_PRESIONADA 1
# define TECLA_SOLTADA 0
# define TECLA_MANTENIDA 2
# define EV_SIZE 24

/*
 * ######## Como funciona el envio de teclas: ########
 * cada key_stroke esta compuesto por 3 eventos
 * un evento per se envuelto entre dos eventos especiales
 *
 *      (Inicio envoltorio) ---------------> rap1
 *
 *
 *                                 +-------> (evento press)
 *                                 |               
 *                  (event)  -> ---+-------> (evento release)
 *                                 |               
 *                                 +-------> (evento repeat)
 *
 *
 *      (Fin envoltorio) ------------------> rap2
 *
 *
 * ######## Como funciona una estructura input_event ########
 * Una estructura input_event está compuesta (para nuestro proposito)
 * de los siguientes valores
 *
 * input event{
 *      type  = tipo de evento (cuando es un tecleo: type=EV_KEY)
 *      code  = numero de la tecla presionada: (code = 30) -> KEY_A
 *      value = tipo evento de tecla 0=soltada 1=presionada 2=mantenida
 * }
 *
 */

// al archivo teclado se escriben los eventos
FILE * teclado;
struct input_event event; // plantilla para enviar un evento press
struct input_event rap1; //inicio de evento
struct input_event rap2; //finalización de evento


int sendEvent(struct input_event ev, FILE * kb){
    //recibe un evento y lo envia por medio del teclado solicitado
    //teclado por defecto es event3 
    event.type=EV_KEY;
    event.code=30;
    event.value=1;

    // Se asignan lo valores para el primer evento especial rap1
    rap1.type=4;
    rap1.code=4;
    rap1.value=31;

    // Se asignan lo valores para el primer evento especial rap2
    rap2.type=0;
    rap2.code=0;
    rap2.value=0;
    
    fwrite(&rap1,1,sizeof(rap1),kb);
    fwrite(&event,1,sizeof(event),kb);
    fwrite(&rap2,1,sizeof(rap2),kb);
    fflush(kb);
    event.value=0;

    fwrite(&rap1,1,sizeof(rap1),kb);
    fwrite(&event,1,sizeof(event),kb);
    fwrite(&rap2,1,sizeof(rap2),kb);
    // "guarda" los cambios al archivo
    fflush(kb);

    return 1;
}
int sendKeyEvent(int KEY,int tipo){
    // recibe:
    //          KEY     -> Número de la tecla
    //          tipo    -> 0=soltada, 1=presionada, 2=mantenida
    
    // Se asignan lo valores para la plantilla evento
    // a la hora de usarse debe especificar code y value
    struct input_event event; // plantilla para enviar un evento press
    event.type=EV_KEY;
    event.code=KEY;
    event.value=tipo;

    // Se efectua la escritura al archivo
    sendEvent(event,teclado);
    
    return 1;
}

int main(){
    // ruta absoluta al teclado que se va a usar
    // este archivo debe tener permisos 666
    const char * ruta = "/dev/input/event3";
    struct input_event ev;

    ev.type = EV_KEY;
    ev.code=30;
    ev.value=1;

    // se asigna el teclado en la ruta especificada al teclado
    teclado = fopen(ruta,"a");

    int limit = 10;

    //archivo = freopen("/dev/input/event3","w",fp);
    for(int i=0; i < limit; i++){

        // presionar la tecla a
        sendEvent(ev,teclado);
        //por algún motivo el evento no se envia bien

        // soltar la tecla a
        sleep(1);

    }
   /* 
    event.type = EV_KEY;
    event.code=30;
    event.value=1;

    fwrite(&rap1,1,EV_SIZE,teclado);
    fwrite(&event,1,EV_SIZE,teclado);
    fwrite(&rap2,1,EV_SIZE,teclado);

    event.value = 0;

    fwrite(&rap1,1,EV_SIZE,teclado);
    fwrite(&event,1,EV_SIZE,teclado);
    fwrite(&rap2,1,EV_SIZE,teclado);
    // cierra el archivo teclado
    fclose(teclado);
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

