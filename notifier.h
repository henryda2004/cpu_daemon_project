/**
 * @brief Header del módulo de sistema de notificaciones
 * @description Archivo de cabecera que contiene las declaraciones de funciones
 *              para el sistema de notificaciones de alertas de temperatura del CPU.
 *              Este header define la interfaz pública del módulo notifier.c
 * @author Sistema de monitoreo CPU
 */

// Guardas de inclusión para prevenir inclusiones múltiples del header
// Evita errores de redefinición durante la compilación
#ifndef NOTIFIER_H  // Si NOTIFIER_H no está definido
#define NOTIFIER_H  // Definir NOTIFIER_H

/**
 * @brief Declaración de función para envío de notificaciones de temperatura crítica
 * @description Esta función envía una notificación visual del sistema cuando
 *              la temperatura del CPU excede los límites seguros establecidos.
 *              Utiliza el sistema de notificaciones nativo del escritorio Linux.
 * 
 * @param temp Temperatura actual del CPU en grados Celsius (float)
 *             - Valor típico de entrada: 65.0 - 100.0°C
 *             - Se muestra con precisión de 1 decimal en la notificación
 * 
 * @return void Esta función no retorna ningún valor
 * 
 * @dependencies 
 *               - Requiere notify-send instalado en el sistema
 *               - Necesita entorno gráfico activo para mostrar notificaciones
 * 
 * @usage Ejemplo de uso:
 *        ```c
 *        #include "notifier.h"
 *        
 *        float cpu_temperature = 72.3;
 *        send_notification(cpu_temperature);
 *        ```
 * 
 * @see notifier.c para la implementación completa de esta función
 */
void send_notification(float temp);

#endif // NOTIFIER_H - Fin de las guardas de inclusión