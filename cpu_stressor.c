/**
 * @brief Programa de prueba de estrés para CPU
 * @description Este programa está diseñado para generar carga intensiva en el CPU
 *              mediante cálculos matemáticos continuos. Su propósito principal es
 *              probar sistemas de monitoreo de temperatura, benchmarking, o 
 *              verificar la estabilidad del sistema bajo carga sostenida.
 * @author Sistema de pruebas CPU
 * @warning Este programa causará uso intensivo del CPU y aumento de temperatura
 */

#include <stdio.h>  // Para funciones de entrada/salida estándar

/**
 * @brief Función principal que ejecuta prueba de estrés continua del CPU
 * @description Implementa un bucle infinito que realiza operaciones matemáticas
 *              intensivas para generar carga máxima en el procesador. Utiliza
 *              multiplicaciones en punto flotante repetitivas para mantener
 *              el CPU ocupado al 100% de su capacidad.
 * 
 * @return int Código de retorno (nunca se alcanza debido al bucle infinito)
 *             - 0: Terminación exitosa (teóricamente)
 * 
 * @details Algoritmo de estrés implementado:
 * 
 *          **🔥 Bucle Principal Infinito:**
 *          - while(1): Ejecuta indefinidamente hasta terminación forzada
 *          - No tiene condición de salida natural
 *          - Requiere Ctrl+C o kill para terminar
 * 
 *          **🧮 Cálculo Intensivo por Iteración:**
 *          - Variable x inicializada en 1.0 (punto flotante doble precisión)
 *          - 1,000,000 de multiplicaciones por x *= 1.000001
 *          - Cada multiplicación consume ciclos de CPU
 * 
 *          **📊 Características del Cálculo:**
 *          - Operación: Multiplicación acumulativa
 *          - Factor: 1.000001 (incremento mínimo para evitar overflow rápido)
 *          - Iteraciones: 1,000,000 por ciclo
 *          - Resultado: x ≈ e^(1000000 * ln(1.000001)) ≈ e^1000 por ciclo
 * 
 * @performance Impacto en el Sistema:
 *              - **Uso de CPU**: 100% en un núcleo (single-threaded)
 *              - **Temperatura**: Incremento significativo del CPU
 *              - **Consumo energético**: Máximo para el núcleo utilizado
 *              - **Duración**: Indefinida hasta terminación manual
 * 
 * @use_cases Casos de Uso Típicos:
 *            - **Pruebas de temperatura**: Verificar sistemas de cooling
 *            - **Benchmarking**: Medir rendimiento sostenido
 *            - **Pruebas de estabilidad**: Detectar fallos bajo carga
 *            - **Validación de monitoring**: Probar daemons de monitoreo
 *            - **Pruebas de throttling**: Verificar limitación térmica
 * 
 * @warnings ⚠️ Advertencias de Seguridad:
 *           - **Sobrecalentamiento**: Puede causar temperaturas peligrosas
 *           - **Throttling térmico**: El CPU puede reducir frecuencia
 *           - **Consumo energético**: Incremento significativo del consumo
 *           - **Ruido del ventilador**: Los ventiladores trabajarán al máximo
 *           - **Duración**: Solo ejecutar por períodos controlados
 * 
 * @termination Métodos de Terminación:
 *              - **Ctrl+C**: Señal SIGINT desde terminal
 *              - **kill PID**: Comando kill desde otra terminal
 *              - **killall programa**: Terminar por nombre del proceso
 *              - **Timeout**: Usar timeout command para límite temporal
 * 
 * @example Ejemplos de Ejecución:
 *          ```bash
 *          # Ejecución básica (cuidado - bucle infinito)
 *          ./cpu_stress_test
 *          
 *          # Ejecución con límite de tiempo (30 segundos)
 *          timeout 30s ./cpu_stress_test
 *          
 *          # Ejecución en segundo plano
 *          ./cpu_stress_test &
 *          
 *          # Terminar por PID
 *          kill $(pgrep cpu_stress_test)
 *          
 *          # Monitorear temperatura mientras ejecuta
 *          ./cpu_stress_test & sensors -f
 *          ```
 * 
 * @monitoring Monitoreo Recomendado:
 *             - **htop/top**: Verificar uso de CPU
 *             - **sensors**: Monitorear temperatura
 *             - **powertop**: Verificar consumo energético
 *             - **iostat**: Monitorear rendimiento general del sistema
 * 
 * @mathematical_analysis Análisis Matemático:
 *                        - Operación: x = x * 1.000001 (1M veces por ciclo)
 *                        - Resultado por ciclo: x ≈ e^1000 ≈ 1.97 × 10^434
 *                        - Overflow: Ocurrirá después de varios ciclos
 *                        - Comportamiento post-overflow: x = inf, luego NaN
 * 
 * @see htop para monitoreo de procesos
 * @see sensors para monitoreo de temperatura
 * @see timeout para ejecución con límite temporal
 */
int main() {
    // BUCLE PRINCIPAL INFINITO - Genera carga continua en el CPU
    // ========================================================
    // while(1): Bucle sin condición de terminación
    // Solo se puede interrumpir externamente (Ctrl+C, kill, etc.)
    while (1) {
        // INICIALIZACIÓN DE VARIABLE DE CÁLCULO
        // ====================================
        // double x: Variable de punto flotante de doble precisión
        // Inicializada en 1.0 para comenzar cada ciclo de cálculo
        // Uso de double (64-bit) en lugar de float para mayor precisión
        double x = 1.0;
        
        // BUCLE DE CÁLCULO INTENSIVO - 1,000,000 iteraciones
        // =================================================
        // for loop: Ejecuta exactamente 1,000,000 multiplicaciones
        // Cada iteración consume ciclos de CPU con operación aritmética
        for (int i = 0; i < 1000000; ++i) {
            // OPERACIÓN MATEMÁTICA INTENSIVA
            // =============================
            // x *= 1.000001: Multiplicación acumulativa
            // Factor 1.000001: Incremento muy pequeño para evitar overflow rápido
            // Esta operación mantiene al CPU ocupado realizando cálculos FPU
            // 
            // Progresión matemática:
            // Iteración 1: x = 1.0 * 1.000001 = 1.000001
            // Iteración 2: x = 1.000001 * 1.000001 = 1.000002000001
            // ...
            // Iteración 1M: x ≈ (1.000001)^1000000 ≈ e^1000 ≈ 1.97×10^434
            x *= 1.000001;
        }
        
        // FIN DEL CICLO DE CÁLCULO
        // =======================
        // Al completar 1M multiplicaciones, el bucle while(1) reinicia
        // Nueva iteración: x se reinicializa a 1.0 y el proceso se repite
        // Este patrón continúa indefinidamente, manteniendo el CPU ocupado
    }
    
    // CÓDIGO INALCANZABLE
    // ==================
    // Esta línea nunca se ejecuta debido al bucle infinito anterior
    // Solo se incluye para cumplir con la sintaxis de función main()
    // En un escenario real, el programa termina por señal externa
    return 0;
}