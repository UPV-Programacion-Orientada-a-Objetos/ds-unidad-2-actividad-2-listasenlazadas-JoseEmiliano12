/**
 * @file TramaBase.h
 * @brief Clase abstracta base para todas las tramas del protocolo PRT-7
 */

#ifndef TRAMA_BASE_H
#define TRAMA_BASE_H

class ListaDeCarga;
class RotorDeMapeo;

/**
 * @class TramaBase
 * @brief Interfaz comun para todas las tramas recibidas por el puerto serial
 */
class TramaBase
{
public:
    /**
     * @brief Procesa la trama usando las estructuras de datos proporcionadas
     * @param carga Puntero a la lista de carga donde se almacenan los datos
     * @param rotor Puntero al rotor de mapeo que realiza la decodificacion
     */
    virtual void procesar(ListaDeCarga *carga, RotorDeMapeo *rotor) = 0;

    /**
     * @brief Destructor virtual para permitir polimorfismo
     */
    virtual ~TramaBase() {}
};

#endif