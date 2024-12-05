#include "greedy.h"

/*Implementación del algoritmo greedy para dar con la solución incial*/
void Greedy::resolinigreedy(TableroSudoku& tablero) {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (tablero.obtenerVal(row, col) == 0) {
                for (int value = 1; value <= 9; ++value) {
                    if (tablero.MovimientoValido(row, col, value)) {
                        tablero.setvalinicial(row, col, value);
                        break;
                    }
                }
            }
        }
    }
}
