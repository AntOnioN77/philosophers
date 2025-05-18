#!/bin/bash

# Configuración
PROGRAMA="../philosophers"        # Ruta a tu programa en C
TIMEOUT_LOGS=1                  # Tiempo sin logs para considerar que está bloqueado (en segundos)
TIMEOUT_EJECUCION=2             # Tiempo máximo de ejecución antes de pasar al siguiente input
LOG_FILE="ejecucion.log"        # Archivo donde guardar resultados del script

echo "Iniciando pruebas del programa..." > $LOG_FILE

# Función para monitorear logs y detectar bloqueos
ejecutar_y_monitorear() {
    args="$@"
    echo "Ejecutando con argumentos: $args" | tee -a $LOG_FILE
    
    # Crear archivo temporal para logs
    temp_log=$(mktemp)
    
    # Ejecutar el programa en segundo plano y redirigir salida
    $PROGRAMA $args > "$temp_log" 2>&1 &
    pid=$!
    
    start_time=$(date +%s)
    last_log_time=$start_time
    last_size=0
    
    # Monitorear logs y detectar bloqueos
    while kill -0 $pid 2>/dev/null; do
        current_time=$(date +%s)
        current_size=$(stat -c %s "$temp_log")
        
        # Verificar si hay nuevos logs
        if [ "$current_size" -gt "$last_size" ]; then
            last_log_time=$current_time
            last_size=$current_size
        fi
        
        # Verificar si hay bloqueo (sin logs durante TIMEOUT_LOGS segundos)
        if [ $((current_time - last_log_time)) -ge $TIMEOUT_LOGS ]; then
            echo "BLOQUEADO: Sin logs durante $TIMEOUT_LOGS segundos" | tee -a $LOG_FILE
            kill -9 $pid
            wait $pid 2>/dev/null
            echo "Proceso terminado por bloqueo" | tee -a $LOG_FILE
            # cat "$temp_log" >> $LOG_FILE  # <- Comentado para no guardar la salida del binario
            rm "$temp_log"
            return 1
        fi
        
        # Verificar si excedió el tiempo máximo total
        if [ $((current_time - start_time)) -ge $TIMEOUT_EJECUCION ]; then
            echo "TIMEOUT: Superó el tiempo máximo de ejecución ($TIMEOUT_EJECUCION segundos)" | tee -a $LOG_FILE
            kill -9 $pid
            wait $pid 2>/dev/null
            echo "Proceso terminado por timeout (comportamiento normal)" | tee -a $LOG_FILE
            # cat "$temp_log" >> $LOG_FILE  # <- Comentado para no guardar la salida del binario
            rm "$temp_log"
            return 0
        fi
        
        sleep 0.1
    done
    
    # Si el programa terminó por sí solo
    echo "FINALIZADO: El programa terminó correctamente" | tee -a $LOG_FILE
    # cat "$temp_log" >> $LOG_FILE  # <- Comentado para no guardar la salida del binario
    rm "$temp_log"
    return 0
}

# Lista de combinaciones de argumentos para probar
declare -a ARGUMENTOS=(
    "100 26 10 10"
    "100 26 10 10 1"
    "101 606 200 150"
    "100 26 10 3"
    "100 19 3 10"
    "101 19 10 3"
    "150 250 350 450"
    "500 400 300 200"
    "100 200 300 400"
)

for args in "${ARGUMENTOS[@]}"; do
    for ((i=1; i<=1000; i++)); do
        ejecutar_y_monitorear $args
        sleep 1  # Pequeña pausa entre ejecuciones
    done
done

echo "Pruebas completadas. Resultados en $LOG_FILE"