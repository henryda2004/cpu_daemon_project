/**
 * @brief Módulo de monitoreo de temperatura del CPU
 * @description Implementa funciones para obtener la temperatura actual del CPU
 *              utilizando el comando 'sensors' del sistema y parseando su salida
 *              para extraer la temperatura del controlador térmico (Tctl).
 * @author Sistema de monitoreo CPU
 * @dependencies Requiere lm-sensors instalado en el sistema Linux
 */

#include <stdio.h>      // Para FILE, fgets(), popen(), pclose()
#include <stdlib.h>     // Para funciones de utilidad del sistema
#include <string.h>     // Para strstr() - búsqueda de subcadenas
#include "temp_monitor.h" // Header con declaraciones del monitor de temperatura

/**
 * @brief Obtiene la temperatura actual del CPU del sistema
 * @description Esta función ejecuta el comando 'sensors' del sistema y parsea
 *              su salida para extraer la temperatura del controlador térmico (Tctl).
 *              Utiliza pipes para capturar la salida del comando y busca 
 *              específicamente la línea que contiene "Tctl:" con el formato
 *              "Tctl: +XX.X°C".
 * 
 * @return float Temperatura del CPU en grados Celsius
 *               - Valor positivo: Temperatura válida del CPU
 *               - -1.0: Error al ejecutar el comando sensors o al leer la temperatura
 *               - 0.0: No se encontró la línea Tctl o formato incorrecto
 * 
 * @details Proceso interno:
 *          1. Ejecuta 'sensors' usando popen() para capturar su salida
 *          2. Lee línea por línea la salida del comando
 *          3. Busca la línea que contiene "Tctl:" (Temperature Control)
 *          4. Extrae el valor numérico con formato "+XX.X°C"
 *          5. Retorna la temperatura como float
 * 
 * @note Formato esperado de sensors: "Tctl: +XX.X°C (high = +XX.X°C)"
 * @note Tctl = Temperature Control - sensor térmico principal en CPUs AMD
 * @note Para CPUs Intel, el formato puede variar (Core 0, Package id 0, etc.)
 * 
 * @warning Requiere que lm-sensors esté instalado y configurado
 * @warning La función es específica para el formato de salida de sensors en Linux
 * 
 * @example Uso típico:
 *          float current_temp = get_cpu_temp();
 *          if (current_temp > 0) {
 *              printf("CPU Temperature: %.1f°C\n", current_temp);
 *          } else {
 *              printf("Error reading CPU temperature\n");
 *          }
 */
float get_cpu_temp() {
    // Ejecutar comando 'sensors' y abrir pipe para leer su salida
    // popen(): crea un pipe y ejecuta el comando, retorna FILE* para leer
    // "r": modo lectura del pipe
    FILE *fp = popen("sensors", "r");
    
    // Verificar si el comando se ejecutó correctamente
    if (!fp) {
        // Error al ejecutar sensors - posiblemente no está instalado
        return -1;
    }

    // Buffer para almacenar cada línea de la salida de sensors
    // 256 caracteres es suficiente para las líneas típicas de sensors
    char line[256];
    
    // Variable para almacenar la temperatura extraída
    float temp = 0.0;

    // Leer la salida del comando línea por línea
    while (fgets(line, sizeof(line), fp)) {
        // Buscar la línea que contiene "Tctl:" (Temperature Control)
        // strstr(): busca la subcadena "Tctl:" en la línea actual
        // Tctl es el sensor de temperatura principal en procesadores AMD
        if (strstr(line, "Tctl:")) {
            // Parsear la línea para extraer el valor de temperatura
            // Formato esperado: "Tctl: +XX.X°C (high = +XX.X°C, crit = +XX.X°C)"
            // sscanf(): extrae el valor numérico después del signo +
            // "+%f°C": patrón que reconoce el signo + seguido de float y °C
            if (sscanf(line, "Tctl: +%f°C", &temp) == 1) {
                // Temperatura extraída exitosamente, salir del bucle
                break;
            }
        }
    }

    // Cerrar el pipe y liberar recursos
    // pclose(): cierra el pipe y espera a que termine el proceso hijo
    pclose(fp);
    
    // Retornar la temperatura obtenida
    // - Si se encontró Tctl: retorna la temperatura en °C
    // - Si no se encontró: retorna 0.0
    return temp;
}