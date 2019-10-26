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
  int i;
  int estado[CANT_MAQ]; // Variables de estado, una para cada maquina.
  char *display16x2[2]; // Las 2 lineas del display de 16x2.	
  int bus[BUS_TAMANIO]; // Para leer el teclado y simular la maquina.
	                        // bus[bus_user] es la ultuma tecla presionada.

	 
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
  DEFINIR_CRONOMETRO(pap);
  // Iniciar los cronometros
  REINICIAR_CRONOMETRO(imp);
  REINICIAR_CRONOMETRO(maq);
  REINICIAR_CRONOMETRO(sim);
	 
/*  INICIO 'init()' - se supone que todo esto va en una funcion */
  for (i=0; i<BUS_TAMANIO; i++) bus[i] = 0;
  for (i=0; i < CANT_MAQ; i++) estado[i] = 0;
  display16x2[0] = (char*)&bus[bus_disp1];
  display16x2[1] = (char*)&bus[bus_disp2];
  bus[bus_lote] = 50;
  bus[bus_sfd] = 1;
  bus[bus_t_esp] = 35;
  bus[bus_vel] = 10;
  bus[bus_UI] = UI_reposo_cant;
  print16x2(bus, D16x2_LIN_1, "Parcial");
    // Imprimir las partes de la simulacion que son constantes
  // seccion 1: display 16x2
  printf("+------------------+\n\n\n");
  printf("+------------------+\n\n");

  // seccion 2: botones
  printf("=== Botones ======================================\n");
  printf("m: motor principal         e: parada de emergencia\n"
         "p: produccion\n"
         "a: izquierda   w: arriba   o: ok\n"
         "d: derecha     s: abajo    f: funciones\n");
         
  // seccion 3: estado de la maquina
  printf("\n=== Estado de la maquina ========================\n-");

/* Aca deberia ser while (1) pero le pongo una condicion de salida para que no se 
    cuelgue el programa y podamos hacer depuracion.
       z: Finaliza la simulacion.
       x: Pausa la simulacion y entra en modo paso a paso.
       c: Reanuda la simulacion.
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
      case vel:
        estado[2] = mod_velocidad(estado[2], bus);
        break;
      case loteo:
        estado[2] = mod_lote(estado[2], bus);
        break;
      case t_esp:
        estado[2] = mod_tiempo(estado[2], bus);
        break;
    }
		
    /* Imprimir la simulacion
         En cada iteracion borrar la pantalla y volver a imprimir.
         lo importante es el display de 16x2, lo demas es para depuracion.
    */
    if (CRONOMETRO(imp) > T_IMP || pausado) // para no saturar el terminal, imprimir cada cierto tiempo de espera
    {
      REINICIAR_CRONOMETRO(imp);
      // seccion 1: display 16x2
      gotoxy(0, 2);
      printf("| %16s |\n| %16s |", display16x2[0], display16x2[1]);

      // seccion 3: Estado de la maquina
      gotoxy(2, 13);
      for (i=0; i < CANT_MAQ; i++) printf("  estado %d: %d  -", i+1, estado[i]);
      for (i=1; i<=bus_sfd; i+=2)
      {
        printf("\n%17s: %5d", desc[i-1], bus[i]);
        if (i+1<=bus_sfd) printf(" | %-5d :%-17s", bus[i+1], desc[i]);
      }
      
      gotoxy(0, 21);
      if (pausado) printf("  Simulacion pausada");
      else printf("Tiempo de simulacion");
      float seg = CRONOMETRO(sim) - ((pausado)? CRONOMETRO(pausa) : 0);
      printf(": %d:%05.2f  ", (int)seg/60, (((int)seg) % 60)+(seg - floor(seg))); 
      
      /* La impresion era defectuosa, habia un conflicto entre system("clear") y los printf(),
      asi que agregue la siguiente linea para asegurar que la impresion sea correcta. */
      fflush(stdout);
    } // Fin impresion de datos

    /* Simular maquina en funcionamento */

    // Simular papeles pasando por debajo del sensor contador
    if(bus[bus_mp] && bus[bus_va])
    {
      if (CRONOMETRO(pap) > 1.0 / bus[bus_vel])
      {
        REINICIAR_CRONOMETRO(pap);
        bus[bus_sc] = !bus[bus_sc];
      }
    } 
    else bus[bus_sc] = 0;
    
    // Actualizar los temporizadores
    if (CRONOMETRO(maq) >= 0.1) // El decremento en general es en decimas de segundo
    {
      for (int i=bus_timer1; i<=bus_timer3; i++) if (bus[i]) bus[i]--;
      REINICIAR_CRONOMETRO(maq);
    }

    // Actualizar sensores de fin de carrera
    if (bus[bus_ma]!=0) // si (motor apilador ON)
    {
     if ((bus[bus_sfi]==1) || (bus[bus_sfd]==1)) // si (algun sensor ON)
     {
       REINICIAR_CRONOMETRO(sfc);
       if (bus[bus_ma]>0) bus[bus_sfi] = 0;
       if (bus[bus_ma]<0) bus[bus_sfd] = 0;
     }
     else // si (ambos sensores OFF)
     {
       if (CRONOMETRO(sfc) > 4.5)
       {
         if (bus[bus_ma] < 0) bus[bus_sfi] = 1;
         if (bus[bus_ma] > 0) bus[bus_sfd] = 1;
       }
     }
    }
    else if (!(bus[bus_sfi] | bus[bus_sfd])) // si (motor apilador OFF) y (ambos sensores OFF)
    {
      printf("\n\n>>> Error: Sensores fin de carrera @line: %d", __LINE__);
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
