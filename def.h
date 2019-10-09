// def.h

#include <stdio.h>
#include <string.h>

/* estos serian los estados de la maquina
cada uno recibe el estado y el boton
presionado. Se me ocurre que b=0 
podria significar que el usuario no presiono
ningun boton. */

/* usar una enumeracion para cada maquina
como si fueran constantes en un #define*/
// general
enum estado_1 {rep, ini, dob, api, con};
enum estado_2 {esp, ade, mov};
enum estado_3 {lote0, t0, lote1=2, lote2=4, lote3=6, lote4=8, t1=2, t2=4};

//prototipos de las funciones de estado 
// -- para la maquina 1
int reposo(int e, char b);
int iniciado(int e, char b);
int doblando(int e, char b);
int apilando(int e, char b);
int configurando(int e, char b);

// -- para la maquina 2
int esperando_fin_lote(int e, char b);
int adelantando(int e, char b);
int moviendo_apilador(int e, char b);

// -- para la maquina 3
int mod_lote(int e, char b);
int mod_tiempo(int e, char b);

//prototipos de otras funciones
int get_e(int e, int maq);
void set_e(int *e, int dato, int maq);