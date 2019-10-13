// maq1.c

/*
Ejemplo de lo que haria una funcion de estado

El algoritmo es siempre el mismo:
 1: Verificar y comparar los valores de los sensores, contadores y demas variables internas.
 2: Realizar las acciones necesarias (prender o apagar motores, cambiar el valor de alguna bariable, etc).
 3: Determinar el siguiente estado, en nuestro caso hay que tener en cuenta que son 3 maquinas
      de estado, a veces pueden cambiar de estado las tres a veces solo una, o quizas ninguna.
*/

estado_t doblando(ARGS_E) {
	// En este estado la maquina esta en plena produccion
	
	// Verificar sensor contador, y si esta en 1, lo que significa que detecta un papel, aumentar el contador
	if (bus[bus_sc] && !bus[bus_pre_sc]) bus[bus_con]++;
	
	/* Ahora el sistema debe verificar si ya se alcanzo la cantidad pedida,
	  si es asi: hay que detener la produccion,
	  sino: no pasa nada, hay que seguir doblando */
	  if (bus[bus_con] == bus[bus_lote]) {
	  	// Realizar las acciones necesarias y pasar al siguiente estado
	  	bus[bus_va] = 0; // Desactivar la valvula para que deje de salir papel
	  	bus[bus_con] = 0; // Reiniciar el contador
	  	start_timer(2000); // Esperar un tiempo para que los papeles que quedaron en la maquina terminen de salir
	  	bus[bus_mc] = 100; // Acelerar la cinta transportadora de salida
	  	e = set_e(e, ini, 1); // Indicar siguiente estado de la maquina_1: "iniciado"
	  	e = set_e(e, cin, 2); // Indicar el siguiente estado de la maquina_2: "cinta_acelerada"
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