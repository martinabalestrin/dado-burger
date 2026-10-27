#include <stdio.h>

int main() {
    Timer t;
    timer_iniciar(&t);
    
    // exemplo apenas para rodar processamento e contabilziar o tempo
    // sleep() não funciona pois não utiliza processamento
    for (volatile long i = 0; i < 1000000000L; i++);
    
    double segundos = timer_parar(&t);
    printf("Tempo: %.6f s\n", segundos);
}
