# Decodificador de Protocolo Industrial PRT-7

**Autor:** Jose Emiliano Puga Saucedo  
**Matrícula:** 2430284  
**Versión:** 1.0  
**Fecha:** Noviembre 2025

---

## Descripcion General

Sistema de decodificacion en tiempo real del protocolo PRT-7, utilizado para descifrar telemetria de sensores industriales. El protocolo transmite instrucciones para ensamblar mensajes ocultos mediante un sistema de mapeo rotatorio. Este proyecto implementa la lectura serial desde una ESP32, decodificación de tramas y ensamblaje automático de mensajes cifrados mediante rotación de alfabeto.

---

## Caracteristicas Principales

- **Comunicacion Serial**: Lectura en tiempo real desde puerto COM (Arduino)
- **Decodificacion Dinamica**: Uso de rotor circular para mapeo de caracteres
- **Gestion de Memoria Manual**: Implementacion propia de listas enlazadas
- **Polimorfismo**: Jerarquia de clases para procesamiento de tramas
- **Deteccion Automatica**: Reconocimiento de patrones de inicio/fin sin marcadores

---

## Arquitectura del Sistema

### Componentes Principales

#### 1. Jerarquia de Clases (POO)

```
TramaBase (Clase Abstracta)
    |
    +-- TramaLoad  (Carga de datos)
    |
    +-- TramaMap   (Rotacion del rotor)
```

#### 2. Estructuras de Datos

- **RotorDeMapeo**: Lista circular doblemente enlazada (A-Z)
- **ListaDeCarga**: Lista doblemente enlazada para mensaje decodificado

#### 3. Comunicacion Serial

- **SerialPort**: Interfaz con puerto COM en Windows
- Configuracion: 9600 baudios, 8 bits, sin paridad

---

## Protocolo PRT-7

### Tipos de Tramas

#### Trama LOAD (L,X)

Carga un caracter para decodificacion:

```
L,H  -> Carga 'H'
L,A  -> Carga 'A'
L,   -> Carga espacio
```

#### Trama MAP (M,N)

Rota el rotor N posiciones:

```
M,2   -> Rota +2 posiciones (A->C, B->D...)
M,-2  -> Rota -2 posiciones (C->A, D->B...)
```

### Ejemplo de Secuencia

```
L,H      -> H decodificado como H
L,O      -> O decodificado como O
M,2      -> Rotar +2
L,K      -> K decodificado como M (con rotacion)
M,-2     -> Restaurar rotacion
```

**Resultado**: "HOM..."

---

## Requisitos del Sistema

### Software Necesario

- **Sistema Operativo**: Windows 7 o superior
- **Compilador**: MinGW (GCC para Windows)
- **CMake**: Version 3.10 o superior
- **Doxygen**: Para generar documentacion
- **Arduino IDE**: Para programar el emisor

### Hardware

- Arduino Uno/Nano (o compatible)
- Cable USB
- Puerto COM disponible

---

## Instrucciones de Compilacion

### 1. Clonar el repositorio

```bash
git clone <url-del-repositorio>
cd PRT7-Decoder
```

### 2. Crear directorio de compilacion

```bash
mkdir build
cd build
```

### 3. Generar archivos de compilacion

```bash
cmake .. -G "MinGW Makefiles"
```

### 4. Compilar

```bash
mingw32-make
```

### 5. Ejecutar

```bash
./PRT7Decoder.exe
```

---

## Generar Documentacion

Desde el directorio `build`:

```bash
cmake --build . --target docs
```

La documentacion se generara en: `docs/output/html/index.html`

---

## Uso del Sistema

### Paso 1: Programar Arduino

1. Abrir Arduino IDE
2. Cargar el archivo `arduino/emisor_prt7.ino`
3. Seleccionar puerto COM
4. Subir el codigo
5. Cerrar Arduino IDE

### Paso 2: Ejecutar Decodificador

1. Conectar Arduino
2. Ejecutar `PRT7Decoder.exe`
3. Ingresar puerto COM (ej: COM3)
4. Esperar la decodificacion automatica

### Salida Esperada

```
========================================
  Decodificador de Protocolo PRT-7
========================================

Ingrese el puerto COM (ej: COM3): COM3

Iniciando Decodificador PRT-7. Conectando a puerto COM3...
Conexion establecida. Esperando tramas...

Trama recibida: [L,H] -> Procesando... -> Fragmento 'H' decodificado como 'H'. Mensaje: [H]
Trama recibida: [L,O] -> Procesando... -> Fragmento 'O' decodificado como 'O'. Mensaje: [H][O]
...

---
MENSAJE OCULTO ENSAMBLADO:
[H][O][L][A][ ][M][U][N][D][O]
---
```

---

## Estructura del Proyecto

