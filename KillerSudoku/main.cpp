#include "TableroSudoku.h"
#include "greedy.h"
#include "tabusearch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>

int main() {
    std::string carpetaInstancias = "instancias/";
    std::string archivoInstancia;
    int numRepeticiones = 100; // Número de repeticiones

    std::cout << "Ingrese el nombre del archivo de instancia: ";
    std::cin >> archivoInstancia;

    std::string rutaCompleta = carpetaInstancias + archivoInstancia;

    try {
        // Vectores para almacenar estadísticas
        std::vector<double> tiemposEjecucion;
        std::vector<int> iteracionesRealizadas;
        std::vector<int> valoresFuncionObjetivo;

        TableroSudoku mejorTablero(rutaCompleta); // Inicializar con la misma instancia
        int mejorFuncionObjetivo = std::numeric_limits<int>::max(); // Inicializar con el peor valor

        // Ciclo de repeticiones
        for (int i = 0; i < numRepeticiones; ++i) {
            // Cargar el tablero inicial
            TableroSudoku tablero(rutaCompleta);

            std::cout << "\nEjecución " << i + 1 << ":\n";
            std::cout << "Tablero inicial:\n";
            tablero.imprimeTablero();

            // Resolver con algoritmo greedy para una solución inicial rápida
            Greedy greedySolver;
            greedySolver.resolinigreedy(tablero);
            std::cout << "\nTablero después del algoritmo Greedy:\n";
            tablero.imprimeTablero();

            // Medir el tiempo de ejecución del algoritmo Tabu Search
            auto start = std::chrono::high_resolution_clock::now();

            TabuSearch tabuSolver(tablero);
            tabuSolver.resolTS(tablero);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;

            // Mostrar el tablero final después de Tabu Search
            std::cout << "\nTablero después del algoritmo Tabu Search:\n";
            tablero.imprimeTablero();

            // Guardar estadísticas de esta ejecución
            tiemposEjecucion.push_back(duration.count());
            iteracionesRealizadas.push_back(tabuSolver.getIteracionesRealizadas());
            valoresFuncionObjetivo.push_back(tablero.evaluar());

            // Verificar si este tablero es mejor que el mejor encontrado hasta ahora
            if (tablero.evaluar() < mejorFuncionObjetivo) {
                mejorFuncionObjetivo = tablero.evaluar();
                mejorTablero = tablero; // Actualizar el mejor tablero
            }
        }

        // Calcular estadísticas globales
        double tiempoPromedio = std::accumulate(tiemposEjecucion.begin(), tiemposEjecucion.end(), 0.0) / tiemposEjecucion.size();
        double tiempoMax = *std::max_element(tiemposEjecucion.begin(), tiemposEjecucion.end());
        double tiempoMin = *std::min_element(tiemposEjecucion.begin(), tiemposEjecucion.end());

        int iteracionesPromedio = std::accumulate(iteracionesRealizadas.begin(), iteracionesRealizadas.end(), 0) / iteracionesRealizadas.size();

        // Mostrar estadísticas finales
        std::cout << "\nEstadísticas después de " << numRepeticiones << " ejecuciones:\n";
        std::cout << "Tiempo promedio de ejecución: " << tiempoPromedio << " segundos\n";
        std::cout << "Tiempo máximo de ejecución: " << tiempoMax << " segundos\n";
        std::cout << "Tiempo mínimo de ejecución: " << tiempoMin << " segundos\n";
        std::cout << "Iteraciones promedio: " << iteracionesPromedio << "\n";
        std::cout << "Mejor valor de la función objetivo: " << mejorFuncionObjetivo << "\n";

        // Guardar la mejor solución en un archivo
        std::ofstream outFile(carpetaInstancias +"solution.txt");
        if (outFile.is_open()) {
            mejorTablero.imprimeMejorTab(outFile);
            std::cout << "\nLa mejor solución se ha guardado correctamente en 'solution.txt'.\n";
        } else {
            std::cerr << "Error al guardar la solución en 'solution.txt'.\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

