
 En el archivo definitions.h se encuentran las definiciones de
 las estructuras de datos para:
      - Remapeo de teclas
      - Lanzamiento de scripts
      - Capas

 En el archivo config.h se encuentran las definiciones del usuario de:
      - Remapeo de teclas
      - Lanzamiento de scripts
      - Capas

 ## Funcionamiento del envio de teclas 
 cada key_stroke esta compuesto por 3 eventos
 un evento per se envuelto entre dos eventos especiales

      (Inicio envoltorio) ---------------> rap1

                                 +-------> (evento press)
                                 |               
                  (event)  -> ---+-------> (evento release)
                                 |               
                                 +-------> (evento repeat)

      (Fin envoltorio) ------------------> rap2

 ## Funcionamiento de una estructura input_event 
 Una estructura input_event está compuesta (para nuestro proposito)
 de los siguientes valores

 input event{
      type  = tipo de evento (cuando es un tecleo: type=EV_KEY)
      code  = numero de la tecla presionada: (code = 30) -> KEY_A
      value = tipo evento de tecla 0=soltada 1=presionada 2=mantenida
 }


