/**
 * @brief Daemon de monitoreo de temperatura de CPU
 * @description Este programa implementa un daemon que monitorea continuamente 
 *              la temperatura del CPU, registra las lecturas en un archivo de 
 *              log y envía notificaciones cuando la temperatura excede el umbral
 * @author Sistema de monitoreo CPU
 */

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "daemon.h"
#include "temp_monitor.h"
#include "notifier.h"

// Configuración del daemon
#define INTERVAL 5          // Intervalo de monitoreo en segundos
#define TEMP_THRESHOLD 65.0 // Umbral de temperatura crítica en grados Celsius

/**
 * @brief Función principal del daemon de monitoreo de temperatura
 * @description Inicializa el daemon, abre el archivo de log y ejecuta el bucle
 *              principal de monitoreo que:
 *              1. Lee la temperatura actual del CPU
 *              2. Registra la temperatura en el archivo de log
 *              3. Verifica si excede el umbral crítico
 *              4. Envía notificación si es necesario
 *              5. Espera el intervalo definido antes de repetir
 * 
 * @return int Código de salida (0 = éxito, 1 = error)
 */
int main() {
    // Convertir el proceso en un daemon del sistema
    create_daemon();

    // Abrir archivo de log en modo append para registrar las temperaturas
    // Ruta: /home/henry/CLionProjects/cpu_daemon/logs/cpu_temp_log.txt
    FILE *log = fopen("/home/henry/CLionProjects/cpu_daemon/logs/cpu_temp_log.txt", "a");
    
    // Verificar si el archivo se abrió correctamente
    if (!log) {
        // Si no se puede abrir el archivo de log, terminar con código de error
        return 1;
    }

    // Bucle principal del daemon - ejecuta indefinidamente
    while (1) {
        // Obtener la temperatura actual del CPU
        float temp = get_cpu_temp();
        
        // Obtener timestamp actual para el registro
        time_t now = time(NULL);

        // Registrar la temperatura actual en el archivo de log
        // Formato: [timestamp] Temp: XX.XX°C
        fprintf(log, "[%s] Temp: %.2f°C\n", ctime(&now), temp);
        
        // Forzar la escritura inmediata al archivo (flush del buffer)
        fflush(log);

        // Verificar si la temperatura excede el umbral crítico
        if (temp >= TEMP_THRESHOLD) {
            // Enviar notificación de alerta por temperatura alta
            send_notification(temp);
        }

        // Pausar ejecución por el intervalo definido (5 segundos)
        sleep(INTERVAL);
    }

    // Cerrar archivo de log (nota: este código nunca se ejecuta debido al bucle infinito)
    fclose(log);
    
    // Retornar código de éxito
    return 0;
}