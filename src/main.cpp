/**
 * @file main.cpp
 * @brief Programa principal del decodificador PRT-7
 */

#include "TramaBase.h"
#include "TramaLoad.h"
#include "TramaMap.h"
#include "RotorDeMapeo.h"
#include "ListaDeCarga.h"
#include "SerialPort.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

/**
 * @brief Parsea una linea recibida del serial y crea la trama correspondiente
 * @param linea Linea a parsear (formato: "L,X" o "M,N")
 * @return Puntero a la trama creada o nullptr si hay error
 */
TramaBase *parsearTrama(const char *linea)
{
    // Verificar que la linea tenga contenido
    if (linea == nullptr || linea[0] == '\0')
    {
        return nullptr;
    }

    // Copiar la linea para no modificar el original
    char buffer[100];
    int i = 0;
    while (linea[i] != '\0' && i < 99)
    {
        buffer[i] = linea[i];
        i++;
    }
    buffer[i] = '\0';

    // Buscar la coma
    char tipo = buffer[0];
    int posicionComa = -1;

    for (int j = 0; buffer[j] != '\0'; j++)
    {
        if (buffer[j] == ',')
        {
            posicionComa = j;
            break;
        }
    }

    // Si no hay coma, la trama es invalida
    if (posicionComa == -1)
    {
        return nullptr;
    }

    // Extraer el valor despues de la coma (puede estar vacío para espacio)
    char *valor = &buffer[posicionComa + 1];

    // Crear la trama segun el tipo
    if (tipo == 'L' || tipo == 'l')
    {
        // Trama de carga - tomar el primer caracter despues de la coma
        // (puede estar vacío si es un espacio)
        char caracter = valor[0] != '\0' ? valor[0] : ' ';
        return new TramaLoad(caracter);
    }
    else if (tipo == 'M' || tipo == 'm')
    {
        // Trama de mapeo - convertir a numero
        int rotacion = atoi(valor);
        return new TramaMap(rotacion);
    }

    return nullptr;
}

/**
 * @brief Verifica si una trama es del tipo MAP con rotacion negativa (patron de fin)
 * @param linea Linea a verificar
 * @return true si es M con numero negativo
 */
bool esPatronDeFin(const char *linea)
{
    if (linea == nullptr || linea[0] == '\0')
        return false;

    // Debe empezar con M o m
    if (linea[0] != 'M' && linea[0] != 'm')
        return false;

    // Buscar la coma
    int i = 0;
    while (linea[i] != '\0' && linea[i] != ',')
        i++;

    if (linea[i] != ',')
        return false;

    // Verificar si hay un signo negativo despues de la coma
    i++; // Saltar la coma
    while (linea[i] == ' ' || linea[i] == '\t')
        i++; // Saltar espacios

    if (linea[i] == '-')
    {
        return true;
    }

    return false;
}

/**
 * @brief Funcion principal
 */
int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  Decodificador de Protocolo PRT-7" << std::endl;
    std::cout << "========================================" << std::endl
              << std::endl;

    // Solicitar puerto COM
    char puerto[20];
    std::cout << "Ingrese el puerto COM (ej: COM3): ";
    std::cin >> puerto;

#ifdef _WIN32
    // Formatear para Windows
    char puertoCompleto[30] = "\\\\.\\";
    strcat(puertoCompleto, puerto);
#else
    // En Linux, pasar el puerto tal cual (SerialPort lo maneja)
    char puertoCompleto[30];
    strncpy(puertoCompleto, puerto, 29);
    puertoCompleto[29] = '\0';
