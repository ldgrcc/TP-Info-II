
# TP Informatica II

## Integrantes
* Castaño Gabriel
* Burgos Pablo
* Grecco Leandro

## Descripcion del proyecto
El proyecto busca automatizar una dobladora de papel de uso industrial. Para ello dividimos el funcionamiento en 3 partes: Entrada, Salida y Control. Para cada una desarrollamos una maquina de estados.

### Entrada
La entrada debe alimentar la maquina controlando la salida de papel cerrando o abriendo la valvula de aire.
* Cuando la cantidad doblada alcanza la cantidad designada a un lote, cerrar la valvula de aire.
* Cuando la salida termine de apilar, abrir la valvula de aire y borrar el contador parcial.

### Salida
de eso comienza el proceso de apilado:
* Esperar que salgan los papeles que quedan en maquina.
* Acelerar la cinta de salida una cantidad de tiempo configurable.
* Volver a velocidad de cinta normal y mover el apilador hasta que se active el sensor de fin de carrera.
* Volver al estado de reposo.

### Control
La parte de control controla la interfaz de usuario. Tiene 2 modos de operación: Información y configuración.
Información muestra:
* Cantidad de papeles doblados en el lote actual.
* Promedio de papeles por hora, teniendo en cuenta velocidad del papel, frecuencia y tiempo de apilado.
* Total doblado hasta el momento.
* * h
* * j
