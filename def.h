// def.h
#ifndef DEF_H
#define DEF_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>

/* estos serian los estados de la maquina
cada uno recibe el estado y el boton
presionado. Se me ocurre que bus[0]=0 
podria significar que el usuario no presiono
ningun boton. */

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
      bus_mp | es el motor que mueve toda la maquina
	    bus_mc | es el motor que mueve la cinta transportadora de la salida
	    bus_ma | es el motor (bidireccional) que mueve el apilador
	    bus_va  | es la valvula que hace salir los papeles
	    
 - Sensores
	    bus_sc    | cuenta los papeles a medida que van pasando,
	                       si esta en 1: hay un papel bajo el sensor,
	                       si esta en 0: no hay papel bajo el sensor
	    bus_pre_sc  | auxiliar para guardar el estado previo del sensor contador
	    bus_sf1  | sensor de fin de carrera del apilador
	    bus_sf2  | sensor de fin de carrera del apilador
	    
 - Otros
      bus_user | el boton que presiono el usuario
      bus_con  | la cantidad de papeles que se llevan contados
      bus_lote  | la cantidad de papeles que el usuario desea que tenga cada pila
      
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
int reposo_1(int e, int *bus) DEFINIDA_NO // Estado inicial y de reposo
int iniciado(int e, int *bus) DEFINIDA_NO
int doblando(int e, int *bus) DEFINIDA_SI
int apilando(int e, int *bus) DEFINIDA_NO
int configurando(int e, int* bus) DEFINIDA_NO

<<<<<<< HEAD
// Para la maquina 2
int reposo_2(int e, int *bus) DEFINIDA_NO // Estado inicial y de reposo
int esperando_fin_lote(int e, int *bus) DEFINIDA_NO
int cinta_acelerada(int e, int *bus) DEFINIDA_NO
int moviendo_apilador(int e, int *bus) DEFINIDA_NO

// Para la maquina 3
int reposo_3(int e, int *bus) DEFINIDA_NO // Estado inicial y de reposo
int mod_lote(int e, int *bus) DEFINIDA_NO
int mod_tiempo(int e, int *bus) DEFINIDA_NO

// Prototipos de otras funciones
int get_e(int e, int maq) DEFINIDA_SI
int set_e(int e, int dato, int maq) DEFINIDA_SI
int start_timer(int milis) DEFINIDA_SI
int stop_timer() DEFINIDA_SI
int check_timer(int tmr) DEFINIDA_SI

#define _start_timer(x) x
#define _stop_timer() 0

#endif