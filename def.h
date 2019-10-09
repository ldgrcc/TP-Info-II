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
enum estado_t {rep, ini, dob, api, con};

//prototipos de las funciones de estado
int reposo(int e, char b);
int iniciado(int e, char b);
int doblando(int e, char b);
int apilando(int e, char b);
int configurando(int e, char b);

//prototipos de otras funciones
int get_e(int e, int maq);
void set_e(int *e, int dato, int maq);