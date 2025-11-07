/**
 * @file SerialPort.cpp
 * @brief Implementacion de la comunicacion serial para Windows y Linux
 */

#include "SerialPort.h"
#include <iostream>
#include <cerrno>
#include <cstring>
#include <string>

#ifndef _WIN32
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

SerialPort::SerialPort(const char *portName)
{
    conectado = false;

#ifdef _WIN32
    // Abrir puerto serial (Windows)
    hSerial = CreateFileA(portName,
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cout << "Error: No se pudo abrir el puerto " << portName << std::endl;
        return;
    }

    // Configurar parametros del puerto
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        std::cout << "Error al obtener configuracion del puerto" << std::endl;
        CloseHandle(hSerial);
        return;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams))
    {
        std::cout << "Error al configurar puerto" << std::endl;
        CloseHandle(hSerial);
        return;
    }

    // Configurar timeouts mas largos
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 100;
    timeouts.ReadTotalTimeoutConstant = 500;
    timeouts.ReadTotalTimeoutMultiplier = 100;

    if (!SetCommTimeouts(hSerial, &timeouts))
    {
        std::cout << "Error al configurar timeouts" << std::endl;
        CloseHandle(hSerial);
        return;
    }

    // Limpiar buffers
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

    conectado = true;
    std::cout << "Conexion establecida en " << portName << std::endl;

#else
    // Abrir puerto serial (Linux/POSIX)
    std::string path = portName ? std::string(portName) : std::string();
    if (path.empty())
    {
        std::cout << "Error: nombre de puerto vacio" << std::endl;
        return;
    }

    // Si no tiene ruta completa, agregar /dev/
    if (path[0] != '/')
    {
        path = std::string("/dev/") + path;
    }

    fd = open(path.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd == -1)
    {
        std::cout << "Error: No se pudo abrir el puerto " << path << " (" << strerror(errno) << ")" << std::endl;
        return;
    }

    // Configurar parametros del puerto serial (9600 8N1)
    struct termios options;
    if (tcgetattr(fd, &options) != 0)
    {
        std::cout << "Error al obtener atributos del puerto: " << strerror(errno) << std::endl;
        close(fd);
        return;
    }

    // Establecer velocidad
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // Control flags
    options.c_cflag &= ~PARENB;    // Sin paridad
    options.c_cflag &= ~CSTOPB;    // 1 bit de stop
    options.c_cflag &= ~CSIZE;     // Limpiar tamaño
    options.c_cflag |= CS8;        // 8 bits de datos
    options.c_cflag &= ~CRTSCTS;   // Sin control de flujo RTS/CTS
    options.c_cflag |= CLOCAL;     // Ignorar señales de módem
    options.c_cflag |= CREAD;      // Habilitar lectura

    // Input flags
    options.c_iflag = 0;

    // Output flags
    options.c_oflag = 0;

    // Local flags
    options.c_lflag = 0;

    // Time between characters and minimum characters to read
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 20; // 2 segundos timeout

    tcflush(fd, TCIOFLUSH);
    if (tcsetattr(fd, TCSANOW, &options) != 0)
    {
        std::cout << "Error al configurar puerto: " << strerror(errno) << std::endl;
        close(fd);
        return;
    }

    // Limpiar buffers de entrada/salida
    sleep(2);  // Esperar a que Arduino se reinicie y envíe INICIO
    tcflush(fd, TCIOFLUSH);  // Limpiar entrada y salida
    
    // Descartar todo lo que Arduino haya enviado durante la inicialización
    char descarte[256];
    ssize_t n;
    while ((n = read(fd, descarte, sizeof(descarte))) > 0)
    {
        // Solo descartar
    }
    tcflush(fd, TCIOFLUSH);  // Limpiar nuevamente
    
    conectado = true;
    std::cout << "Conexion establecida en " << path << std::endl;
#endif
}

SerialPort::~SerialPort()
{
#ifdef _WIN32
    if (conectado)
    {
        CloseHandle(hSerial);
    }
#else
    if (conectado)
    {
        close(fd);
    }
#endif
}

bool SerialPort::leerLinea(char *buffer, int bufferSize)
{
#ifdef _WIN32
    if (!conectado)
        return false;

    DWORD bytesLeidos;
    int indice = 0;
    bool finDeLinea = false;

    // Limpiar buffer
    for (int i = 0; i < bufferSize; i++)
    {
        buffer[i] = '\0';
    }

    // Leer caracter por caracter hasta encontrar fin de linea
    while (indice < bufferSize - 1 && !finDeLinea)
    {
        char c;
        DWORD leidos = 0;

        if (ReadFile(hSerial, &c, 1, &leidos, NULL))
        {
            if (leidos > 0)
            {
                // Ignorar retorno de carro
                if (c == '\r')
                {
                    continue;
                }

                // Fin de linea con salto de linea
                if (c == '\n')
                {
                    if (indice > 0)
                    {
                        finDeLinea = true;
                    }
                }
                else
                {
                    buffer[indice++] = c;
                }
            }
        }
        else
        {
            // Error de lectura
            break;
        }
    }

    buffer[indice] = '\0';
    return (indice > 0);
#else
    // Implementación POSIX (Linux)
    if (!conectado)
        return false;

    int indice = 0;
    // Limpiar buffer
    for (int i = 0; i < bufferSize; i++)
    {
        buffer[i] = '\0';
    }

    bool finDeLinea = false;
    int timeoutsConsecutivos = 0;
    const int MAX_TIMEOUTS = 50;

    // Leer caracteres hasta encontrar fin de línea (\n)
    while (indice < bufferSize - 1 && !finDeLinea)
    {
        char c;
        ssize_t r = read(fd, &c, 1);

        if (r > 0)
        {
            timeoutsConsecutivos = 0;

            // Ignorar retorno de carro
            if (c == '\r')
            {
                continue;
            }

            // Fin de línea
            if (c == '\n')
            {
                if (indice > 0)
                {
                    finDeLinea = true;
                }
                continue;
            }

            // Acumular caracteres
            buffer[indice++] = c;
        }
        else if (r == 0)
        {
            // timeout sin datos
            timeoutsConsecutivos++;
            if (timeoutsConsecutivos >= MAX_TIMEOUTS)
            {
                // Si tenemos algo acumulado, considerarlo como línea completa
                if (indice > 0)
                {
                    finDeLinea = true;
                }
                break;
            }
            usleep(1000); // 1ms entre intentos
        }
        else
        {
            // error o EAGAIN
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                timeoutsConsecutivos++;
                if (timeoutsConsecutivos >= MAX_TIMEOUTS && indice > 0)
                {
                    finDeLinea = true;
                    break;
                }
                usleep(1000); // 1ms
                continue;
            }
            break;
        }
    }

    buffer[indice] = '\0';
    return (indice > 0);
#endif
}

bool SerialPort::estaConectado()
{
    return conectado;
}