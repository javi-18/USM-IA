#ifndef TABLEROSUDOKU_H
#define TABLEROSUDOKU_H

#include <string>
#include <fstream>
#include <vector>
#include <set>

/*Este archivo corresponde a la definición del tablero Sudoku obtenido a partir de las instancias entregadas*/
class TableroSudoku {
private:
    struct Casilla {
        int valor;  // Valor actual (0 si está vacía)
        int bloqueID;  // Identificador del bloque (cage)
    };

    std::vector<std::vector<Casilla>> tablero;  // Tablero 9x9 representado como matriz de Casilla
    std::vector<int> bloqueSums;  // Sumas objetivo de cada bloque
    std::vector<std::set<std::pair<int, int>>> bloques;  // Conjunto de coordenadas de cada bloque

public:
    TableroSudoku(const std::string& instanceFile);  // Constructor que crea el tablero a partir de un archivo de instancia
    void setvalinicial(int row, int col, int value);  // Establece un valor inicial en una celda
    int obtenerVal(int row, int col) const;  // Obtiene el valor de una celda específica
    int evaluar() const;  // Evalúa la calidad de la solución actual en función de restricciones
    bool MovimientoValido(int row, int col, int value) const;  // Comprueba si un valor puede ser colocado en una celda específica

    void imprimeTablero() const;  // Imprime el tablero en la consola
    void imprimeMejorTab(std::ofstream& outFile) const;  // Imprime la mejor solución en un archivo
};

#endif
