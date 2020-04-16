#include <libevdev/libevdev.h>
#include <libinput.h>
#include <libudev.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <time.h>
#include <unistd.h>

#include "remap.h"

# define BLANK -1
# define EV_SIZE 24

# define TECLA_MANTENIDA 2
# define TECLA_PRESIONADA 1
# define TECLA_SOLTADA 0

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

struct input_event rap1; //inicio de evento
struct input_event rap2; //finalización de evento
struct input_event event; // plantilla para enviar un evento press
struct input_event ev; 

int sendEvent(struct input_event evento, FILE * kb){
    //recibe un evento y lo envia por medio del teclado solicitado
    //teclado por defecto es event3 

    fwrite(&rap1,1,EV_SIZE,kb);
    fwrite(&evento,1,EV_SIZE,kb);
    fwrite(&rap2,1,EV_SIZE,kb);
    fflush(kb);

    return 1;
}
int sendKeyEvent(int KEY,int tipo){
    // recibe:
    //          KEY     -> Número de la tecla
    //          tipo    -> 0=soltada, 1=presionada, 2=mantenida

    // se idica que event es de tipo KEY
    event.type=EV_KEY;
    event.code = KEY;
    event.value = tipo;

    // Se efectua la escritura al archivo
    sendEvent(event,teclado);
    
    return 1;
}

int main(){
    // ruta absoluta al teclado que se va a usar
    // este archivo debe tener permisos 666
    //const char * ruta = "/dev/input/event3";

    //se usa el comando intercept para obtener los eventos
    //generados por el teclado especificado


    // Se asignan lo valores para el primer evento especial rap1
    rap1.type = 4;
    rap1.code = 4;
    rap1.value = 31;

    // Se asignan lo valores para el primer evento especial rap2
    rap2.type = 0;
    rap2.code = 0;
    rap2.value = 0;

    // se asigna el teclado en la ruta especificada al teclado
    teclado = popen("sudo uinput -d /dev/input/event3","w");

    // teclas guarda la o las teclas presionadas en el momento
    int teclas[8]={BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK};

    makeRemaps();

    int eventoEnviado;

    // Ciclo principal de la aplicación!
    // se lee cada evento generado por el teclado
    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (fread(&ev, sizeof(event), 1, stdin) == 1) {
        if(ev.type == EV_KEY){
            eventoEnviado = -1;
            // tecla presionada
            // agregar tecla a teclas

            if(ev.value == TECLA_PRESIONADA){
                append(teclas,ev.code);
                eventoEnviado = getMatchIndex(teclas);
                if(eventoEnviado != -1){
                    sendKeyEvent(remaps[eventoEnviado].to,TECLA_PRESIONADA);
                }
            }
            else if(ev.value == TECLA_SOLTADA){
                // tecla soltada
                // quitar tecla de teclas
                eventoEnviado = getMatchIndex(teclas);
                if(eventoEnviado != -1){
                    sendKeyEvent(remaps[eventoEnviado].to,TECLA_SOLTADA);
                }
                pop(teclas,ev.code);
            }
            else if(ev.value == TECLA_MANTENIDA){
                // tecla mantenida
                eventoEnviado = getMatchIndex(teclas);
                if(eventoEnviado != -1){
                    sendKeyEvent(remaps[eventoEnviado].to,TECLA_MANTENIDA);
                }
            }

            if(eventoEnviado == -1){
                sendKeyEvent(ev.code,ev.value);
            }
        }
    }
}
/*
 * TODO:
 * - eleminar la necesidad de usar stdin al ejecutar el programa
 * - revisar el reemplazo de las teclas rehacerlo con un vector 
 * - buscar una forma de medir el rendimiento desde c
 * - 
 *  
 */
