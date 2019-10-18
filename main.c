//main.c

#include "def.h"

#ifdef no_compila_varios_punto_c
  #include "general.c"
  #include "maq1.c"
  #include "maq2.c"
  #include "maq3.c"
#endif

int main()
{ 
  int estado[CANT_MAQ]; // Variables de estado, una para cada maquina.
  char lin1[16], lin2[16]; // Las 2 lineas del display de 16x2.	
  int bus[BUS_ELEMENTOS]; // Para leer el teclado y simular la maquina.
	                         // bus[bus_user] es la ultuma tecla presionada.
	 // Para hacer depuracion usamos un archivo de salida
	 
  // Vector de cadenas de caractetes auxiliar para la simulacion:
  char desc[][20] = {"motor principal", "motor cinta", "motor apilador",
		                   "valvula de aire", "contador", "loteo",
		                   "timer 1", "timer 2", "timer 3",
		                   "sensor contador", "fin de carrera i", "fin de carrera d" };
  // Estas variable ayuda a controlar el flujo de la simulacion
  char pausado = 0;
  clock_t aux_pausa;
  
  // Para manejar los tiempos
  DEFINIR_CRONOMETRO(imp); // Este cronometro controla la impresion de la simulacion
  DEFINIR_CRONOMETRO(maq); // Este cronometro decrementa los temporizadores de la maquina (timer(x))
  DEFINIR_CRONOMETRO(sim); // Este temporizador lleva la cuenta del tiempo transcurrido desde el inicio de la simulacion
  DEFINIR_CRONOMETRO(pausa);
  DEFINIR_CRONOMETRO(sfc);
  // Iniciar los cronometros
  REINICIAR_CRONOMETRO(imp);
  REINICIAR_CRONOMETRO(maq);
  REINICIAR_CRONOMETRO(sim);
	 
/*  INICIO [init()]- se supone que todo esto va en una funcion */
  for (int i=0; i<BUS_ELEMENTOS; i++) bus[i] = 0;
  for (int i=0; i < CANT_MAQ; i++) estado[i] = 0;
  strcpy(lin1, "Bienvenido");
  lin2[0] = '\0';
  bus[bus_lote] = 8;
  bus[bus_sfd] = 1;
  
/* Aca deberia ser while (1) pero le pongo una condicion de salida para que no se 
    cuelgue el programa y podamos hacer depuracion.
       x: Finaliza la simulacion.
       y: Pausa la simulacion y entra en modo paso a paso.
       z: Reanuda la simulacion.
*/
  while (bus[bus_user] != 'z')
  {
    // Maquina de estado 1
    switch (estado[0])
    {
      case rep1:
        estado[0] = reposo_1(estado[0], bus);
        break;
      case ini:
        estado[0] = iniciado(estado[0], bus);
        break;
      case dob:
        estado[0] = doblando(estado[0], bus);
        break;
      case api:
        estado[0] = apilando(estado[0], bus);
        break;
      case con: 
        estado[0] = configurando(estado[0], bus);
        break;
    }
		
    // Maquina de estado 2
    switch (estado[1])
    {
      case rep2:
        estado[1] = reposo_2(estado[1], bus);
        break;
      case esp:
        estado[1] = esperando_fin_lote(estado[1], bus);
        break;
      case cin:
        estado[1] = cinta_acelerada(estado[1], bus);
        break;
      case mov_i:
      case mov_d:
        estado[1] = moviendo_apilador(estado[1], bus);
        break;
    }
		
    // Maquina de estado 3
    switch (estado[2]) {
      case rep3:
        estado[2] = reposo_3(estado[2], bus);
        break;
      case lote0:
      case lote1:
      case lote2:
      case lote3:
      case lote4:
        estado[2] = mod_lote(estado[2], bus);
        break;
      case t0:
      case t1:
      case t2:
        estado[2] = mod_tiempo(estado[2], bus);
        break;
    }
		
    /* Imprimir la simulacion
         En cada iteracion borrar la pantalla y volver a imprimir.
         lo importante es el display de 16x2, lo demas
         es para depuracion.
    */

    if (CRONOMETRO(imp) > T_IMP || pausado) // para no saturar el terminal, imprimir cada cierto tiempo de espera
    {
      REINICIAR_CRONOMETRO(imp);
      system(BORRAR_PANTALLA);

      // seccion 1: display 16x2
      printf("+------------------+\n");
      printf("| %-16s |\n| %-16s |\n", lin1, lin2);
      printf("+------------------+\n\n");

      // seccion 2: botones
      printf("=== Botones ================================\n");
      printf("p: produccion      e: parada de emergencia\n"
             "m: motor           f: funciones\n"
             "o: ok              w: arriba\n"
             "a: izquierda       s: abajo\n"
             "d: derecha\n");

      // seccion 3: estado de la maquina
      printf("\n=== Estado de la maquina ========================\n-");
      for (int i=0; i < CANT_MAQ; i++) printf("  estado %d: %d  -", i+1, estado[i]);
      for (int i=1; i<=bus_sfd; i+=2)
      {
        printf("\n%18s: %3d", desc[i-1], bus[i]);
        if (i+1<=bus_sfd) printf(" | %-3d :%-18s", bus[i+1], desc[i]);
      }
      printf("\n\n");
      if (pausado) printf("  Simulacion pausada");
      else printf("Tiempo de simulacion");
      float seg = CRONOMETRO(sim) - ((pausado)? CRONOMETRO(pausa) : 0);
      printf(": %d:%05.2f   > Teclado > ", (int)seg/60, (((int)seg) % 60)+(seg - floor(seg))); 
      if (bus[bus_user]) putchar(bus[bus_user]);
      /* La impresion era defectuosa, habia un conflicto entre system("clear")
         y los printf(), asi que agregue la siguiente linea para asegurar que la
         impresion sea correcta. */
      fflush(stdout);
    } // Fin impresion de datos

/*   Simular maquina en funcionamento   */
    // Simular papeles pasando por debajo del sensor contador
    if(bus[bus_mp] && bus[bus_va]) bus[bus_sc] = (CRONOMETRO(sim) - floor(CRONOMETRO(sim)) > 0.3)? 1 : 0; 
    else bus[bus_sc] = 0;
  
    // Actualizar los temporizadores
    if (CRONOMETRO(maq) >= 0.1) // El incremento es en decimas de segundo
    {
      for (int i=bus_timer1; i<=bus_timer3; i++) if (bus[i]) bus[i]--;
      REINICIAR_CRONOMETRO(maq);
    }

    // Sensores de fin de carrera
    if (bus[bus_ma]!=0) // si (motor apilador ON)
    {
     if ((bus[bus_sfi]==1) || (bus[bus_sfd]==1))
     {
       //if (CRONOMETRO(sfc) < 3.5)
       {
         REINICIAR_CRONOMETRO(sfc);
         if (bus[bus_ma]>0) bus[bus_sfi] = 0;
         if (bus[bus_ma]<0) bus[bus_sfd] = 0;
       }
     }
     else
     {
       if (CRONOMETRO(sfc) > 4.5)
       {
         if (bus[bus_ma] < 0) bus[bus_sfi] = 1;
         if (bus[bus_ma] > 0) bus[bus_sfd] = 1;
       }
     }
    }
    else if (!(bus[bus_sfi] | bus[bus_sfd])) // si (motor apilador OFF)
    {
      printf("\n\n>>> Error: Sensores fin de carrera @ %s: %d", __FILE__, __LINE__);
      exit(0);
    }
    
/*   Leer el teclado   */
    if (kbhit()) bus[bus_user] = getchar();
    else bus[bus_user] = 0;
    
    // Evaluar el modo de simulacion: ( pausado | normal )
    if (pausado)
  	{
  	 	while (!kbhit()); // Esperar a que se presione una tecla
  	  bus[bus_user] = getchar();
  	  if (bus[bus_user] == 'c') // salir del modo paso a paso
  	  {
  	  	pausado = 0;
  	  	bus[bus_user] = 0;
  	  	aux_clk_sim += clock() - aux_clk_pausa;
  	  }
  	  aux_clk_sim -= (CLOCKS_PER_SEC/10); // Incrementar una decima de segundo en cada iteracion
  	}
  	if (!pausado && bus[bus_user]=='x') // Entrar en modo paso a paso
  	{
  		pausado = 1;
  		REINICIAR_CRONOMETRO(pausa);
  	}
  } // fin del while(1)
  
  printf("\n\nSimulacion finalizada.");
  return 0;
}  // Fin del main()
