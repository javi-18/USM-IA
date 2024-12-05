Nombre: Javiera Bobadilla
Rol: 202173584-4

Instrucciones:

1. Para ejecutar el programa, primero se debe tener todas los archivos de texto, que representan las instancias dentro de una carpeta
llamada instancias, ya que el programa esta configurado para ingresar a esta carpeta para leer las instancias, la cual lo hace mediante 
el archivo main.cpp .

2. Dentro de la terminal, se debe ejecutar el archivo mediante el comando "make" para compilar todos los archivos.h y archivos .cpp presentes.

3. Una vez compilados los archivos, se debe escribir el comando "./main" para ejecutar el programa.

4. Al comenzar la ejecución se debe escribir dentro de la consola, el nombre de la instancia a evaluar 
Ejemplo(dentro de la consola):
Ingrese el nombre del archivo de instancia : 10blank.txt

Una vez escrito el nombre de la instancia, este devolverá el tablero inicial leido del archivo de la instancia, junto a la resolución del tablero
inicialmente con greedy y luego con tabu search, la cual se ejecutara 100 veces para obtener el tiempo de ejecución y otras estadísticas, 
para de esa forma poder comparar la eficiencia de los algortimos a medida que crece el tamaño de la instancia. Se guardara la mejor solución 
dentro de un archivo .txt llamado solution.txt, el cual esta dentro de la carpeta instancias.

*Nota: También existe otro archivo llamado solution.txt, el cual corresponde a un archivo de solución que se entrego inicialmente, por lo que
no se debe tomar en cuenta al momento de la ejecución del programa

5. Para una mejor ejecución, usa el comando "make clean" para poder borrar los archivos .o creados y asi obtener mejores resultados.

*Consideraciones:
Dentro del código se agregó 3 parámetros, los cuales son:
- maxIteraciones = std::max(100, tamanoInstancia * 200) : representa el cantidad máxima de iteraciones que el algoritmo TabuSearch va a ejecutar
según el tamaño de la instancia.
-  DuracionTabu = std::max(10, tamanoInstancia / 2) : representa el tiempo que estaran los movimientos realizados(valor de la casilla) 
dentro de la lista tabu, donde el minimo valor de duración es de 10.
- int iteracionesRealizadas: representa la cantidad de iteraciones que lleva actualmente al ejecutar TabuSearch.
Los primeros 2 parámetros se  ajustan según el tamaño de la instancia, por lo que no hay que hacer modificaciones de código de ningún tipo
El tercer parámetro actúa como contador dentro del código, por lo que no hay que modificarlo manualmente.
*Nota2: Dentro del programa solo se usaron librerías estandar dentro de C++, por lo que no requiere de ninguna instalación de
librerías extras.
