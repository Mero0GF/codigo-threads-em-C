#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_CADEIRAS 10
#define NUM_CLIENTES 15

pthread_mutex_t mutex_cadeiras;
int cadeiras_disponiveis[NUM_CADEIRAS];

void* atendimento(void* arg) {
    int cliente_id = *(int*) arg;
    int cadeira_vendida = -1;
    int tempo_escolha = rand() % 9 + 1;
    sleep(tempo_escolha);
    pthread_mutex_lock(&mutex_cadeiras);
    for (int i = 0; i < NUM_CADEIRAS; i++) {
        if (cadeiras_disponiveis[i] == 1) {
            cadeiras_disponiveis[i] = 0;
            cadeira_vendida = i + 1;
            break;
        }
    }
    pthread_mutex_unlock(&mutex_cadeiras);
    if (cadeira_vendida == -1) {
        printf("Cliente %d ficou sem cadeira.\n", cliente_id);
    } else {
        printf("Cliente %d comprou a cadeira %d.\n", cliente_id, cadeira_vendida);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_mutex_init(&mutex_cadeiras, NULL);
    for (int i = 0; i < NUM_CADEIRAS; i++) {
        cadeiras_disponiveis[i] = 1;
    }
    pthread_t clientes[NUM_CLIENTES];
    int ids[NUM_CLIENTES];
    for (int i = 0; i < NUM_CLIENTES; i++) {
        ids[i] = i + 1;
        pthread_create(&clientes[i], NULL, atendimento, &ids[i]);
    }
    for (int i = 0; i < NUM_CLIENTES; i++) {
        pthread_join(clientes[i], NULL);
    }
    pthread_mutex_destroy(&mutex_cadeiras);
    return 0;
}
