// maq1.c

/*==========================================================================*/
// estado = rep1
estado_t reposo_1(ARGS_E) 
{
 	estado_t respuesta = rep1; // Valor por defecto
 	switch (bus[bus_user]) 
 	{
 	  case 'e': // Boton: Parada de emergencia.
      emergencia(bus);
      break;
    case 'm': // Boton: Motor principal.
      bus[bus_user] = 0;
      bus[bus_mp] = 1; // Encender motor principal
      bus[bus_mc] = 10; // Encender motor cinta (velocidad normal)
      respuesta = ini; // Siguiente estado
      break;
 	}
 	return respuesta;
}

/*==========================================================================*/
// estado = ini
estado_t iniciado(ARGS_E)
{
 	estado_t respuesta = ini; // Valor por defecto
 	switch (bus[bus_user])
 	{
    case 'e': // Boton: Parada de emergencia.
    case 'm': // Boton: Motor principal.
      emergencia(bus);
      respuesta = rep1; // Siguiente estado
      break;
    case 'p': // Boton: Produccion.
      if (bus[bus_mp])
      {
        bus[bus_va] = 1; // Abrir valvula de aire
        respuesta = dob; // Siguiente estado
      }
      break;
	 }
	return respuesta;
}

/*==========================================================================*/
// estado = dob
estado_t doblando(ARGS_E)
{
  estado_t respuesta = dob; // Valor por defecto
  switch (bus[bus_user])
 	{
    case 'e': // Boton: Parada de emergencia.
    case 'm': // Boton: Motor principal.
      emergencia(bus);
      respuesta = rep1; // Siguiente estado
      break;
    case 'p': // Boton: Produccion.
      bus[bus_va] = 0; // Cerrar valvula de aire
      respuesta = ini; // Siguiente estado
      break;
	 }
	 if (bus[bus_sc] && !bus[bus_pre_sc]) // Un papel acaba fe ingresar a la maquina
	 {
	   bus[bus_con]++; // Aumentar contador
	 }
	 bus[bus_pre_sc] = bus[bus_sc]; // Para no contar dos veces el mismo papel
	 if (bus[bus_con] >= bus[bus_lote]) // Se alcanzo la cantidad pedida
	 {
	   bus[bus_va] = 0; // Cerrar valvula de aire
	   respuesta = api; // Siguente estado
	   bus[bus_timer2] = 25; // Esperar que terminen de salir los papeles
	   set_msg(bus, 2, esp); // Avisar del evento a la maq. 2
	 }
  return respuesta;
}

/*==========================================================================*/
// estado = api
estado_t apilando(ARGS_E)
{
  estado_t respuesta = api; // Valor por defecto
  switch (bus[bus_user])
 	{
    case 'e': // Boton: Parada de emergencia.
    case 'm':
      emergencia(bus);
      break;
	 }
	 //if (e[1] == rep2 && check_timer(bus[bus_timer1])) // ! CONFLICTO !
	 if (get_msg(bus, 1) == dob)
	 {
	   bus[bus_va] = 1; // Abrir valvula de aire
	   bus[bus_con] = 0; // Reiniciar contador
	   respuesta = dob; // Siguiente estado
	   
	 }
  return respuesta;
}