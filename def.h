// def.h
#ifndef DEF_H
#define DEF_H

// Para compatibilidad entre sistemas operativos diferentes
#define SIS_OP_NO_INFO
#include "local.h"

/* Si su sistema operativo no se encuentra entre los siguientes,
agregarlo en un #elif defined(x) (x: es su sistema operativo),
y definir las constantes necesarias. */
#if defined(SIS_OP_NO_INFO)
   // Sin definiciones, solo para cumplir con la consigna anterior
#elif defined(SIS_OP_ANDROID)
   #define BORRAR_PANTALLA "clear"
   #define T_IMP 0.04
#elif defined(SIS_OP_WIN_7)

#endif

// Asegurarse que las constantes necesarias esten definidas
#ifndef BORRAR_PANTALLA
   #error definir: BORRAR_PANTALLA
#endif
#ifndef T_IMP
   #error definir: T_IMP
#endif

// Llibrerias necesarias
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>

typedef unsigned char estado_t;

#define CANT_MAQ 3 /* Cantidad de maquinas de estado */
#define BUS_ELEMENTOS 31
#define ARGS_E estado_t e, int *bus /* Una avreviatura para ahorrar un poco de tipeo ;) */
#define DEFINIR_CRONOMETRO(x) clock_t aux_clk_##x
#define CRONOMETRO(x) ((float) (clock() - aux_clk_##x) / CLOCKS_PER_SEC)
#define REINICIAR_CRONOMETRO(x) aux_clk_##x = clock()

/* Estos serian los estados de la maquina. Cada funcion recibe el estado y el boton
presionado. Se me ocurre que bus[bus_user]=0 podria significar que el usuario no presiono
ningun boton. 
   Usar una enumeracion como si fueran constantes en un #define.
*/

// General (para los estados)
enum estado_maq1 {rep1, ini, dob, api, auxapi, con};
enum estado_maq2 {rep2, esp, cin, mov_d=4, mov_i=8};
enum estado_maq3 {rep3,lote0, t0, lote1=4, lote2=8, lote3=12, lote4=16, t1=20, t2=24};

/* Para simular la maquina usamos el vector 'bus', que cada funcion de estado recibira como parametro
   La siguiente enumeracion serian subindices para manejar el vector. La regla es: "bus_" seguido
   de un nombre corto que identifica a que variable de la maquina (maquina fisica, no cinfundir con
   maquina de estado) hace referencia ese elemento del vector.
*/
enum bus_subindice 
{
  bus_user, // lectura de teclado
  bus_mp, bus_mc, bus_ma, // motores
  bus_va, // valvula de aire
  bus_con, // contador de papeles
  bus_lote, // cantidad deseada por el usuario
  bus_timer1, bus_timer2, bus_timer3, // temporizadores
  bus_sc, bus_sfi, bus_sfd, bus_pre_sc, // sensores
  bus_d1, bus_d2, bus_d3, bus_d4, // digitos del menu 'loteo"
  bus_t1, bus_t2, // digitos del menu 'tiempo de espera'
  bus_msg_lo, bus_msg_1, bus_msg_2,
  bus_msg_3, bus_msg_4, bus_msg_hi
};
                              
/* Descripcion de los subindices.
Formato de la tabla: nombre | descripcion

  - Actuadores:
       bus_mp |  Es el motor que mueve toda la maquina
	      bus_mc |  Es el motor que mueve la cinta transportadora de la salida
	      bus_ma |  Es el motor (bidireccional) que mueve el apilador
	      bus_va |  Es la valvula que hace salir los papeles
	    
 - Sensores:
	      bus_sc |  Cuenta los papeles a medida que van pasando,
	             |    si esta en 1: hay un papel bajo el sensor,
	             |    si esta en 0: no hay papel bajo el sensor.
	  bus_pre_sc |  Auxiliar para guardar el estado previo del sensor contador.
	     bus_sfi |  Sensor de fin de carrera del apilador (izquierdo).
	     bus_sfd |  Sensor de fin de carrera del apilador (derecho).
	    
 - Otros:
     bus_user |  El boton que presiono el usuario. En cada iteracion se recalcula su valor.
              |    si vale 0 significa que el operario no presiono' ningun boton.
      bus_con |  La cantidad de papeles que se llevan contados.
     bus_lote |  La cantidad de papeles que el usuario desea que tenga cada pila.
 bus_timer(x) |  Temporizadores. El sistema decrementa los temporizadores hasta llegar
              |    a cero. Usar temporizadores en vez de la funcion delay() evita que el
              |    programa se cuelgue.
  bus_msg_(x) |  Estos elementos sirven para comunicarse con otras maquinas de estado.
              |    Se usan con las funciones set_msg() y get_msg() (ver la deacripcion
              |    mas adelante).
 
En cada funcion de estado cada uno va a tener que controlar
partes de la maquina, eso se va a hacer asignando valores a
los elementos del vector 'bus'. Por ejemplo:
para encender el motor principal:  bus[bus_mp] = 1;
para apagar el motor principal:  bus[bus_mp] = 0;
verificar si un sensor esta activo: if (bus[bus_sc]==1)...
*/

/* Prototipos de las funciones de estado 
Las funciones que estan marcadas como DEFINIDA_NO faltan hacerlas,
una vez que alguien ya definio una funcion hay que marcarla como DEFINIDA_SI,
sino el compilador deberia tirar un error.
*/
#define DEFINIDA_NO { return e; }
#define DEFINIDA_SI ;

// Para la maquina 1
estado_t reposo_1(ARGS_E) DEFINIDA_SI // rep1 // Estado inicial y de reposo
estado_t iniciado(ARGS_E) DEFINIDA_SI // ini
estado_t doblando(ARGS_E) DEFINIDA_SI // dob
estado_t apilando(ARGS_E) DEFINIDA_SI // api
estado_t configurando(ARGS_E) DEFINIDA_NO // con

// Para la maquina 2
estado_t reposo_2(ARGS_E) DEFINIDA_SI // rep2 //  Estado inicial y de reposo
estado_t esperando_fin_lote(ARGS_E) DEFINIDA_SI // esp
estado_t cinta_acelerada(ARGS_E) DEFINIDA_SI // cin
estado_t moviendo_apilador(ARGS_E) DEFINIDA_SI // mov_i | mov_d

// Para la maquina 3
estado_t reposo_3(ARGS_E) DEFINIDA_NO // rep3 // Estado inicial y de reposo
estado_t mod_lote(ARGS_E) DEFINIDA_NO // lote0
estado_t mod_tiempo(ARGS_E) DEFINIDA_NO // t0

// Funciones de uso general
void print16x2(int linea, char *flags, char *str);
void emergencia(int *bus);
void print_info(char* info);

#endif