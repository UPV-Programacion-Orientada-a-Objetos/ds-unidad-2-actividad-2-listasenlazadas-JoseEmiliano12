/**
 * @file TramaMap.cpp
 * @brief Implementacion de la clase TramaMap
 */

#include "TramaMap.h"
#include "RotorDeMapeo.h"
#include <iostream>

TramaMap::TramaMap(int n) : rotacion(n) {}

void TramaMap::procesar(ListaDeCarga *carga, RotorDeMapeo *rotor)
{
    // Rotar el rotor
    rotor->rotar(rotacion);

    // Mostrar en consola
    std::cout << "Trama recibida: [M," << rotacion << "] -> ROTANDO ROTOR " 
              << (rotacion >= 0 ? "+" : "") << rotacion << "." << std::endl;
}