// maq3.c

#include "def.h"

/*==========================================================================*/
// estado = rep3
estado_t reposo_3(ARGS_E)
{
  estado_t retval = rep3;
  char auxstr[17];
  int aux_e = bus[bus_UI];
  
  /* Atender teclas pulsadas */
  if ((bus[bus_UI] & UI_reposo_borrar) && bus[bus_user]!='o')
  {
    bus[bus_UI] &= -1>>1;
    return rep3;
  }
  switch (bus[bus_user])
  {
    case 'w':
      if (bus[bus_UI] > UI_reposo_cant) bus[bus_UI]--;
      break;
    case 's':
      if (bus[bus_UI] < UI_reposo_total) bus[bus_UI]++;
      break;
    case 'f':
      bus[bus_UI] = UI_menu;
      print16x2(bus, D16x2_LIN_CMD|D16x2_BRR_ALL, 0);
      //print16x2(bus, D16x2_LIN_1|D16x2_ALIN_IZQ, "Loteo");
      //sprintf(auxstr, "%d", bus[bus_lote]);
      print16x2(bus, D16x2_LIN_1|D16x2_ALIN_IZQ, "Velocidad Motor");
      sprintf(auxstr, "%d", bus[bus_vel]);
      print16x2(bus, D16x2_LIN_2|D16x2_ALIN_DER, auxstr);
      //return loteo;
      return vel;
      break;
    case 'a':
      switch (bus[bus_UI])
      {
        case UI_reposo_cant:
        case UI_reposo_total:
          //printf("\n<< ui=%d  >>", bus[bus_UI]);
          bus[bus_UI] |= UI_reposo_borrar;
          print16x2(bus, D16x2_LIN_2|D16x2_ALIN_DER|D16x2_BRR_2, "Borrar?");
          //printf("<< ui=%d  >>\n", bus[bus_UI]);
          //system("pause");
          break;
      }
      break;
    case 'o':
      if (bus[bus_UI] & UI_reposo_borrar)
      {
        //printf("<< ui=%d  >>", bus[bus_UI]);
        //system("pause");
        switch (bus[bus_UI] & ~UI_reposo_borrar)
        {
          case UI_reposo_cant:
            bus[bus_con] = 0;
            break;
          case UI_reposo_total:
            bus[bus_total] = 0;
            break;
        }
        bus[bus_UI] &= ~UI_reposo_borrar;
      }
      break;
  }
  if (bus[bus_UI] & UI_reposo_borrar) return rep3;
  
  /* Imprimir cambios de estado */
  if (bus[bus_UI] != aux_e)
  {
    switch (bus[bus_UI])
    {
      case UI_reposo_cant:
        sprintf(auxstr, "Parcial");
        break;
      case UI_reposo_ratio:
        sprintf(auxstr, "Ratio (Pl/h)");
        break;
      case UI_reposo_total:
        sprintf(auxstr, "Total");
        break;
      default:
        return rep3;
        break;
    }
    print16x2(bus, D16x2_LIN_1|D16x2_ALIN_IZQ|D16x2_BRR_ALL, auxstr);
  }
  
  /* Imprimir datos variables */
  switch (bus[bus_UI])
  {
    case UI_reposo_cant:
      sprintf(auxstr, "%d", bus[bus_con]);
      print16x2(bus, D16x2_LIN_2|D16x2_ALIN_DER, auxstr);
      break;
    case UI_reposo_ratio:
      if (!bus[bus_timer3]) bus[bus_timer3] = 9999;
      if (bus[bus_mp])
      {
        int dif = bus[bus_total] - bus[bus_aux_ratio];
        
        if ((dif > 0) && (bus[bus_timer3] < 9999))
        {
          int plh, v, t;
          float n;
          t = 9999 - bus[bus_timer3];
          //if (bus[bus_aux_v]/t > 0.5) t = bus[bus_aux_v];
          //printf("<<  t1=%d  >>\n", t);
          if (!bus[bus_aux_v]) bus[bus_aux_v] = t;
          t /= dif;
          t = (t + bus[bus_aux_v]) / 2;
          //printf("<<  t2=%d  >>\n", t);
          bus[bus_aux_v] = t;
          v = 600/t;
          //printf("<<  v=%d  >>\n", v);
          if (bus[bus_lote] > 0)
          {
            n = (v * 60) / bus[bus_lote];
          //  printf("<<  n=%f  >>\n", n);
            plh = (60-((25+bus[bus_t_esp])*n/600))*v;
          //  printf("<<  plh(if)=%d  >>\n", plh);
          }
          else plh = 60 * v;
          //printf("<<  plh(pre redondeo)=%d  >>\n", plh);
          plh /= 100;
          plh *= 100;
          //printf("<<  plh=%d  >>\n", plh);
          //fflush(stdout);
          //system("pause");
          sprintf(auxstr, "%d", plh);
          bus[bus_timer3] = 9999;
          bus[bus_aux_ratio] = bus[bus_total];
          bus[bus_ratio] = plh;
        }
      }
      else bus[bus_ratio] = 0;
      
      sprintf(auxstr, "%d", bus[bus_ratio]);
      print16x2(bus, D16x2_LIN_2|D16x2_ALIN_DER, auxstr);
      break;
    case UI_reposo_total:
      sprintf(auxstr, "%d", bus[bus_total]);
      print16x2(bus, D16x2_LIN_2|D16x2_ALIN_DER, auxstr);
      break;
    default:
      return rep3;
      break;
  }
  return retval;
}

