/**
 * @brief Módulo de sistema de notificaciones para alertas de temperatura
 * @description Implementa funciones para enviar notificaciones del sistema
 *              cuando la temperatura del CPU excede los límites seguros.
 *              Utiliza notify-send para mostrar alertas visuales en el escritorio.
 * @author Sistema de monitoreo CPU
 */

#include <stdlib.h>  // Para system() y funciones de utilidad del sistema
#include <stdio.h>   // Para snprintf() y funciones de E/S
#include "notifier.h" // Header con declaraciones de funciones del notificador

/**
 * @brief Envía una notificación visual del sistema sobre temperatura crítica
 * @description Esta función crea y ejecuta un comando notify-send para mostrar
 *              una alerta visual en el escritorio del usuario cuando la temperatura
 *              del CPU excede el umbral seguro. La notificación incluye un icono
 *              de advertencia y la temperatura actual.
 * 
 * @param temp Temperatura actual del CPU en grados Celsius
 * 
 * @details Funcionamiento interno:
 *          1. Crea un buffer para el comando del sistema
 *          2. Formatea el comando notify-send con la temperatura
 *          3. Ejecuta el comando usando system()
 * 
 * @note Requiere que notify-send esté instalado en el sistema
 * @note El comando se ejecuta en el contexto del usuario actual
 * 
 * @example Uso típico:
 *          float cpu_temp = 70.5;
 *          send_notification(cpu_temp);
 *          // Resultado: Notificación "⚠️ CPU ALERT - Temp: 70.5°C exceeds safe limit!"
 */
void send_notification(float temp) {
    // Buffer para almacenar el comando completo del sistema
    // Tamaño: 128 caracteres (suficiente para el comando notify-send)
    char command[128];
    
    // Construir el comando notify-send con formato personalizado
    // notify-send: utilidad de Linux para mostrar notificaciones de escritorio
    // Parámetros:
    // - Título: '⚠️ CPU ALERT' (con emoji de advertencia)
    // - Mensaje: 'Temp: XX.X°C exceeds safe limit!' (temperatura con 1 decimal)
    snprintf(command, sizeof(command),
             "notify-send '⚠️ CPU ALERT' 'Temp: %.1f°C exceeds safe limit!'", temp);
    
    // Ejecutar el comando del sistema para mostrar la notificación
    // system(): ejecuta el comando en el shell del sistema
    // La notificación aparecerá como popup en el escritorio del usuario
    system(command);
}