// general.c

// este archivo es para poner funciones 
// que usemos todos

//#define main_general_c

#ifdef main_general_c
/* todo este bloque de codigo se compila
si se define "main_global_c", es para hacer
pruebas   */
main() {
	int e;
	set_e(&e, 19, 2);
	printf("%d", get_estado(e, 2));
	};
#endif

/* esta funcion devuelve el estado de la
maquina pedida.
parametros:
  e = la variable de estado
  maq = el numero de la maquina de estados
*/
int get_estado(int e, int maq) {
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
void set_e(int *e, int dato, int maq) {
	int aux;
	switch (maq) {
		case 1:
		  aux = dato;
		  *e = *e & ~(0x7);
		  break;
		case 2:
		  aux = dato << 3;
		  *e = *e & ~(0x38);
		  break;
		case 3:
		  aux = dato << 6;
		  *e = *e & ~(0x1c0);
		  break;
	}
	*e = *e | aux;
	return;
}