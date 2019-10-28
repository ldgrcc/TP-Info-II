// maq2.c
#include "def.h"
/*==========================================================================*/
// estado = rep2
estado_t reposo_2(ARGS_E)
{
  estado_t resp = rep2;
  if ((bus[bus_sfd] | bus[bus_sfi])) bus[bus_ma] = 0;
  switch (bus[bus_user])
  {
    case 'e': // Boton: Parada de emergencia.
      emergencia(bus);
    case 'm':
      while (get_msg(bus, 2));
      break;
  }
  if (get_msg(bus, 2) == esp) // maquina 1 en estado 'apilando'
  {
    resp = esp;
  }
  return resp;
}

/*==========================================================================*/
// estado = esp
estado_t esperando_fin_lote(ARGS_E)
{
  estado_t resp = esp; // Valor por defecto
  switch (bus[bus_user])
  {
    case 'e': // Boton: Parada de emergencia.
      emergencia(bus);
      return rep2;
      break;
  }
  if (check_timer(bus[bus_timer2]))
  {
    bus[bus_mc] = 50; // Aumentar velocidad de la cinta
    bus[bus_timer2] = bus[bus_t_esp]; // Esperar que los papeles terminen de pasar
    resp = cin; // Pasar al siguiente estado
    print_info("cinta acelerada");
  }
  return resp;
}

/*==========================================================================*/
// estado = cin
estado_t cinta_acelerada(ARGS_E)
{
  estado_t resp = cin; // Valor por defecto
  switch (bus[bus_user])
  {
    case 'e': // Boton: Parada de emergencia.
      emergencia(bus);
      return rep2;
      break;
  }
  if (check_timer(bus[bus_timer2])) // Fin tiempo de eapera
  {
    bus[bus_mc] = 10; // Desacelerar la cinta
    resp = ((bus[bus_sfd])? mov_i : mov_d); // Determinar el siguiente estado
    bus[bus_ma] = (bus[bus_sfi] - bus[bus_sfd]); // Poner en marcha el motor apilador en la direccion correcta
  }
  return resp;
}

/*==========================================================================*/
// estado = mov_i || mov_d
estado_t moviendo_apilador(ARGS_E)
{
  estado_t resp = e;
  switch (bus[bus_user])
  {
    case 'e': // Boton: Parada de emergencia.
      emergencia(bus);
      return rep2;
      break;
  }
  if (((e == mov_d) && bus[bus_sfd]==1)
   || ((e == mov_i) && bus[bus_sfi]==1)) // Se activo un sensor de fin de carrera
  {
    bus[bus_ma] = 0; // Apagar el motor apilador
    set_msg(bus, 1, dob); // Avisar a la maq. de est. 1
    return rep2; // Pasar al siguiente estado
  }
  return e;
}