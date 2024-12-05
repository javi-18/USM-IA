#include "TableroSudoku.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>  // Para std::max

// Constructor: Inicializa el tablero leyendo el archivo de instancia
TableroSudoku::TableroSudoku(const std::string& instanceFile) {
    tablero.resize(9, std::vector<Casilla>(9));  // Inicializar el tablero como una matriz 9x9
    std::ifstream file(instanceFile);
    
    // Verificar si el archivo se abrió correctamente
    if (!file.is_open()) {
        throw std::runtime_error("Error al abrir el archivo de instancia.");
    }

    // Leer y cargar el tablero inicial a partir del archivo
    for (int i = 0; i < 9; ++i) {
        std::string line;
        std::getline(file, line);  // Leer una línea del archivo

        if (line.empty()) {  // Validar que no haya líneas vacías
            throw std::runtime_error("Línea vacía encontrada en la instancia.");
        }

        std::istringstream iss(line);
        for (int j = 0; j < 9; ++j) {
            std::string casilla;
            iss >> casilla;  // Leer cada casilla en formato aCn (e.g., 5C1, .C3)

            // Validar el formato de la casilla
            if (casilla.size() < 3 || casilla[1] != 'C') {
                throw std::runtime_error("Formato incorrecto en la casilla: " + casilla);
            }

            char valueChar = casilla[0];  // Primer carácter: valor o '.'
            int bloqueID = std::stoi(casilla.substr(2));  // Extraer el ID del bloque desde la posición 2

            // Asignar el valor a la celda, 0 si está vacía
            tablero[i][j].valor = (valueChar == '.') ? 0 : valueChar - '0';
            tablero[i][j].bloqueID = bloqueID;  // Asignar el ID del bloque correspondiente

            // Asegurar que el vector bloques tenga el tamaño suficiente
            if (bloques.size() < static_cast<size_t>(bloqueID)) {
                bloques.resize(bloqueID);
            }

            // Añadir la celda al bloque correspondiente
            bloques[bloqueID - 1].insert({i, j});
        }
    }

    // Leer las sumas objetivo para cada bloque
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;  // Saltar líneas vacías

        std::istringstream iss(line);
        std::string bloqueInfo;
        int bloqueID, bloqueSum;

        std::getline(iss, bloqueInfo, ':');  // Leer hasta el carácter ':'
        bloqueID = std::stoi(bloqueInfo.substr(1));  // Extraer el número del bloque
        iss >> bloqueSum;  // Leer la suma objetivo del bloque

        // Asegurar que el vector de sumas tenga el tamaño suficiente
        if (bloqueSums.size() < static_cast<size_t>(bloqueID)) {
            bloqueSums.resize(bloqueID);
        }

        // Asignar la suma objetivo al bloque correspondiente
        bloqueSums[bloqueID - 1] = bloqueSum;
    }

    file.close();  // Cerrar el archivo
}

// Establece un valor inicial en una celda específica
void TableroSudoku::setvalinicial(int row, int col, int value) {
    tablero[row][col].valor = value;
}

// Obtiene el valor de una celda específica
int TableroSudoku::obtenerVal(int row, int col) const {
    return tablero[row][col].valor;
}

// Evalúa la calidad de la solución actual, calculando las restricciones violadas
int TableroSudoku::evaluar() const {
    int totalViolations = 0;

    // Verificar restricciones de filas
    for (int i = 0; i < 9; ++i) {
        std::vector<bool> seen(10, false);  // Vector para rastrear valores únicos
        for (int j = 0; j < 9; ++j) {
            int value = tablero[i][j].valor;
            if (value != 0 && seen[value]) {
                totalViolations++;  // Incrementar por valores repetidos en la fila
            }
            seen[value] = true;
        }
    }

    // Verificar restricciones de columnas
    for (int j = 0; j < 9; ++j) {
        std::vector<bool> seen(10, false);
        for (int i = 0; i < 9; ++i) {
            int value = tablero[i][j].valor;
            if (value != 0 && seen[value]) {
                totalViolations++;  // Incrementar por valores repetidos en la columna
            }
            seen[value] = true;
        }
    }

    // Verificar restricciones de bloques 3x3
    for (int blockRow = 0; blockRow < 3; ++blockRow) {
        for (int blockCol = 0; blockCol < 3; ++blockCol) {
            std::vector<bool> seen(10, false);
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    int row = blockRow * 3 + i;
                    int col = blockCol * 3 + j;
                    int value = tablero[row][col].valor;
                    if (value != 0 && seen[value]) {
                        totalViolations++;  // Incrementar por valores repetidos en el bloque
                    }
                    seen[value] = true;
                }
            }
        }
    }

    // Verificar restricciones de sumas de bloques personalizados (cages)
    for (size_t c = 0; c < bloques.size(); ++c) {
        int currentSum = 0;
        for (const auto& cell : bloques[c]) {
            currentSum += tablero[cell.first][cell.second].valor;
        }
        if (currentSum != bloqueSums[c]) {
            totalViolations++;  // Incrementar si la suma no coincide con el objetivo
        }
    }

    return totalViolations;
}

// Comprueba si un movimiento es válido en la celda especificada
bool TableroSudoku::MovimientoValido(int row, int col, int value) const {
    // Verificar restricciones de fila
    for (int j = 0; j < 9; ++j) {
        if (tablero[row][j].valor == value) {
            return false;
        }
    }

    // Verificar restricciones de columna
    for (int i = 0; i < 9; ++i) {
        if (tablero[i][col].valor == value) {
            return false;
        }
    }

    // Verificar restricciones de bloque 3x3
    int blockRow = (row / 3) * 3;
    int blockCol = (col / 3) * 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (tablero[blockRow + i][blockCol + j].valor == value) {
                return false;
            }
        }
    }

    // Verificar restricciones de bloque personalizado (cage)
    int bloqueID = tablero[row][col].bloqueID - 1;
    int currentSum = 0;
    for (const auto& cell : bloques[bloqueID]) {
        if (cell.first == row && cell.second == col) {
            currentSum += value;  // Considerar el nuevo valor
        } else {
            currentSum += tablero[cell.first][cell.second].valor;
        }
    }
    if (currentSum > bloqueSums[bloqueID]) {
        return false;
    }

    return true;
}

// Imprime el tablero en la consola
void TableroSudoku::imprimeTablero() const {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            std::cout << (tablero[i][j].valor == 0 ? "." : std::to_string(tablero[i][j].valor)) << " ";
        }
        std::cout << std::endl;
    }
}

// Imprime la mejor solución encontrada en un archivo
void TableroSudoku::imprimeMejorTab(std::ofstream& outFile) const {
    // Imprimir el tablero con el formato aCn (valor seguido del bloque correspondiente)
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            int valor = tablero[i][j].valor;
            int bloqueID = tablero[i][j].bloqueID;
            outFile << (valor == 0 ? "." : std::to_string(valor)) << "C" << bloqueID << " ";
        }
        outFile << std::endl; 
    }

    // Imprimir las sumas objetivo de cada bloque al final
    for (size_t c = 0; c < bloques.size(); ++c) {
        outFile << "C" << c + 1 << ":" << bloqueSums[c] << std::endl;
    }
}
