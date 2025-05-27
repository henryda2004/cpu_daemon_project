/**
 * @brief Módulo para creación de procesos daemon (servicios del sistema)
 * @description Implementa la funcionalidad para convertir un proceso normal
 *              en un daemon del sistema siguiendo las mejores prácticas de Unix.
 *              Un daemon es un proceso que se ejecuta en segundo plano sin
 *              interacción directa con el usuario y sin terminal de control.
 * @author Sistema de monitoreo CPU
 * @standard POSIX/Unix daemon creation best practices
 */

#include <stdio.h>      // Para constantes de descriptores de archivo
#include <stdlib.h>     // Para exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h>     // Para fork(), setsid(), chdir(), close()
#include <sys/types.h>  // Para pid_t y tipos del sistema
#include <sys/stat.h>   // Para umask() - permisos de archivos
#include <fcntl.h>      // Para open() y constantes O_*
#include "daemon.h"     // Header con declaraciones del módulo daemon

/**
 * @brief Convierte el proceso actual en un daemon del sistema
 * @description Esta función implementa el proceso estándar de "daemonización"
 *              siguiendo las mejores prácticas de Unix. Realiza una serie de
 *              pasos críticos para desacoplar completamente el proceso del
 *              entorno de ejecución del usuario y convertirlo en un servicio
 *              del sistema que puede ejecutarse independientemente.
 * 
 * @return void Esta función no retorna (termina procesos padre) o convierte
 *              el proceso actual en daemon
 * 
 * @details Proceso de daemonización (pasos críticos):
 * 
 *          **PASO 1: Primer fork() - Separación del proceso padre**
 *          - Crea proceso hijo y termina el padre original
 *          - El hijo se convierte en huérfano, adoptado por init (PID 1)
 *          - Garantiza que el nuevo proceso no sea líder de grupo
 * 
 *          **PASO 2: setsid() - Creación de nueva sesión**
 *          - Crea nueva sesión y grupo de procesos
 *          - El proceso se convierte en líder de sesión
 *          - Se desasocia de cualquier terminal de control
 * 
 *          **PASO 3: Segundo fork() - Prevención de terminal de control**
 *          - Segundo fork para garantizar que nunca pueda adquirir terminal
 *          - El proceso ya no es líder de sesión
 *          - Proporciona aislamiento total del entorno de usuario
 * 
 *          **PASO 4: umask(0) - Limpieza de máscara de permisos**
 *          - Establece máscara de permisos a 0 (sin restricciones)
 *          - Permite control total sobre permisos de archivos creados
 * 
 *          **PASO 5: chdir("/") - Cambio a directorio raíz**
 *          - Cambia directorio de trabajo a raíz del sistema
 *          - Evita bloquear sistemas de archivos montados
 * 
 *          **PASO 6: Redirección de descriptores estándar**
 *          - Cierra stdin, stdout, stderr heredados
 *          - Los redirige a /dev/null para evitar E/S accidental
 *          - Previene errores por escritura a descriptores cerrados
 * 
 * @note Técnica de "double fork": Método estándar Unix para crear daemons
 * @note Después de esta función, el proceso es completamente independiente
 * @note No hay vuelta atrás - el proceso original y sus padres terminan
 * 
 * @warning Esta función termina los procesos padre - solo el daemon sobrevive
 * @warning Después de llamarla, no hay salida estándar disponible
 * @warning Errores en cualquier paso resultan en terminación del proceso
 * 
 * @example Uso típico al inicio del programa:
 *          ```c
 *          #include "daemon.h"
 *          
 *          int main() {
 *              // Convertir a daemon antes de lógica principal
 *              create_daemon();
 *              
 *              // A partir de aquí, el código se ejecuta como daemon
 *              while (1) {
 *                  // Lógica del servicio en segundo plano
 *                  perform_daemon_tasks();
 *                  sleep(INTERVAL);
 *              }
 *              
 *              return 0; // Nunca se alcanza en daemons típicos
 *          }
 *          ```
 * 
 * @see Stevens, W. Richard - "Advanced Programming in the UNIX Environment"
 * @see man 7 daemon - Manual de Linux sobre creación de daemons
 */
