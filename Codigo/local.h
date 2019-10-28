/*   local.h   */

#ifndef LOCAL_H

  #define LOCAL_H

  #ifdef SIS_OP_NO_INFO
    #undef SIS_OP_NO_INFO
  #endif

  // Opciones de sistema operativo
  #define SIS_OP_ANDROID
  #define SIS_OP_WIN

  // Opciones de compilador
  #define no_compila_varios_punto_c

#endif