/*==========================================================================*/
// estado = vel
estado_t mod_velocidad(ARGS_E)
{
  char auxstr[17];
  
  /* Atender teclas pulsadas */
  switch (bus[bus_user])
  {
    case 's':
      print16x2(bus, D16x2_LIN_1|D16x2_ALIN_IZQ, "Loteo");
      return loteo;
      break;
    case 'a':
      bus[bus_vel]--;
      if (bus[bus_vel] < 1) bus[bus_vel] = 1;
      break;
    case 'd':
      bus[bus_vel]++;
      if (bus[bus_vel] > 20) bus[bus_vel] = 20;
      break;
    case 'f':
      bus[bus_UI] = UI_reposo_cant;
      print16x2(bus, D16x2_LIN_1|D16x2_BRR_ALL, "Parcial");
      return rep3;
      break;
  }
  
  /* Imprimir datos variables */
  sprintf(auxstr, "%d", bus[bus_vel]);
  print16x2(bus, D16x2_LIN_2|D16x2_ALIN_DER, auxstr);
  return vel;
}

/*==========================================================================*/
// estado = loteo
estado_t mod_lote(ARGS_E)
{
  estado_t retval = loteo;
  char auxstr[17];
  int aux_e = bus[bus_UI];
  int auxint=-1, dig[4], *p=NULL;
        
  /* Atender teclas pulsadas */
  if (bus[bus_UI] != UI_menu)
  {
    auxint = get_msg(bus, 3);
    dig[3] = (auxint / 1000) * 1000;
    dig[2] = ((auxint - dig[3]) / 100) * 100;
    dig[1] = ((auxint - dig[3] - dig[2]) / 10) * 10;
    dig[0] = auxint - dig[3] - dig[2] - dig[1];
    dig[3] /= 1000;
    dig[2] /= 100;
    dig[1] /= 10;
  }
  switch (bus[bus_user])
  {
    case 's':
      if (bus[bus_UI] != UI_menu)
      {
        p = &dig[bus[bus_UI]-1];
        if ((*p)>0) (*p)--;
      }
      else
      {
        print16x2(bus, D16x2_LIN_1|D16x2_ALIN_IZQ, "Pausa");
        return t_esp;
      }
      break;
    case 'w':
      if (bus[bus_UI] != UI_menu)
      {
        p = &dig[bus[bus_UI]-1];
        if ((*p)<9) (*p)++;
      }
      else
      {
        print16x2(bus, D16x2_LIN_1|D16x2_ALIN_IZQ, "Velocidad Motor");
        return vel;
      }
      break;
    case 'a':
      if (bus[bus_UI] != UI_menu)
      {
        bus[bus_UI]++;
        if (bus[bus_UI] > UI_lote_max_dig) bus[bus_UI] = UI_lote_max_dig;
      }
      break;
    case 'd':
      if (bus[bus_UI] != UI_menu)
      {
        bus[bus_UI]--;
        if (bus[bus_UI] < 1) bus[bus_UI] = 1;
      }
      break;
    case 'o':
      if (bus[bus_UI] == UI_menu)
      {
        bus[bus_UI] = UI_dig1;
        set_msg(bus, 3, bus[bus_lote]);
        print16x2(bus, D16x2_LIN_1|D16x2_ALIN_DER|D16x2_BRR_ALL, "Loteo          v");
        return loteo;
      }
      else
      {
        bus[bus_UI] = UI_menu;
        bus[bus_lote] = dig[0] + (dig[1]*10) + (dig[2]*100) + (dig[3]*1000);
        bus[bus_aux_v] = 0;
      }
      break;
    case 'f':
      retval = rep3;
      bus[bus_UI] = UI_reposo_cant;
      print16x2(bus, D16x2_LIN_1|D16x2_BRR_ALL, "Parcial");
      break;
  }
  if (auxint != -1)
  {
    int i;
    auxint = 0;
    for (i=0; i<UI_lote_max_dig; i++) auxint += dig[i]*pow(10,i);
    set_msg(bus, 3, auxint);
  }
  /* Imprimir cambios de estado */
  if (bus[bus_UI] != aux_e)
  {
    strcpy(auxstr, "Loteo       ");
    switch (bus[bus_UI])
    {
      case UI_dig1:
        strcat(auxstr, "   v");
        break;
      case UI_dig2:
        strcat(auxstr, "  v ");
        break;
      case UI_dig3:
        strcat(auxstr, " v  ");
        break;
      case UI_dig4:
        strcat(auxstr, "v   ");
        break;
      default:
        strcat(auxstr, "    ");
        break;
    }
    print16x2(bus, D16x2_LIN_1|D16x2_ALIN_DER|D16x2_BRR_ALL, auxstr);
  }
  /* Imprimir datos variables */
  switch (bus[bus_UI])
  {
    case UI_menu:
      sprintf(auxstr, "%d", bus[bus_lote]);
      print16x2(bus, D16x2_LIN_2|D16x2_ALIN_DER, auxstr);
      break;
    default:
      sprintf(auxstr, "%04d", auxint);
      print16x2(bus, D16x2_LIN_2|D16x2_ALIN_DER, auxstr);
      break;
  }
  return retval;
}