void create_daemon() {
    // PASO 1: Primer fork() - Crear proceso hijo y terminar padre
    // =========================================================
    // fork(): crea copia exacta del proceso actual
    // pid_t: tipo para almacenar IDs de proceso
    pid_t pid = fork();
    
    // Verificar error en fork()
    if (pid < 0) {
        // fork() falló - terminar con código de error
        exit(EXIT_FAILURE);
    }
    
    // Si somos el proceso padre (pid > 0), terminar
    if (pid > 0) {
        // El padre termina exitosamente, dejando al hijo huérfano
        // El hijo será adoptado por init (PID 1)
        exit(EXIT_SUCCESS);
    }
    
    // A partir de aquí, solo el proceso hijo continúa ejecutándose
    // El hijo ahora es huérfano y adoptado por init
    
    // PASO 2: setsid() - Crear nueva sesión y grupo de procesos
    // ========================================================
    // setsid(): crea nueva sesión, hace al proceso líder de sesión
    // Desasocia el proceso de cualquier terminal de control
    if (setsid() < 0) {
        // Error al crear nueva sesión - terminar
        exit(EXIT_FAILURE);
    }
    
    // PASO 3: Segundo fork() - Prevenir adquisición de terminal de control
    // ===================================================================
    // Segundo fork para garantizar que nunca pueda convertirse en
    // controlador de terminal (solo líderes de sesión pueden hacerlo)
    pid = fork();
    
    // Verificar error en segundo fork()
    if (pid < 0) {
        // Segundo fork() falló - terminar
        exit(EXIT_FAILURE);
    }
    
    // Si somos el segundo padre, terminar
    if (pid > 0) {
        // El segundo padre termina, dejando al nieto como daemon final
        exit(EXIT_SUCCESS);
    }
    
    // A partir de aquí, solo el proceso nieto (daemon final) continúa
    // Este proceso nunca podrá adquirir una terminal de control
    
    // PASO 4: umask(0) - Limpiar máscara de permisos de archivos
    // =========================================================
    // umask(0): establece máscara de creación de archivos a 0
    // Permite control total sobre permisos de archivos creados por el daemon
    // Sin esta llamada, los permisos heredados podrían causar problemas
    umask(0);
    
    // PASO 5: chdir("/") - Cambiar directorio de trabajo a raíz
    // =======================================================
    // chdir("/"): cambia directorio de trabajo al directorio raíz
    // Evita que el daemon mantenga ocupado algún directorio específico
    // Previene problemas al desmontar sistemas de archivos
    chdir("/");

    // PASO 6: Redirigir descriptores de archivo estándar a /dev/null
    // =============================================================
    // Los daemons no deben usar stdin, stdout, stderr heredados
    // Cerrarlos y redirigirlos previene errores y comportamientos inesperados
    
    // Cerrar descriptores estándar heredados del proceso original
    close(STDIN_FILENO);   // Cerrar entrada estándar (descriptor 0)
    close(STDOUT_FILENO);  // Cerrar salida estándar (descriptor 1)  
    close(STDERR_FILENO);  // Cerrar error estándar (descriptor 2)
    
    // Redirigir descriptores a /dev/null para evitar errores de E/S
    // /dev/null: dispositivo especial que descarta toda escritura
    open("/dev/null", O_RDONLY);  // stdin -> /dev/null (solo lectura)
    open("/dev/null", O_RDWR);    // stdout -> /dev/null (lectura/escritura)
    open("/dev/null", O_RDWR);    // stderr -> /dev/null (lectura/escritura)
    
    // ¡DAEMONIZACIÓN COMPLETA!
    // El proceso ahora es un daemon completamente funcional:
    // - Sin padre (adoptado por init)
    // - Sin terminal de control
    // - En nueva sesión y grupo de procesos
    // - Directorio de trabajo en raíz
    // - Descriptores estándar redirigidos
    // - Máscara de permisos limpia
}