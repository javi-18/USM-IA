#ifndef TABUSEARCH
#define TABUSEARCH

#include "TableroSudoku.h"
#include <set>
#include <vector>
#include <utility>

class TabuSearch{
private:
    std::set<std::pair<int, int>> ListaTabu; //Lista tabu, almacena los movimientos que no se pueden realizar 
    int maxIteraciones; // variable que representa el número máximo de iteraciones que realizara el algoritmo 
    int DuracionTabu; //duracion de los movimientos dentro de la lista tabu 
    int calcularTamanoInstancia(const TableroSudoku& tablero) const; // calcula el tamaño de la instancia según el número de celdas vacías
    int iteracionesRealizadas;

public:
    TabuSearch(int maxIter, int tenure); //constructor del algoritmo tabu search 
    TabuSearch(const TableroSudoku& tablero);
    void resolTS(TableroSudoku& tablero); // método para resolver el tablero mediante tabu search
    int getIteracionesRealizadas() const { return iteracionesRealizadas;}
};

#endif