/*==========================================================================*/
// estado = t_esp
estado_t mod_tiempo(ARGS_E)
{
  estado_t retval = t_esp;
  char auxstr[17];
  int aux_e = bus[bus_UI];
  int auxint=-1, dig[2], *p=NULL;
        
  /* Atender teclas pulsadas */
  if (bus[bus_UI] != UI_menu)
  {
    auxint = get_msg(bus, 3);
    dig[1] = (auxint / 10) * 10;
    dig[0] = auxint - dig[1];
    dig[1] /= 10;
  }
  switch (bus[bus_user])
  {
    case 's':
      if (bus[bus_UI] != UI_menu)
      {
        p = &dig[bus[bus_UI]-1];
        if ((*p)>0) (*p)--;
      }
      break;
    case 'w':
      if (bus[bus_UI] != UI_menu)
      {
        p = &dig[bus[bus_UI]-1];
        if ((*p)<9) (*p)++;
      }
      else
      {
        print16x2(bus, D16x2_LIN_1|D16x2_ALIN_IZQ, "Loteo");
        return loteo;
      }
      break;
    case 'a':
      if (bus[bus_UI] != UI_menu)
      {
        bus[bus_UI]++;
        if (bus[bus_UI] > UI_esp_max_dig) bus[bus_UI] = UI_esp_max_dig;
      }
      break;
    case 'd':
      if (bus[bus_UI] != UI_menu)
      {
        bus[bus_UI]--;
        if (bus[bus_UI] < 1) bus[bus_UI] = 1;
      }
      break;
    case 'o':
      if (bus[bus_UI] == UI_menu)
      {
        bus[bus_UI] = UI_dig1;
        set_msg(bus, 3, bus[bus_t_esp]);
        print16x2(bus, D16x2_LIN_1|D16x2_ALIN_DER|D16x2_BRR_ALL, "Pausa          v");
        return t_esp;
      }
      else
      {
        bus[bus_UI] = UI_menu;
        bus[bus_t_esp] = dig[0] + (dig[1]*10);
      }
      break;
    case 'f':
      retval = rep3;
      bus[bus_UI] = UI_reposo_cant;
      print16x2(bus, D16x2_LIN_1|D16x2_BRR_ALL, "Parcial");
      break;
  }
  if (auxint != -1)
  {
    int i;
    auxint = 0;
    for (i=0; i<UI_esp_max_dig; i++) auxint += dig[i]*pow(10,i);
    set_msg(bus, 3, auxint);
  }
  /* Imprimir cambios de estado */
  if (bus[bus_UI] != aux_e)
  {
    strcpy(auxstr, "Pausa       ");
    switch (bus[bus_UI])
    {
      case UI_dig1:
        strcat(auxstr, "   v");
        break;
      case UI_dig2:
        strcat(auxstr, "  v ");
        break;
      case UI_dig3:
        strcat(auxstr, " v  ");
        break;
      case UI_dig4:
        strcat(auxstr, "v   ");
        break;
      default:
        strcat(auxstr, "    ");
        break;
    }
    print16x2(bus, D16x2_LIN_1|D16x2_ALIN_DER|D16x2_BRR_ALL, auxstr);
  }
  /* Imprimir datos variables */
  switch (bus[bus_UI])
  {
    case UI_menu:
      sprintf(auxstr, "%d", bus[bus_t_esp]);
      print16x2(bus, D16x2_LIN_2|D16x2_ALIN_DER, auxstr);
      break;
    default:
      sprintf(auxstr, "%02d", auxint);
      print16x2(bus, D16x2_LIN_2|D16x2_ALIN_DER, auxstr);
      break;
  }
  return retval;
}