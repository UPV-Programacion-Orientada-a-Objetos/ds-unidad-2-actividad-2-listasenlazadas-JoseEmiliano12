/**
 * @file TramaMap.h
 * @brief Trama de tipo MAP que rota el rotor de decodificacion
 */

#ifndef TRAMA_MAP_H
#define TRAMA_MAP_H

#include "TramaBase.h"

/**
 * @class TramaMap
 * @brief Representa una trama M,N que rota el rotor
 */
class TramaMap : public TramaBase
{
private:
    int rotacion; ///< Cantidad de posiciones a rotar (puede ser negativa)

public:
    /**
     * @brief Constructor que inicializa la rotacion
     * @param n Numero de posiciones a rotar
     */
    TramaMap(int n);

    /**
     * @brief Procesa la trama rotando el rotor
     * @param carga Lista de carga (no se usa en MAP)
     * @param rotor Rotor que se va a rotar
     */
    void procesar(ListaDeCarga *carga, RotorDeMapeo *rotor);
};

#endif