/**
 * @file ListaDeCarga.cpp
 * @brief Implementacion de la lista de carga
 */

#include "ListaDeCarga.h"
#include <iostream>

ListaDeCarga::ListaDeCarga()
{
    cabeza = nullptr;
    cola = nullptr;
}

ListaDeCarga::~ListaDeCarga()
{
    NodoCarga *actual = cabeza;
    while (actual != nullptr)
    {
        NodoCarga *temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
}

void ListaDeCarga::insertarAlFinal(char dato)
{
    NodoCarga *nuevo = new NodoCarga;
    nuevo->dato = dato;
    nuevo->siguiente = nullptr;
    nuevo->previo = cola;

    if (cola != nullptr)
    {
        cola->siguiente = nuevo;
    }
    else
    {
        cabeza = nuevo;
    }

    cola = nuevo;
}

void ListaDeCarga::imprimirMensaje()
{
    NodoCarga *actual = cabeza;
    while (actual != nullptr)
    {
        std::cout << "[" << actual->dato << "]";
        actual = actual->siguiente;
    }
}