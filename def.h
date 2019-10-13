// def.h
#ifndef DEF_H
#define DEF_H

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>

/* estos serian los estados de la maquina
cada uno recibe el estado y el boton
presionado. Se me ocurre que bus[0]=0 
podria significar que el usuario no presiono
ningun boton. */

typedef unsigned char estado_t;
#define CANT_MAQ 3
#define BUS_ELEMENTOS 31
#define ARGS_E estado_t e, int bus[BUS_ELEMENTOS]
#define DEFINIR_CRONOMETRO(x) clock_t aux_clk_##x
#define CRONOMETRO(x) ((float) (clock() - aux_clk_##x) / CLOCKS_PER_SEC)
#define REINICIAR_CRONOMETRO(x) aux_clk_##x = clock()

/* Usar una enumeracion para cada maquina
como si fueran constantes en un #define*/
// General
enum estado_1 {rep1, ini, dob, api, con};
enum estado_2 {rep2, esp, cin, mov_i, mov_d};
enum estado_3 {rep3,lote0, t0, lote1=4, lote2=8, lote3=12, lote4=16, t1=20, t2=24};

// Para simular la maquina usamos el vector 'bus', que cada funcion de estado recibira como parametro
// La siguiente enumeracion serian subindices para manejar el vector
enum bus_msg {bus_user, // lectura de teclado
                              bus_mp, bus_mc, bus_ma, // motores
                              bus_va, // valvula de aire
                              bus_con, // contador de papeles
                              bus_lote, // cantidad deseada por el usuario
                              bus_timer1, bus_timer2, bus_timer3, // temporizadores
                              bus_sc, bus_pre_sc, bus_sf1, bus_sf2, // sensores
                              bus_d1, bus_d2, bus_d3, bus_d4, // digitos del menu 'loteo"
                              bus_t1, bus_t2, // digitos del menu 'tiempo de espera'
                              };
                              
/* Descripcion de los subindices
 - Actuadores
      bus_mp |  Es el motor que mueve toda la maquina
	    bus_mc |  Es el motor que mueve la cinta transportadora de la salida
	    bus_ma |  Es el motor (bidireccional) que mueve el apilador
	     bus_va  |  Es la valvula que hace salir los papeles
	    
 - Sensores
	            bus_sc  |  Cuenta los papeles a medida que van pasando,
	                              si esta en 1: hay un papel bajo el sensor,
	                              si esta en 0: no hay papel bajo el sensor.
	    bus_pre_sc  |  Auxiliar para guardar el estado previo del sensor contador.
	          bus_sf1  |  Sensor de fin de carrera del apilador.
	          bus_sf2  |  Sensor de fin de carrera del apilador.
	    
 - Otros
            bus_user  |  El boton que presiono el usuario
             bus_con  |  La cantidad de papeles que se llevan contados
             bus_lote  |  La cantidad de papeles que el usuario desea que tenga cada pila
      bus_timer(x)  |  Temporizadores. El sistema decrementa los temporizadores hasta llegar
                                  a cero. Usar temporizadores en vez de la funcion delay() evita que el
                                  programa se cuelgue.
                                
En cada funcion de estado cada unovba a tener que controlar
partes de la maquina, eso se va a hacer asignando valores a
los elementos del vector 'bus'. Por ejemplo:
  para encender el motor principal:  bus[bus_mp] = 127;
  para apagar el motor principal:  bus[bus_mp] = 0;
*/

/* Prototipos de las funciones de estado 
Las funciones que estan marcadas como DEFINIDA_NO faltan hacerlas,
una vez que alguien ya definio una funcion hay que marcarla como DEFINIDA_SI,
sino el compilador deberia tirar un error.
*/
#define DEFINIDA_NO { return 0; }
#define DEFINIDA_SI ;

// Para la maquina 1
estado_t reposo_1(ARGS_E) DEFINIDA_NO // Estado inicial y de reposo
estado_t iniciado(ARGS_E) DEFINIDA_NO
estado_t doblando(ARGS_E) DEFINIDA_SI
estado_t apilando(ARGS_E) DEFINIDA_NO
estado_t configurando(ARGS_E) DEFINIDA_NO

// Para la maquina 2
estado_t reposo_2(ARGS_E) DEFINIDA_NO // Estado inicial y de reposo
estado_t esperando_fin_lote(ARGS_E) DEFINIDA_NO
estado_t cinta_acelerada(ARGS_E) DEFINIDA_NO
estado_t moviendo_apilador(ARGS_E) DEFINIDA_NO

// Para la maquina 3
estado_t reposo_3(ARGS_E) DEFINIDA_NO // Estado inicial y de reposo
estado_t mod_lote(ARGS_E) DEFINIDA_NO
estado_t mod_tiempo(ARGS_E) DEFINIDA_NO

#endif