#endif

    std::cout << std::endl
              << "Iniciando Decodificador PRT-7. Conectando a puerto "
              << puerto << "..." << std::endl;

    // Conectar al puerto serial
    SerialPort serial(puertoCompleto);

    if (!serial.estaConectado())
    {
        std::cout << "No se pudo conectar. Verifique que:" << std::endl;
        std::cout << "1. El Arduino este conectado" << std::endl;
        std::cout << "2. El puerto COM sea correcto" << std::endl;
        std::cout << "3. Ningun otro programa use el puerto" << std::endl;
        return 1;
    }

    std::cout << "Conexion establecida. Esperando tramas..." << std::endl
              << std::endl;

    // Crear estructuras de datos
    ListaDeCarga listaCarga;
    RotorDeMapeo rotor;

    // Buffer para leer lineas
    char buffer[100];
    bool mensajeCompleto = false;
    int tramasProcesadas = 0;
    const int TRAMAS_MINIMAS = 10; // Al menos 10 tramas para considerar un mensaje completo

    // Sincronizar: descartar líneas hasta encontrar L,H
    std::cout << "Sincronizando con Arduino..." << std::endl;
    bool primeraSincronizacion = true;
    bool encontroInicio = false;
    
    while (primeraSincronizacion)
    {
        if (serial.leerLinea(buffer, 100))
        {
            if (buffer[0] == 'L' && buffer[1] == ',' && buffer[2] == 'H')
            {
                encontroInicio = true;
                primeraSincronizacion = false;
            }
        }
    }
    std::cout << "Listo para recibir tramas." << std::endl << std::endl;

    // Procesar el primer L,H que encontramos durante la sincronización
    if (encontroInicio)
    {
        TramaBase *trama = parsearTrama(buffer);
        if (trama != nullptr)
        {
            trama->procesar(&listaCarga, &rotor);
            tramasProcesadas++;
            delete trama;
        }
    }

    // Bucle principal de procesamiento
    while (!mensajeCompleto)
    {
        if (serial.leerLinea(buffer, 100))
        {
            // Eliminar espacios al inicio
            int inicio = 0;
            while (buffer[inicio] == ' ' || buffer[inicio] == '\t')
                inicio++;

            // Eliminar espacios al final
            int fin = 0;
            while (buffer[fin] != '\0')
                fin++;
            if (fin > 0)
                fin--;
            // Solo eliminar espacios al final si están después de una coma
            while (fin > inicio && buffer[fin] == ' ')
            {
                // Si hay una coma antes y este es el último carácter, es probablemente padding
                if (fin > inicio + 1)
                {
                    bool hayComaBefore = false;
                    for (int k = inicio; k < fin; k++)
                    {
                        if (buffer[k] == ',')
                        {
                            hayComaBefore = true;
                            break;
                        }
                    }
                    if (!hayComaBefore)
                        break; // No hay coma, podría ser un espacio válido
                }
                buffer[fin] = '\0';
                fin--;
            }

            // Si la linea esta vacia, continuar
            if (buffer[inicio] == '\0')
                continue;

            // Parsear la trama
            TramaBase *trama = parsearTrama(&buffer[inicio]);

            if (trama != nullptr)
            {
                // Procesar la trama
                trama->procesar(&listaCarga, &rotor);
                tramasProcesadas++;

                // Verificar si es el patron de fin Y ya procesamos suficientes tramas
                if (esPatronDeFin(&buffer[inicio]) && tramasProcesadas >= TRAMAS_MINIMAS)
                {
                    std::cout << std::endl
                              << ">>> Patron de fin detectado. Mensaje completo. <<<" << std::endl;
                    mensajeCompleto = true;
                }

                // Liberar memoria
                delete trama;
            }
            else
            {
                // Solo mostrar tramas invalidas si parecen ser tramas reales
                if ((buffer[inicio] == 'L' || buffer[inicio] == 'l' ||
                     buffer[inicio] == 'M' || buffer[inicio] == 'm'))
                {
                    std::cout << "Trama invalida recibida: [" << &buffer[inicio] << "]" << std::endl;
                }
            }
        }
    }

    std::cout << std::endl
              << "---" << std::endl;
    std::cout << "Flujo de datos terminado." << std::endl;
    std::cout << "MENSAJE OCULTO ENSAMBLADO:" << std::endl;
    listaCarga.imprimirMensaje();
    std::cout << std::endl
              << "---" << std::endl;
    std::cout << "Liberando memoria... Sistema apagado." << std::endl;

    return 0;
}