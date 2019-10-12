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

#define c

int main() {
	// variable de estado
	int estado;
	// Las 2 lineas del display de 16x2
	char lin1[16], lin2[16];
	// para leer el teclado y simular la maquina
	//bus[0] es la ultuma tecla presionada
	int bus[21];
	char desc[][20] = {"motor principal", "motor cinta", "motor apilador", "contador", "loteo"};
/*  INICIO
se supone que todo esto va en una funcion
*/
  for (int i=0; i<21; i++) bus[i] = 0;
  strcpy(lin1, "Bienvenido");
  lin2[0] = '\0';
  
/* Aca deberia ser while (1) pero le pongo
una condicion de salida para que no se 
cuelgue el programa y podamos hacer pruebas
*/
	while (bus[bus_user] != 'x') {
		// Maquina de estado 1
		switch (get_e(estado, 1)) {
			case rep1:
			  estado = reposo_1(estado, bus);
			  break;
		 case ini:
			  estado = iniciado(estado, bus);
			  break;
	  case dob:
			  estado = doblando(estado, bus);
			  break;
			case api:
			  estado = apilando(estado, bus);
			  break;
	  case con: 
			  estado = configurando(estado, bus);
			  break;
		}
		
		// Maquina de estado 2
		switch (get_e(estado, 2)) {
			case rep2:
			  estado = reposo_2(estado, bus);
			  break;
			case esp:
			  estado = esperando_fin_lote(estado, bus);
			  break;
			case cin:
			  estado = cinta_acelerada(estado, bus);
			  break;
			case mov_i:
			case mov_d:
			  estado = moviendo_apilador(estado, bus);
			  break;
		}
		
		// Maquina de estado 3
		switch (get_e(estado, 3)) {
			case rep3:
			  estado = reposo_3(estado, bus);
			  break;
			case lote0:
			case lote1:
			case lote2:
			case lote3:
			case lote4:
			  
			  break;
			case t0:
			case t1:
			case t2:
			  
			  break;
		}
// actualizar el estado de la maquina segun el bus de mensajes
		// Imprimir la simulacion
/* en cada iteracion borrar la pantalla y volver a imprimir.
lo importante es el display de 16x2, lo demas
es para depuracion */
		system("clear");
		// seccion 1: display 16x2
		printf("+------------------+\n");
		printf("| %-16s |\n| %-16s |\n", lin1, lin2);
		printf("+------------------+\n\n");
		// seccion 2: botones
		printf("= Botones ====================\n");
		printf("p: produccion\n"
		       "m: motor\n"
		       "e: parada de emergencia\n"
		       "f: funciones\n"
		       "o: ok\n"
		       "w: arriba\n"
		       "a: izquierda\n"
		       "s: abajo\n"
		       "d: derecha\n");
// seccion 3: estado de la maquina
		printf("\n=============================\n");
		for (int i=1; i<=bus_lote; i++) printf("%20s: %d\n", desc[i-1], bus[i]);
// Simular maquina en funcionamento
// Aumentar contador si la maquina esta produciendo
  if(bus[bus_mp] == 1 && bus[bus_va]) !bus[bus_sc]; 
  else bus[bus_sc] = 0;
  
// Leer el teclado
		bus[0] = getchar();
		
	} // fin del while(1)
	return 0;
}
