/**
 * @file SerialPort.h
 * @brief Clase para manejar comunicacion serial en Windows
 */

#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @class SerialPort
 * @brief Maneja la comunicacion con el puerto serial
 */
class SerialPort
{
private:
#ifdef _WIN32
    HANDLE hSerial; ///< Handle del puerto serial en Windows
#else
    int fd; ///< File descriptor del puerto serial en POSIX
#endif
    bool conectado; ///< Estado de la conexion

public:
    /**
     * @brief Constructor que abre el puerto serial
     * @param portName Nombre del puerto (ej: "COM3")
     */
    SerialPort(const char *portName);

    /**
     * @brief Destructor que cierra el puerto
     */
    ~SerialPort();

    /**
     * @brief Lee una linea del puerto serial
     * @param buffer Buffer donde se almacena la linea leida
     * @param bufferSize Tamanio del buffer
     * @return true si se leyo correctamente, false en caso contrario
     */
    bool leerLinea(char *buffer, int bufferSize);

    /**
     * @brief Verifica si esta conectado
     * @return true si esta conectado
     */
    bool estaConectado();
};

#endif