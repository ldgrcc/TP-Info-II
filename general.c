// general.c

/* este archivo es para poner funciones 
/* que usemos todos
*/


/* esta funcion devuelve el estado de la
maquina pedida.
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

// Funciones de temporizacion
int start_timer(int dseg) {
	return dseg;
}

int stop_timer() {
	return 0;
}

int check_timer(int timer) {
	if (timer) return 0;
	return 1;
}
