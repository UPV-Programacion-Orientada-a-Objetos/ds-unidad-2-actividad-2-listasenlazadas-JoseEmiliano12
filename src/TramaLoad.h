/**
 * @file TramaLoad.h
 * @brief Trama de tipo LOAD que contiene un caracter a decodificar
 */

#ifndef TRAMA_LOAD_H
#define TRAMA_LOAD_H

#include "TramaBase.h"

/**
 * @class TramaLoad
 * @brief Representa una trama L,X que carga un caracter
 */
class TramaLoad : public TramaBase
{
private:
    char dato; ///< Caracter recibido en la trama

public:
    /**
     * @brief Constructor que inicializa el caracter
     * @param c Caracter recibido desde el serial
     */
    TramaLoad(char c);

    /**
     * @brief Procesa la trama decodificando el caracter
     * @param carga Lista donde se almacena el caracter decodificado
     * @param rotor Rotor que mapea el caracter original al decodificado
     */
    void procesar(ListaDeCarga *carga, RotorDeMapeo *rotor);
};

#endif