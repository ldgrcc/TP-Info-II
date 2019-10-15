// maq1.c

// estado = rep1
estado_t reposo_1(ARGS_E) 
{
 	estado_t respuesta = e[0];
 	switch (bus[bus_user]) 
 	{
 	  case 'e': // Boton: Parada de emergencia.
      bus[bus_mp] = 0;
      bus[bus_mc] = 0;
      bus[bus_va] = 0;
      break;
    case 'm': // Boton: Motor principal.
      bus[bus_mp] = 1;
      bus[bus_mc] = 10;
      respuesta = ini;
      break;
 	}
 	return respuesta;
}

// estado = ini
estado_t iniciado(ARGS_E)
{
 	estado_t respuesta = e[0];
 	switch (bus[bus_user])
 	{
    case 'e': // Boton: Parada de emergencia.
    case 'm': // Boton: Motor principal.
      bus[bus_mp] = 0;
      bus[bus_mc] = 0;
      bus[bus_va] = 0;
      respuesta = rep1;
      break;
    case 'p': // Boton: Produccion.
      if (bus[bus_mp])
      {
        bus[bus_va] = 1;
        respuesta = dob;
      }
      break;
	 }
	return respuesta;
}

// estado = dob
estado_t doblando(ARGS_E)
{
  estado_t respuesta = e[0];
  switch (bus[bus_user])
 	{
    case 'e': // Boton: Parada de emergencia.
    case 'm': // Boton: Motor principal.
      bus[bus_mp] = 0;
      bus[bus_mc] = 0;
      bus[bus_va] = 0;
      respuesta = rep1;
      break;
    case 'p': // Boton: Produccion.
      bus[bus_va] = 0;
      respuesta = ini;
      break;
	 }
	 if (bus[bus_sc] && !bus[bus_pre_sc])
	 {
	   bus[bus_con]++;
	 }
	 bus[bus_pre_sc] = bus[bus_sc];
	 if (bus[bus_con] >= bus[bus_lote]) 
	 {
	   bus[bus_va] = 0;
	   respuesta = api;
	   bus[bus_timer2] = 25;
	 }
  return respuesta;
}

// estado = api
estado_t apilando(ARGS_E)
{
  estado_t respuesta = e[0];
  switch (bus[bus_user])
 	{
    case 'e': // Boton: Parada de emergencia.
      bus[bus_mp] = 0;
      bus[bus_mc] = 0;
      bus[bus_va] = 0;
      break;
	 }
  return respuesta;
}

// estado = con
estado_t configurando(ARGS_E)
{
  estado_t respuesta = e[0];
  switch (bus[bus_user])
 	{
    case 'e': // Boton: Parada de emergencia.
      bus[bus_mp] = 0;
      bus[bus_mc] = 0;
      bus[bus_va] = 0;
      break;
	 }
  return respuesta;
}