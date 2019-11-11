// general.c

/* este archivo es para definir funciones que usen todas las funciones
   de estado que se usen en el main() pero no se correspondan con
   funciones de estado.
*/
#include "def.h"

/*==========================================================================*/
/* Funciones de temporizacion
  Habria que evaluar que conviene mas entre usar las funciones o usar las
  macros. Ambas tiene el mismo nombre, la que no se use anteponer el carater "_".
  
  La resolucion de los temporizadores es en decimas de segundo.
  Las funciones se describen por si solas solo mirando el nombre.
*/
#define _start_timer(x) (x)
int start_timer(int dseg) {
	return dseg;
}

#define _stop_timer() 0
int stop_timer() {
	return 0;
}

#define _check_timer(x) (((x))? 0 : 1)
int check_timer(int timer) {
	if (timer) return 0;
	return 1;
}

/*==========================================================================*/
/* Parada de emergencia
  Evaluar si conviene que cada funcion de estado llame a esta funcion, o si
  la ponemos en el main antes de los switch...case.
>> Leandro >> Yo me tiro por la segunda opcion. <<
*/
void emergencia(int *bus) {
  bus[bus_mp] = 0;
  bus[bus_mc] = 0;
  bus[bus_ma] = 0;
  bus[bus_va] = 0;
  bus[bus_timer1] = 0;
  bus[bus_timer2] = 0;
  bus[bus_timer3] = 0;
  return;
}

/*==========================================================================*/
/* Mensajes informativos
*/
void print_info(char *info) {
#ifndef SIS_OP_WIN
  int i;
  gotoxy(0, 20);
  for (i=1; i<=50; i++) printf(" ");
  gotoxy(0, 20);
  printf(info);
#endif
  return;
}

/*==========================================================================*/

void init(int *bus, int* estado)
{
  int i;
  for (i=0; i<BUS_TAMANIO; i++) bus[i] = 0;
  for (i=0; i < CANT_MAQ; i++) estado[i] = 0;
  bus[bus_lote] = 50;
  bus[bus_sfd] = 1;
  bus[bus_t_esp] = 35;
  bus[bus_vel] = 10;
  bus[bus_UI] = UI_reposo_cant;
 return;
}

/*==========================================================================*/
/* Para manejar los mensajes entre maquinas */

/* Busca en la lista si hay algun mensaje para la maq. de est.
  Parametros:
    bus: el vector que representa la maquina.
    para: el numero de la maq. de est.
  Devuelve:
    0 si no hay mensaje para esa maq. de est.
    el contenido del mensaje si es que hay uno.
*/
int get_msg(int *bus, int para) {
  int msg=0, i;
  for (i = bus_msg_lo; i <= bus_msg_hi; i++)
    if ((bus[i] & 7) == para)
    {
      msg = bus[i] >> 3;
      bus[i] = 0;
      if (i<bus_msg_hi) 
      {
        for (;i<bus_msg_hi; i++) bus[i]=bus[i+1];
        bus[bus_msg_hi]=0;
        return msg;
      }
    } 
  return msg;
}

/* Agrega un mensaje para una maq. de est.
  Parametros:
    bus: vector que representa la maquina.
    para: maq. de est. destinataria.
    msg: mensaje.
  Devuelve:
    >0 si pudo grabar el mensaje.
    0 si no pudo grabar el mensaje.
    
nota: msg puede tomar valores desde: (2 ^ ((sizeof (int) * 8) - 3)) - 1
                              hasta: -(2 ^ ((sizeof (int) * 8) - 3))
      debido a que los 3 bits menos signifivativos se usan
      para indicar el deatinatario.
*/
int set_msg(int *bus, int para, int msg)
{
  int i;
  for (i = bus_msg_lo; i <= bus_msg_hi; i++)
  if (bus[i] == 0)
  {
    bus[i] = msg << 3;
    bus[i] |= para;
    return bus_msg_lo - i;
  }
  return 0;
}

/*==========================================================================*/
/* Escribir en el display 16x2
  Parametros:
    linea: el numero de linea del display (1 o 2) donde se quiere escribir
    flags: ???
    str: lo que se quiere escribir
*/
#define evaluar_cmd if (0);
#define si_vale(x) else if(!strcmp(str, (x)))
#define goto_lin1 gotoxy(2, 1)
#define goto_lin2 gotoxy(2, 2)
#define flag(x) (strstr(flags, (x)))
void print16x2(int *bus, int linea, char *str)
{
  char frmt[17], brr[] = {"              "};
  char *destino;
  // Evaluar borrado de linea
  switch (linea & D16x2_MASK_BRR)
  {
    case D16x2_BRR_1:
      sprintf(&bus[bus_disp1], brr);
      break;
    case D16x2_BRR_ALL:
      sprintf(&bus[bus_disp1], brr);
    case D16x2_BRR_2:
      sprintf(&bus[bus_disp2], brr);
      break;
  }
  // Evaluar linea a imprimir
  switch (linea & D16x2_MASK_LIN) // evaluar bit 0
  {
    case D16x2_LIN_1:
      destino = (char*)&bus[bus_disp1];
      break;
    case D16x2_LIN_2:
      destino = (char*)&bus[bus_disp2];
      break;
    default:
      return;
      break;
  }
  // Evaluar alineacion
  switch (linea & D16x2_MASK_ALIN)
  {
    case D16x2_ALIN_IZQ:
      strcpy(frmt, "%-16s");
      break;
    case D16x2_ALIN_DER:
      strcpy(frmt, "%16s");
      break;
  }
  // Imprimir resultado
  sprintf(destino, frmt, str);
  return;
}

/*==========================================================================*/
/* IMPORTANTE: NO DARLE BOLA A LAS SIGUIENTES DOS FUNCIONES,
   ERAN PARA USAR UNA VARIABLE DE ESTADO TIPO int Y USAR
   MASCARAS PARA METER LAS VARIABLES DE ESTADO DE TODAS LAS
   MAQUINAS EN UNA SOLA, LAS CONSERVO POR SI LLEGAN A HACER
   FALTA, PERO ESTAN EN DESHUSO.
*/
/* esta funcion devuelve el estado de la maquina pedida.
parametros:
  e = la variable de estado
  maq = el numero de la maquina de estados
*/
int get_e(int e, int maq) {
	switch (maq) {
		case 1:
		  return e & 0x7;
		  break;
		case 2:
		  return (e & 0x38) >> 3;
		  break;
		case 3:
		  return (e & 0x3c0) >> 6;
		  break;
	}
	return -1;
}

/* esta funcion asigna un valor a la variable
de estado de la maquina en particular.
parametros:
  e = puntero a la variable de estado
  dato = estado que se quiere guardar
  maq = numero de la maqina de estados
*/
int set_e(int e, int dato, int maq) {
	int aux;
	switch (maq) {
		case 1:
		  aux = dato;
		  e = e & ~(0x7);
		  break;
		case 2:
		  aux = dato << 3;
		  e = e & ~(0x38);
		  break;
		case 3:
		  aux = dato << 6;
		  e = e & ~(0x1c0);
		  break;
	}
	e = e | aux;
	return e;
}