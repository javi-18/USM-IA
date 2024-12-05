#include "tabusearch.h"
#include <iostream>
#include <limits>
#include <algorithm> // Para std::max

// Constructor que ajusta los parámetros dinámicamente según el tamaño de la instancia
TabuSearch::TabuSearch(const TableroSudoku& tablero) {
    int tamanoInstancia = calcularTamanoInstancia(tablero);  // Calcula el número de celdas vacías
    maxIteraciones = std::max(100, tamanoInstancia * 200);  // Escalar iteraciones según el tamaño de la instancia
    DuracionTabu = std::max(10, tamanoInstancia / 2);  // Duración mínima de 10 en la lista Tabu
    iteracionesRealizadas = 0;  // Inicialización del contador
    std::cout << "Inicializando TabuSearch con " << maxIteraciones
              << " iteraciones máximas y duración Tabu de " << DuracionTabu << ".\n";
}

// Constructor alternativo con valores definidos directamente para iteraciones y duración Tabu
TabuSearch::TabuSearch(int maxIter, int tenure)
    : maxIteraciones(maxIter), DuracionTabu(tenure) {
    iteracionesRealizadas = 0;  // Inicialización del contador
    std::cout << "Inicializando TabuSearch con valores personalizados.\n";
}

// Método auxiliar que calcula el tamaño de la instancia basado en las celdas vacías del tablero
int TabuSearch::calcularTamanoInstancia(const TableroSudoku& tablero) const {
    int celdasVacias = 0;
    for (int fila = 0; fila < 9; ++fila) {
        for (int columna = 0; ++columna < 9; columna++) {
            if (tablero.obtenerVal(fila, columna) == 0) {  // Verifica si la celda está vacía
                celdasVacias++;
            }
        }
    }
    return celdasVacias;  // Retorna el número total de celdas vacías
}

// Método principal que ejecuta el algoritmo Tabu Search para optimizar la solución del tablero
void TabuSearch::resolTS(TableroSudoku& tablero) {
    iteracionesRealizadas = 0;  // Inicializa el contador de iteraciones a 0
    int mejorEvaluacion = tablero.evaluar();  // Evaluación inicial del tablero
    TableroSudoku mejorTablero = tablero;  // Almacena la mejor solución encontrada
    TableroSudoku tableroActual = tablero;  // Copia del tablero para explorar soluciones vecinas
    int sinMejoras = 0;  // Contador para iteraciones sin mejora

    // Imprime el tablero inicial y su evaluación
    std::cout << "Tablero inicial:" << std::endl;
    tablero.imprimeTablero();
    std::cout << "Evaluación inicial: " << mejorEvaluacion << "\n";

    // Bucle principal de Tabu Search que ejecuta hasta el máximo de iteraciones
    for (int iteracion = 0; iteracion < maxIteraciones; ++iteracion) {
        iteracionesRealizadas++;  // Incrementa el contador de iteraciones
        std::pair<int, int> mejorMovimiento;  // Almacena el mejor movimiento de la iteración
        int mejorEvaluacionVecina = std::numeric_limits<int>::max();  // Inicializa con el peor valor posible
        int mejorValor = 0;  // Almacena el mejor valor para la celda actual

        // Itera sobre todas las celdas del tablero
        for (int fila = 0; fila < 9; ++fila) {
            for (int columna = 0; ++columna < 9; columna++) {
                if (tableroActual.obtenerVal(fila, columna) == 0) {  // Verifica si la celda está vacía
                    // Prueba todos los valores posibles en la celda
                    for (int valor = 1; valor <= 9; ++valor) {
                        if (tableroActual.MovimientoValido(fila, columna, valor)) {  // Valida restricciones del Sudoku
                            tableroActual.setvalinicial(fila, columna, valor);  // Realiza el movimiento temporal
                            int evaluacionVecina = tableroActual.evaluar();  // Evalúa la calidad de la solución vecina

                            // Si el movimiento es válido y mejora la mejor evaluación vecina
                            if ((ListaTabu.find({fila, columna}) == ListaTabu.end() || evaluacionVecina < mejorEvaluacion) &&
                                evaluacionVecina < mejorEvaluacionVecina) {
                                mejorEvaluacionVecina = evaluacionVecina;  // Actualiza la mejor evaluación vecina
                                mejorMovimiento = {fila, columna};  // Almacena el mejor movimiento
                                mejorValor = valor;  // Almacena el valor asociado a ese movimiento
                            }
                            tableroActual.setvalinicial(fila, columna, 0);
                        }
                    }
                }
            }
        }

        // Imprime el progreso al final de cada iteración
        std::cout << "\nIteración: " << iteracion << std::endl;
        tableroActual.imprimeTablero();
        std::cout << "Evaluación actual: " << mejorEvaluacionVecina << std::endl;

        // Si no hay mejoras, maneja estancamiento
        if (mejorEvaluacionVecina >= mejorEvaluacion) {
            sinMejoras++;
            std::cout << "Estancado en iteración " << iteracion << ", diversificando..." << std::endl;
            if (sinMejoras >= 50) {  // Diversificar si se alcanzan 50 iteraciones sin mejora
                sinMejoras = 0;  // Resetea el contador de estancamiento
            }
            continue;
        } else {
            sinMejoras = 0;  // Resetea el contador si hubo mejoras
        }

        // Realiza el mejor movimiento encontrado
        tableroActual.setvalinicial(mejorMovimiento.first, mejorMovimiento.second, mejorValor);
        ListaTabu.insert(mejorMovimiento);  // Añade el movimiento a la lista Tabu

        // Actualiza la mejor solución global si mejora la evaluación
        if (mejorEvaluacionVecina < mejorEvaluacion) {
            mejorEvaluacion = mejorEvaluacionVecina;
            mejorTablero = tableroActual;  // Almacena la mejor solución global
        }

        // Controla el tamaño de la lista Tabu para mantenerlo dentro de DuracionTabu
        if (ListaTabu.size() > static_cast<size_t>(DuracionTabu)) {
            ListaTabu.erase(ListaTabu.begin());  // Elimina el movimiento más antiguo de la lista Tabu
        }
    }

    // Imprime el resultado final
    std::cout << "\nTablero después del algoritmo Tabu Search:" << std::endl;
    mejorTablero.imprimeTablero();
    std::cout << "Valor final de la función objetivo (mejor solución): " << mejorEvaluacion << std::endl;

    // Al final, actualiza el tablero con la mejor solución encontrada
    tablero = mejorTablero;
}
