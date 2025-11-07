/**
 * @file ListaDeCarga.h
 * @brief Lista doblemente enlazada para almacenar caracteres decodificados
 */

#ifndef LISTA_DE_CARGA_H
#define LISTA_DE_CARGA_H

/**
 * @struct NodoCarga
 * @brief Nodo de la lista de carga
 */
struct NodoCarga
{
    char dato;            ///< Caracter decodificado
    NodoCarga *siguiente; ///< Puntero al siguiente nodo
    NodoCarga *previo;    ///< Puntero al nodo anterior
};

/**
 * @class ListaDeCarga
 * @brief Lista doblemente enlazada para almacenar el mensaje decodificado
 */
class ListaDeCarga
{
private:
    NodoCarga *cabeza; ///< Primer nodo de la lista
    NodoCarga *cola;   ///< Ultimo nodo de la lista

public:
    /**
     * @brief Constructor que inicializa la lista vacia
     */
    ListaDeCarga();

    /**
     * @brief Destructor que libera la memoria
     */
    ~ListaDeCarga();

    /**
     * @brief Inserta un caracter al final de la lista
     * @param dato Caracter a insertar
     */
    void insertarAlFinal(char dato);

    /**
     * @brief Imprime el mensaje almacenado en la lista
     */
    void imprimirMensaje();
};

#endif