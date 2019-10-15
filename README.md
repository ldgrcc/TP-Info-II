# TP-Info-II
Repositorio para el TP de la dobladora de Informatica II

================================================================================
Cada uno debe tener un archivo llamado "local.h" con el siguiente contenido:

/*   local.h   */

#ifndef LOCAL_H
#define LOCAL_H

#ifdef SIS_OP_NO_INFO
  #undef SIS_OP_NO_INFO
#endif

// Sistema operativo
#define SIS_OP_ANDROID

// Compilador
#define no_compila_varios_punto_c

#endif

donde dice SIS_OP_ANDROID hay que poner el s.o. de cada una, por ejemplo
SIS_OP_WIN_7 para windows 7.
Este archivo no se sube ni clona del repositorio. "def.h" lo mete en un include,
por lo que si no existe deberia dar un error.

====================================================================================
{
Se me ocurre que cada una de nuestra máquinas use la misma variable de estado,
De la siguiente manera:
Los 3 primeros bit para la máquina 1.
Bits 3, 4 y 5 para la máquina 2,
Los demás bits para la máquina 3.
Para poder obtener el estado de cada maquina
usamos una mascara, metida en una funcion.
} Hubo cambios en esto, lo conservo como referencia.

/*
Ejemplo de lo que haria una funcion de estado

El procedimiento es siempre el mismo:
 1: Verificar y comparar los valores de los sensores, contadores y demas variables internas.
 2: Realizar las acciones necesarias (prender o apagar motores, cambiar el valor de alguna bariable, etc).
 3: Determinar el siguiente estado, en nuestro caso hay que tener en cuenta que son 3 maquinas
      de estado, a veces pueden cambiar de estado las tres a veces solo una, o quizas ninguna.
*/

int doblando(int e, int *bus) {
	// En este estado la maquina esta en plena produccion
	
	// Verificar sensor contador, y si esta en 1, lo que significa que detecta un papel, aumentar el contador
	if (bus[bus_sc]) bus[bus_con]++;
	
	/* Ahora el sistema debe verificar si ya se alcanzo la cantidad pedida,
	  si es asi: hay que detener la produccion,
	  sino: no pasa nada, hay que seguir doblando */
	  if (bus[bus_con == bus[bus_lote]) {
	  	// Realizar las acciones necesarias y pasar al siguiente estado
	  	bus[bus_va] = 0; // Desactivar la valvula para que deje de salir papel
	  	e = set_e(e, ini, 1); // Indicar siguiente estado de la maquina_1
	  	e = set_e(e, cin, 2); // Indicar el siguiente estado de la maquina_2
	  	// La maquina_3 no cambia de estado
	  }
	// Siempre se devuelve un valor de estado
	return e;
}

/*
Y bue... cada funcion no deberia ser mucho mas complicada que esto.
Recordar que el vector "bus" representa la maquina fisica, cadacelemento
del vector esta relacionado con algun sensor, algun motor, o alguna variable,
como por ejemplocun contador.
Y la variable "e" o "estado" solo se encarga de determinar la siguiente
funcion que va a ser invocada.
*/
