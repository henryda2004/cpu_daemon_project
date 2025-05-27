/**
 * @brief Header del módulo de monitoreo de temperatura del CPU
 * @description Archivo de cabecera que contiene las declaraciones de funciones
 *              para el monitoreo de temperatura del CPU. Este header define la
 *              interfaz pública del módulo temp_monitor.c que utiliza el comando
 *              'sensors' para obtener lecturas de temperatura en tiempo real.
 * @author Sistema de monitoreo CPU
 */

// Guardas de inclusión para prevenir inclusiones múltiples del header
// Mecanismo estándar para evitar errores de redefinición durante la compilación
#ifndef TEMP_MONITOR_H  // Si TEMP_MONITOR_H no está definido
#define TEMP_MONITOR_H  // Definir TEMP_MONITOR_H como macro

/**
 * @brief Declaración de función para obtener la temperatura actual del CPU
 * @description Esta función obtiene la temperatura actual del procesador
 *              ejecutando el comando 'sensors' del sistema y parseando su salida
 *              para extraer el valor de temperatura del controlador térmico (Tctl).
 * 
 * @return float Temperatura del CPU en grados Celsius
 *               - Valor positivo (ej: 45.5): Temperatura válida del CPU
 *               - -1.0: Error al ejecutar el comando sensors o leer datos
 *               - 0.0: No se encontró el sensor Tctl o formato incorrecto
 * 
 * @details Comportamiento de la función:
 *          - Ejecuta 'sensors' usando pipes del sistema
 *          - Busca líneas que contengan "Tctl:" (Temperature Control)
 *          - Extrae valores con formato "Tctl: +XX.X°C"
 *          - Maneja errores de ejecución y formato
 * 
 * @dependencies Sistema requerido:
 *               - Linux con lm-sensors instalado
 *               - Comando 'sensors' accesible desde PATH
 *               - Sensores de temperatura configurados
 * 
 * @compatibility 
 *                - CPUs AMD: Utiliza sensor "Tctl" (Temperature Control)
 *                - CPUs Intel: Puede requerir adaptación del formato
 *                - Sistemas Linux: Probado con lm-sensors estándar
 * 
 * @performance 
 *              - Tiempo de ejecución: ~50-100ms (depende del sistema)
 *              - Uso de memoria: Mínimo (buffer de 256 chars)
 *              - Operación: No bloqueante para el sistema
 * 
 * @usage Ejemplos de uso típicos:
 *        ```c
 *        #include "temp_monitor.h"
 *        
 *        // Lectura básica de temperatura
 *        float temp = get_cpu_temp();
 *        
 *        // Verificación con manejo de errores
 *        if (temp > 0) {
 *            printf("CPU: %.1f°C\n", temp);
 *        } else if (temp == -1.0) {
 *            printf("Error: sensors no disponible\n");
 *        } else {
 *            printf("Error: sensor Tctl no encontrado\n");
 *        }
 *        
 *        // Uso en bucle de monitoreo
 *        while (monitoring) {
 *            float current_temp = get_cpu_temp();
 *            if (current_temp >= TEMP_THRESHOLD) {
 *                trigger_alert(current_temp);
 *            }
 *            sleep(MONITOR_INTERVAL);
 *        }
 *        ```
 * 
 * @see temp_monitor.c para la implementación completa de esta función
 * @see lm-sensors documentación para configuración de sensores
 */
float get_cpu_temp();

#endif // TEMP_MONITOR_H - Fin de las guardas de inclusión