```
PRT7-Decoder/
├── src/
│   ├── main.cpp              # Programa principal
│   ├── TramaBase.h           # Clase base abstracta
│   ├── TramaLoad.h/cpp       # Trama de carga
│   ├── TramaMap.h/cpp        # Trama de mapeo
│   ├── RotorDeMapeo.h/cpp    # Rotor circular
│   ├── ListaDeCarga.h/cpp    # Lista de datos
│   └── SerialPort.h/cpp      # Comunicacion serial
├── arduino/
│   └── emisor_prt7.ino       # Codigo del Arduino
├── docs/
│   ├── Doxyfile              # Configuracion Doxygen
│   ├── mainpage.md           # Pagina principal de documentacion
│   └── output/               # Documentacion generada
├── build/                     # Archivos de compilacion
├── CMakeLists.txt            # Configuracion CMake
└── README.md                 # Instrucciones del profesor (NO MODIFICAR)
```

---

## Clases Principales

### TramaBase

Clase abstracta que define la interfaz para todas las tramas.

**Metodo virtual puro:**

```cpp
virtual void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) = 0;
```

### TramaLoad

Procesa tramas de tipo LOAD, decodificando caracteres.

**Metodos:**

- `TramaLoad(char c)`: Constructor
- `void procesar(...)`: Decodifica y almacena el caracter

### TramaMap

Procesa tramas de tipo MAP, rotando el rotor de decodificacion.

**Metodos:**

- `TramaMap(int n)`: Constructor con numero de rotacion
- `void procesar(...)`: Ejecuta la rotacion del rotor

### RotorDeMapeo

Lista circular con el alfabeto A-Z que mapea caracteres segun su rotacion.

**Metodos clave:**

- `void rotar(int n)`: Rota el rotor N posiciones
- `char getMapeo(char in)`: Devuelve el caracter mapeado

**Estructura interna:**

```cpp
struct NodoRotor {
    char letra;
    NodoRotor* siguiente;
    NodoRotor* previo;
};
```

### ListaDeCarga

Almacena los caracteres decodificados en orden de llegada.

**Metodos:**

- `void insertarAlFinal(char dato)`: Agrega un caracter al final
- `void imprimirMensaje()`: Muestra el mensaje completo

**Estructura interna:**

```cpp
struct NodoCarga {
    char dato;
    NodoCarga* siguiente;
    NodoCarga* previo;
};
```

### SerialPort

Maneja la comunicacion con el puerto serial en Windows.

**Metodos:**

- `SerialPort(const char* portName)`: Abre y configura el puerto
- `bool leerLinea(char* buffer, int size)`: Lee una linea completa
- `bool estaConectado()`: Verifica estado de conexion

---

## Algoritmo de Decodificacion

### Flujo Principal

1. **Inicializacion**

   - Crear RotorDeMapeo con alfabeto A-Z
   - Crear ListaDeCarga vacia
   - Conectar al puerto serial

2. **Bucle de Recepcion**

   ```
   MIENTRAS no se detecte fin:
       Leer linea del puerto serial
       Parsear tipo de trama (L o M)
       Crear objeto TramaLoad o TramaMap
       Ejecutar procesar()
       Liberar memoria del objeto trama
   ```

3. **Procesamiento de LOAD**

   ```
   Recibir caracter C
   Obtener C' = rotor.getMapeo(C)
   Insertar C' en listaCarga
   ```

4. **Procesamiento de MAP**

   ```
   Recibir numero N
   Ejecutar rotor.rotar(N)
   ```

5. **Deteccion de Fin**

   - Patron: Trama M con numero negativo
   - Condicion: Minimo 10 tramas procesadas

6. **Salida**
   - Imprimir mensaje decodificado
   - Liberar memoria
   - Cerrar puerto serial

---

## Diagrama de Flujo

```
[INICIO]
    |
    v
[Conectar Serial]
    |
    v
[Inicializar Rotor y Lista]
    |
    v
[Leer Trama] <-----------+
    |                    |
    v                    |
[Parsear Trama]          |
    |                    |
    +---> [Es LOAD?]     |
    |         |          |
    |         v          |
    |     [Decodificar]  |
    |         |          |
    +---> [Es MAP?]      |
              |          |
              v          |
          [Rotar]        |
              |          |
              v          |
          [Es Fin?] -----+
              |
              v (Si)
      [Imprimir Mensaje]
              |
              v
           [FIN]
```

---

## Gestion de Memoria

### Principios

- **Sin STL**: Implementacion manual de todas las estructuras
- **RAII**: Destructores liberan memoria automaticamente
- **Polimorfismo**: Destructor virtual en clase base

### Puntos Criticos

