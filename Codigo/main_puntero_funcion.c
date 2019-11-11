//main.c

#include "def.h"

#ifdef SIS_OP_WIN

int main()
{ 
  int i;
  char terminal[23][51];
  for (i=0; i<23; i++) strcpy(terminal[i], "");
  int estado[CANT_MAQ]; // Variables de estado, una para cada maquina.
  char *display16x2[2]; // Las 2 lineas del display de 16x2.	
  int bus[BUS_TAMANIO]; // Para leer el teclado y simular la maquina.
	                        // bus[bus_user] es la ultuma tecla presionada.
  
	 estado_t (*maq[3][5])(estado_t*, int*);
	 
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
  DEFINIR_CRONOMETRO(t3);
  // Iniciar los cronometros
  REINICIAR_CRONOMETRO(imp);
  REINICIAR_CRONOMETRO(maq);
  REINICIAR_CRONOMETRO(sim);
  REINICIAR_CRONOMETRO(t3);
	 
	 display16x2[0] = (char*)&bus[bus_disp1];
  display16x2[1] = (char*)&bus[bus_disp2];
  
/*  INICIO 'init()' */
  init(bus, estado);
  
  // Imprimir las partes de la simulacion que son constantes
  print16x2(bus, D16x2_LIN_1, "Parcial");
  
  // seccion 1: display 16x2
  sprintf(terminal[1], "+------------------+");
  sprintf(terminal[3], "+------------------+");

  // seccion 2: botones
  sprintf(terminal[5], "=== Botones ======================================");
  sprintf(terminal[6], "m: motor principal         e: parada de emergencia");
  sprintf(terminal[7], "p: produccion");
  sprintf(terminal[8], "a: izquierda   w: arriba   o: ok");
  sprintf(terminal[9], "d: derecha     s: abajo    f: funciones");
         
  // seccion 3: estado de la maquina
  sprintf(terminal[11], "=== Estado de la maquina =========================");
  sprintf(terminal[12], "-");
/* Aca deberia ser while (1) pero le pongo una condicion de salida para que no se 
    cuelgue el programa y podamos hacer depuracion.
       z: Finaliza la simulacion.
       x: Pausa la simulacion y entra en modo paso a paso.
       c: Reanuda la simulacion.
*/
  maq[0][0] = reposo_1;
  maq[0][1] = iniciado;
  maq[0][2] = doblando;
  maq[0][3] = apilando;
  
  maq[1][0] = reposo_2;
  maq[1][1] = esperando_fin_lote;
  maq[1][2] = cinta_acelerada;
  maq[1][3] = moviendo_apilador;
  maq[1][4] = moviendo_apilador;
  
  maq[2][0] = reposo_3;
  maq[2][1] = mod_velocidad;
  maq[2][2] = mod_lote;
  maq[2][3] = mod_tiempo;
  
  while (bus[bus_user] != 'z') /* while (1) */
  {
    for (i=0; i<3; i++) estado[i] = maq[i][estado[i]](estado[i], bus);
		
    /* Imprimir la simulacion
         En cada iteracion borrar la pantalla y volver a imprimir.
         lo importante es el display de 16x2, lo demas es para depuracion.
    */
    if (CRONOMETRO(imp) > T_IMP || pausado) // para no saturar el terminal, imprimir cada cierto tiempo de espera
    {
      REINICIAR_CRONOMETRO(imp);
      // seccion 1: display 16x2
      //gotoxy(0, 2);
      sprintf(terminal[2], "| %16s |\n| %16s |", display16x2[0], display16x2[1]);

      // seccion 3: Estado de la maquina
      //gotoxy(2, 13);
      for (i=0; i < CANT_MAQ; i++) sprintf(&terminal[12][1+(i*16)], "  estado %2d:%2d  -", i+1, estado[i]);
      for (i=1; i<=bus_sfd; i+=2)
      {
        sprintf(terminal[13+(i/2)], "%17s: %4d", desc[i-1], bus[i]);
        if (i+1<=bus_sfd) sprintf(&terminal[13+(i/2)][23], " | %-4d :%-17s", bus[i+1], desc[i]);
      }
      
      //gotoxy(0, 21);
      if (pausado) sprintf(terminal[20], "  Simulacion pausada");
      else sprintf(terminal[20], "Tiempo de simulacion");
      float seg = CRONOMETRO(sim) - ((pausado)? CRONOMETRO(pausa) : 0);
      sprintf(&terminal[20][19], ": %d:%04.1f  ", (int)seg/60, (((int)seg) % 60)+(seg - floor(seg))); 
      
      /* La impresion era defectuosa, habia un conflicto entre system("clear") y los printf(),
      asi que agregue la siguiente linea para asegurar que la impresion sea correcta. */
      system(BORRAR_PANTALLA);
      for (i=1; i<21; i++) printf("%-48s\n", terminal[i]);
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
      //for (i=bus_timer1; i<=bus_timer2; i++) if (bus[i]) bus[i]--;
      if (bus[bus_timer1]) bus[bus_timer1]--;
      if (bus[bus_timer2]) bus[bus_timer2]--;
      REINICIAR_CRONOMETRO(maq);
    }
    if (CRONOMETRO(t3) >= 0.001) // El decremento del timer3 es en milecimas de segundo
    {
      if (bus[bus_timer3]) bus[bus_timer3]--;
      REINICIAR_CRONOMETRO(t3);
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
       if (CRONOMETRO(sfc) > 5.5)
       {
         if (bus[bus_ma] < 0) bus[bus_sfi] = 1;
         if (bus[bus_ma] > 0) bus[bus_sfd] = 1;
       }
     }
    }
    else if (!(bus[bus_sfi] | bus[bus_sfd])) // si (motor apilador OFF) y (ambos sensores OFF)
    {
      REINICIAR_CRONOMETRO(sfc);
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


#ifdef no_compila_varios_punto_c
  #include "general.c"
  #include "maq1.c"
  #include "maq2.c"
  #include "maq3.c"
#endif

#endif