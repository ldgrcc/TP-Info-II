//main.c

#include "def.h"

/*esto es por si el compilador no compila
todos los "*.c" del proyecto y solo compila
el archivo actual. Si no es el caso, dejar
comentado el siguiente #define */
#define no_compila_varios_punto_c
#ifdef no_compila_varios_punto_c
  #include "maq1.c"
  #include "maq2.c"
  #include "maq3.c"
  #include "general.c"
#endif

int main() {
	// variable de estado
	int estado;
	// Las 2 lineas del display de 16x2
	char lin1[16], lin2[16];
	// para leer el teclado
	char boton;
	
/*  INICIO
se supone que todo esto va en una funcion
*/
  boton = 0;
  estado = 0;
  strcpy(lin1, "Bienvenido");
  lin2[0] = '\0';
  
/* Aca deberia ser while (1) pero le pongo
una condicion de salida para que no se 
cuelgue el programa y podamos hacer pruebas
*/
	while (boton != 'x') {
		switch (estado & 0x7) {
			case rep:
			  estado = reposo(estado, boton);
			  break;
		 case ini:
			  estado = iniciado(estado, boton);
			  break;
	  case dob:
			  estado = doblando(estado, boton);
			  break;
			case api:
			  estado = apilando(estado, boton);
			  break;
	  case con: 
			  estado = configurando(estado, boton);
			  break;
		}
		
		// Imprimir la simulacion
		system("clear");
		printf("+------------------+\n");
		printf("| %-16s |\n| %-16s |\n", lin1, lin2);
		printf("+------------------+\n\n");
		printf("=====================\n");
		printf("p: produccion\n"
		       "m: motor\n"
		       "e: parada de emergencia\n"
		       "f: funciones\n"
		       "o: ok\n"
		       "w: arriba\n"
		       "a: izquierda\n"
		       "s: abajo\n"
		       "d: derecha\n");
		printf("\n===============\n");
		boton = getchar();
	} // fin del while(1)
	return 0;
}
