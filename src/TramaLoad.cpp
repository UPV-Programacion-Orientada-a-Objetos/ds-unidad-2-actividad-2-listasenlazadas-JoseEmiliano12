/**
 * @file TramaLoad.cpp
 * @brief Implementacion de la clase TramaLoad
 */

#include "TramaLoad.h"
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"
#include <iostream>

TramaLoad::TramaLoad(char c) : dato(c) {}

void TramaLoad::procesar(ListaDeCarga *carga, RotorDeMapeo *rotor)
{
    // Obtener el caracter decodificado usando el rotor
    char decodificado = rotor->getMapeo(dato);

    // Insertar en la lista de carga
    carga->insertarAlFinal(decodificado);

    // Mostrar en consola lo que se procesa
    std::cout << "Trama recibida: [L," << dato << "] -> Fragmento '" << dato 
              << "' decodificado como '" << decodificado << "'." << std::endl;
}