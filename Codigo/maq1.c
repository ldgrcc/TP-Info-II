// maq1.c
#include "def.h"
/*==========================================================================*/
// estado = rep1
estado_t reposo_1(ARGS_E) 
{
 	switch (bus[bus_user]) 
 	{
 	  case 'e': // Boton: Parada de emergencia.
      emergencia(bus);
      break;
    case 'm': // Boton: Motor principal.
      if (!(bus[bus_sfd] | bus[bus_sfi])) // Berificar que el apilador este en la posicion correcta
      {
        bus[bus_ma] = -1;
        return rep1;
      }
      bus[bus_user] = 0;
      bus[bus_mp] = 1; // Encender motor principal
      bus[bus_mc] = 10; // Encender motor cinta (velocidad normal)
      return ini; // Siguiente estado
      break;
 	}
 	return rep1;
}

/*==========================================================================*/
// estado = ini
estado_t iniciado(ARGS_E)
{
 	switch (bus[bus_user])
 	{
    case 'e': // Boton: Parada de emergencia.
    case 'm': // Boton: Motor principal.
      emergencia(bus);
      return rep1; // Siguiente estado
      break;
    case 'p': // Boton: Produccion.
      if (bus[bus_mp])
      {
        bus[bus_va] = 1; // Abrir valvula de aire
        return dob; // Siguiente estado
      }
      break;
	 }
	return ini; //spuesta;
}

/*==========================================================================*/
// estado = dob
estado_t doblando(ARGS_E)
{
  switch (bus[bus_user])
 	{
    case 'e': // Boton: Parada de emergencia.
    case 'm': // Boton: Motor principal.
      emergencia(bus);
      return rep1; // Siguiente estado
      break;
    case 'p': // Boton: Produccion.
      bus[bus_va] = 0; // Cerrar valvula de aire
      return ini; // Siguiente estado
      break;
	 }
	 if (bus[bus_sc] && !bus[bus_pre_sc]) // Un papel acaba fe ingresar a la maquina
	 {
	   if (bus[bus_lote]) bus[bus_con]++; // Aumentar contador
	   bus[bus_total]++;
	 }
	 bus[bus_pre_sc] = bus[bus_sc]; // Para no contar dos veces el mismo papel
	 if (bus[bus_con] >= bus[bus_lote]) // Se alcanzo la cantidad pedida
	 {
	   bus[bus_va] = 0; // Cerrar valvula de aire
	   bus[bus_timer2] = 25; // Esperar que terminen de salir los papeles
	   set_msg(bus, 2, esp); // Avisar del evento a la maq. 2
	   return api; // Siguente estado
	 }
  return dob;
}

/*==========================================================================*/
// estado = api
estado_t apilando(ARGS_E)
{
  int msg;
  switch (bus[bus_user])
 	{
    case 'e': // Boton: Parada de emergencia.
    case 'm':
      emergencia(bus);
      bus[bus_con] = 0;
      while (get_msg(bus, 1));
      return rep1;
      break;
	 }
	 while (msg = get_msg(bus, 1))
	 {
	   {
	     bus[bus_con] = 0; // Reiniciar contador
	     if (bus[bus_mp] > 0)
	     {
	       bus[bus_va] = 1; // Abrir valvula de aire
	       return dob; // Siguiente estado
	     }
	     else
	     {
	      	bus[bus_mc] = 0;
	       return rep1; // Siguiente estado
	     }
	   }
	 }
  return api;
}