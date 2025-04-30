#include <stdio.h>
#include <sys/time.h>
#include <limits.h>

int main() {
    struct timeval tiempo_inicio, tiempo_actual;
    long segundos, microsegundos;
    long long total;

    printf("MAX LONG LONG %lld\n", LLONG_MAX);
    // Captura el tiempo de inicio
    gettimeofday(&tiempo_inicio, NULL);
    segundos = tiempo_inicio.tv_sec;
    microsegundos = tiempo_inicio.tv_usec;
    printf("Tiempo inicio: %ld.%06ld segundos\n", segundos, microsegundos);
    total = segundos * 1000000;
    total = total + microsegundos;
    printf("Tiempo inicio: %lld microsgundos\n", total);

    // Captura el tiempo actual
    gettimeofday(&tiempo_actual, NULL);
    
    // Calcula la diferencia
    segundos = tiempo_actual.tv_sec - tiempo_inicio.tv_sec;
    microsegundos = tiempo_actual.tv_usec - tiempo_inicio.tv_usec;
    
    // Ajuste si los microsegundos son negativos
    if (microsegundos < 0) {
        segundos--;
        microsegundos += 1000000;
    }
    
    printf("Tiempo transcurrido: %ld.%06ld segundos\n", segundos, microsegundos);
    
    return 0;
}