1. **Tramas dinamicas**

   ```cpp
   TramaBase* trama = new TramaLoad('A');
   trama->procesar(...);
   delete trama;  // IMPORTANTE: Liberar memoria
   ```

2. **Nodos de listas**

   ```cpp
   // En destructor de ListaDeCarga
   while (cabeza != nullptr) {
       NodoCarga* temp = cabeza;
       cabeza = cabeza->siguiente;
       delete temp;
   }
   ```

3. **Lista circular**
   ```cpp
   // Romper el circulo antes de eliminar
   ultimo->siguiente = nullptr;
   ```

---

## Manejo de Errores

### Conexion Serial

- Validacion de puerto COM
- Timeout en lectura
- Verificacion de estado

### Parseo de Tramas

- Validacion de formato
- Deteccion de tramas incompletas
- Manejo de caracteres especiales

### Robustez

- Reintentos en lectura
- Descarte de datos corruptos
- Logging de errores

---

## Pruebas y Validacion

### Casos de Prueba

1. **Mensaje Simple**: "HOLA"
2. **Mensaje con Espacios**: "HOLA MUNDO"
3. **Rotaciones Multiples**: M,5 seguido de M,-3
4. **Caracteres Especiales**: Numeros, simbolos

### Validacion de Salida

```
Entrada: L,H L,O L,L M,2 L,A
Salida esperada: [H][O][L][C]
```

---

## Optimizaciones

### Rendimiento

- Rotacion en O(n) donde n = desplazamiento
- Insercion en lista O(1) al final
- Busqueda en rotor O(n) donde n = posicion

### Memoria

- Nodos minimos: 26 (rotor) + caracteres del mensaje
- Sin copia de datos innecesaria
- Liberacion inmediata de tramas

---

## Problemas Comunes y Soluciones

### Puerto COM no abre

**Sintoma**: Error al conectar
**Solucion**:

- Verificar que Arduino este conectado
- Cerrar Arduino IDE
- Verificar permisos de administrador

### Mensaje corrupto

**Sintoma**: Caracteres incorrectos
**Solucion**:

- Verificar baud rate (9600)
- Revisar cable USB
- Resetear Arduino

### Memory Leak

**Sintoma**: Aumento de memoria
**Solucion**:

- Verificar delete de tramas
- Revisar destructores de listas

---

## Referencias

### Documentacion Tecnica

- [Comunicacion Serial Windows](https://docs.microsoft.com/en-us/windows/win32/devio/communications)
- [CMake Documentation](https://cmake.org/documentation/)
- [Doxygen Manual](https://www.doxygen.nl/manual/)

### Conceptos Aplicados

- Listas Doblemente Enlazadas
- Listas Circulares
- Polimorfismo en C++
- Gestion Manual de Memoria
- Protocolos de Comunicacion Serial

---

## Documentacion Doxygen Generada

La documentacion completa del proyecto ha sido generada automáticamente mediante **Doxygen** y está disponible en formato HTML.

### Ubicación de Archivos HTML

**Directorio:** `/home/emiliano/Documentos/ds-unidad-2-actividad-2-listasenlazadas-FabiRamiro/docs/output/html/`

### Archivos Principales Generados

| Archivo | Descripción |
|---------|------------|
| `index.html` | **Página de inicio** - Contiene introducción, autor y matrícula |
| `classes.html` | Listado completo de clases documentadas |
| `files.html` | Listado de archivos fuente (.h y .cpp) |
| `classSerialPort.html` | Documentación detallada de SerialPort |
| `classRotorDeMapeo.html` | Documentación detallada de RotorDeMapeo |
| `classListaDeCarga.html` | Documentación detallada de ListaDeCarga |
| `classTramaBase.html` | Documentación de clase abstracta TramaBase |
| `classTramaLoad.html` | Documentación de TramaLoad |
| `classTramaMap.html` | Documentación de TramaMap |

### Cómo Abrir la Documentación

**Desde terminal:**
```bash
firefox /home/emiliano/Documentos/ds-unidad-2-actividad-2-listasenlazadas-FabiRamiro/docs/output/html/index.html
```

**Desde explorador de archivos:**
```bash
nautilus /home/emiliano/Documentos/ds-unidad-2-actividad-2-listasenlazadas-FabiRamiro/docs/output/html/
```

**Total de archivos generados:** ~60+ archivos HTML con documentación completa

---

## Autor

**Jose Emiliano Puga Saucedo**  
**Matrícula:** 2430284

Universidad: Universidad Politecnica de Victoria
Materia: Estructuras de Datos
Fecha: Noviembre 2025

---

## Licencia

Proyecto academico desarrollado como parte del curso de Estructuras de Datos.

**Restricciones:**

- No usar STL (Standard Template Library)
- Implementacion manual de estructuras de datos
- Gestion explicita de memoria con new/delete
