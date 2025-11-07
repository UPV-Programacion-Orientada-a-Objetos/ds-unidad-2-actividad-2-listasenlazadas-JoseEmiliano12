/**
 * @file RotorDeMapeo.cpp
 * @brief Implementacion del rotor de mapeo circular
 */

#include "RotorDeMapeo.h"

RotorDeMapeo::RotorDeMapeo()
{
    cabeza = nullptr;
    tamanio = 26;

    // Crear nodos para A-Z
    NodoRotor *primero = nullptr;
    NodoRotor *anterior = nullptr;

    for (int i = 0; i < 26; i++)
    {
        NodoRotor *nuevo = new NodoRotor;
        nuevo->letra = 'A' + i;
        nuevo->siguiente = nullptr;
        nuevo->previo = anterior;

        if (anterior != nullptr)
        {
            anterior->siguiente = nuevo;
        }
        else
        {
            primero = nuevo;
        }

        anterior = nuevo;
    }

    // Hacer la lista circular
    if (primero != nullptr && anterior != nullptr)
    {
        primero->previo = anterior;
        anterior->siguiente = primero;
        cabeza = primero;
    }
}

RotorDeMapeo::~RotorDeMapeo()
{
    if (cabeza == nullptr)
        return;

    // Romper el circulo
    NodoRotor *ultimo = cabeza->previo;
    ultimo->siguiente = nullptr;

    // Eliminar todos los nodos
    NodoRotor *actual = cabeza;
    while (actual != nullptr)
    {
        NodoRotor *temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
}

void RotorDeMapeo::rotar(int n)
{
    if (cabeza == nullptr)
        return;

    // Normalizar n para evitar rotaciones innecesarias
    n = n % tamanio;

    // Si es negativo, convertir a positivo equivalente
    if (n < 0)
    {
        n = tamanio + n;
    }

    // Rotar moviendo la cabeza
    for (int i = 0; i < n; i++)
    {
        cabeza = cabeza->siguiente;
    }
}

char RotorDeMapeo::getMapeo(char in)
{
    // Si no es letra, retornar tal cual
    if ((in < 'A' || in > 'Z') && (in < 'a' || in > 'z'))
    {
        return in; // Espacios u otros caracteres
    }

    // Convertir a mayuscula si es necesario
    bool eraMinuscula = false;
    if (in >= 'a' && in <= 'z')
    {
        in = in - 32; // Convertir a mayuscula
        eraMinuscula = true;
    }

    // Calcular el desplazamiento del caracter respecto a 'A'
    int posicion = in - 'A';

    // Encontrar el nodo correspondiente en el rotor
    NodoRotor *nodoActual = cabeza;
    for (int i = 0; i < posicion; i++)
    {
        nodoActual = nodoActual->siguiente;
    }

    // El caracter mapeado es el que esta en ese nodo
    char resultado = nodoActual->letra;

    // Si era minuscula, regresar a minuscula
    if (eraMinuscula)
    {
        resultado = resultado + 32;
    }

    return resultado;
}