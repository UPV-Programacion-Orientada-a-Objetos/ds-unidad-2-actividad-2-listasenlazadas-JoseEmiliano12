/**
 * @file RotorDeMapeo.h
 * @brief Lista circular doblemente enlazada que actua como disco de cifrado
 */

#ifndef ROTOR_DE_MAPEO_H
#define ROTOR_DE_MAPEO_H

/**
 * @struct NodoRotor
 * @brief Nodo de la lista circular para el rotor
 */
struct NodoRotor
{
    char letra;           ///< Letra del alfabeto
    NodoRotor *siguiente; ///< Puntero al siguiente nodo
    NodoRotor *previo;    ///< Puntero al nodo anterior
};

/**
 * @class RotorDeMapeo
 * @brief Implementa un rotor rotatorio para mapeo de caracteres
 */
class RotorDeMapeo
{
private:
    NodoRotor *cabeza; ///< Puntero a la posicion cero del rotor
    int tamanio;       ///< Tamanio del rotor (26 para A-Z)

public:
    /**
     * @brief Constructor que inicializa el rotor con A-Z
     */
    RotorDeMapeo();

    /**
     * @brief Destructor que libera la memoria
     */
    ~RotorDeMapeo();

    /**
     * @brief Rota el rotor N posiciones
     * @param n Numero de posiciones (positivo=derecha, negativo=izquierda)
     */
    void rotar(int n);

    /**
     * @brief Mapea un caracter usando la posicion actual del rotor
     * @param in Caracter a mapear
     * @return Caracter mapeado
     */
    char getMapeo(char in);
};

#endif