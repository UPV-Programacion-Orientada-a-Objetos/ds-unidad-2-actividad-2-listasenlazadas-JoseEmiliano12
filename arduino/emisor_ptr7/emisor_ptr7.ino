/**
 * Emisor de protocolo PRT-7 para Arduino/ESP32
 * Envia tramas de tipo LOAD y MAP por el puerto serial en bucle
 * Compatible con Linux y Windows
 */

void setup()
{
    // Inicializar comunicacion serial a 9600 baudios
    Serial.begin(9600);
    delay(2000); // Esperar a que se establezca la conexion
    
    // Mensaje inicial para verificar conexión
    Serial.println("INICIO");
}

void loop()
{
    // Mensaje: "HOLA MUNDO"
    // Envia continuamente cada 100ms
    
    Serial.println("L,H");
    delay(100);

    Serial.println("L,O");
    delay(100);

    Serial.println("L,L");
    delay(100);

    Serial.println("L,A");
    delay(100);

    Serial.println("L, "); // Espacio
    delay(100);

    Serial.println("M,2"); // Rotar +2
    delay(100);

    Serial.println("L,K"); // M con rotacion +2
    delay(100);

    Serial.println("L,S"); // U con rotacion +2
    delay(100);

    Serial.println("L,L"); // N con rotacion +2
    delay(100);

    Serial.println("L,B"); // D con rotacion +2
    delay(100);

    Serial.println("L,M"); // O con rotacion +2
    delay(100);

    Serial.println("M,-2"); // Regresar rotacion a 0 (PATRON DE FIN)
    delay(100);

    // Pausa larga entre ciclos para sincronizar con cliente
    delay(3000);
}
