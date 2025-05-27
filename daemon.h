/**
 * @brief Header del módulo de creación de procesos daemon del sistema
 * @description Archivo de cabecera que define la interfaz para convertir
 *              procesos normales en daemons (servicios del sistema) siguiendo
 *              las mejores prácticas estándar de Unix/Linux. Este header
 *              proporciona acceso a la funcionalidad de daemonización completa.
 * @author Sistema de monitoreo CPU
 * @standard POSIX/Unix daemon creation standards
 */

// Guardas de inclusión para prevenir inclusiones múltiples del header
// Mecanismo estándar C para evitar redefiniciones durante la compilación
#ifndef DAEMON_H  // Si DAEMON_H no está definido previamente
#define DAEMON_H  // Definir DAEMON_H como macro de protección

/**
 * @brief Declaración de función para conversión de proceso a daemon
 * @description Esta función convierte el proceso actual en un daemon del sistema
 *              (servicio en segundo plano) siguiendo el protocolo estándar Unix
 *              de daemonización. Implementa la técnica "double fork" y realiza
 *              todos los pasos necesarios para crear un daemon completamente
 *              independiente del entorno de usuario.
 * 
 * @return void Esta función NO retorna al proceso original
 *              - Los procesos padre terminan durante la daemonización
 *              - Solo el daemon final continúa ejecutándose
 *              - En caso de error, el proceso termina con EXIT_FAILURE
 * 
 * @details Proceso de daemonización implementado:
 * 
 *          **🔀 Double Fork Technique:**
 *          - Primer fork(): Separación del proceso padre original
 *          - Segundo fork(): Prevención de adquisición de terminal
 * 
 *          **👑 Gestión de Sesiones:**
 *          - setsid(): Crea nueva sesión sin terminal de control
 *          - Desasociación completa del entorno de usuario
 * 
 *          **🔧 Configuración del Entorno:**
 *          - umask(0): Limpia máscara de permisos de archivos
 *          - chdir("/"): Establece directorio de trabajo en raíz
 * 
 *          **🔇 Redirección de E/S:**
 *          - stdin, stdout, stderr → /dev/null
 *          - Previene conflictos de entrada/salida
 * 
 * @behavior Comportamiento Post-Daemonización:
 *           - El proceso se ejecuta completamente en segundo plano
 *           - No tiene terminal de control asociada
 *           - Es adoptado por init (PID 1) como proceso padre
 *           - No puede ser interrumpido por señales de terminal (Ctrl+C)
 *           - Persiste hasta ser terminado explícitamente o reinicio del sistema
 * 
 * @requirements Requisitos del Sistema:
 *               - Sistema Unix/Linux compatible con POSIX
 *               - Permisos para crear procesos (fork)
 *               - Acceso de escritura a /dev/null
 *               - Capacidad de cambio de directorio de trabajo
 * 
 * @usage Patrones de uso recomendados:
 * 
 *        **Inicialización temprana:**
 *        ```c
 *        #include "daemon.h"
 *        
 *        int main() {
 *            // Convertir a daemon ANTES que cualquier otra lógica
 *            create_daemon();
 *            
 *            // Inicializar servicios del daemon
 *            initialize_logging();
 *            setup_signal_handlers();
 *            
 *            // Bucle principal del servicio
 *            while (service_running) {
 *                perform_service_tasks();
 *                sleep(SERVICE_INTERVAL);
 *            }
 *            
 *            return 0;
 *        }
 *        ```
 * 
 *        **Daemon con logging:**
 *        ```c
 *        int main() {
 *            create_daemon();
 *            
 *            // Abrir log después de daemonización
 *            FILE *log = fopen("/var/log/myservice.log", "a");
 *            
 *            while (1) {
 *                // Usar archivo de log en lugar de stdout
 *                fprintf(log, "Service running at %s\n", get_timestamp());
 *                fflush(log);
 *                
 *                do_service_work();
 *                sleep(60);
 *            }
 *        }
 *        ```
 * 
 *        **Daemon con manejo de señales:**
 *        ```c
 *        volatile sig_atomic_t keep_running = 1;
 *        
 *        void signal_handler(int sig) {
 *            keep_running = 0;
 *        }
 *        
 *        int main() {
 *            create_daemon();
 *            
 *            signal(SIGTERM, signal_handler);
 *            signal(SIGINT, signal_handler);
 *            
 *            while (keep_running) {
 *                perform_daemon_tasks();
 *                sleep(SERVICE_INTERVAL);
 *            }
 *            
 *            cleanup_and_exit();
 *            return 0;
 *        }
 *        ```
 * 
 * @warnings ⚠️ Advertencias Críticas:
 *           - **Irreversible**: No hay forma de "des-daemonizar" un proceso
 *           - **Sin stdout**: printf() y salida estándar no funcionarán
 *           - **Adopción por init**: El proceso será hijo de init (PID 1)
 *           - **Terminación de padres**: Procesos padre terminan permanentemente
 *           - **Debugging**: Más difícil debuggear procesos daemon
 * 
 * @best_practices 💡 Mejores Prácticas:
 *                  - Llamar create_daemon() lo más temprano posible en main()
 *                  - Configurar logging antes de lógica principal
 *                  - Implementar manejo de señales para terminación limpia
 *                  - Usar archivos de log en lugar de stdout/stderr
 *                  - Crear archivo PID para gestión del servicio
 *                  - Implementar reinicio automático si es necesario
 * 
 * @see daemon.c para la implementación completa de la daemonización
 * @see Stevens, W.R. "Advanced Programming in the UNIX Environment"
 * @see man 7 daemon para documentación del sistema sobre daemons
 * @see systemd para gestión moderna de servicios en Linux
 */
void create_daemon();

#endif // DAEMON_H - Fin de las guardas de inclusión