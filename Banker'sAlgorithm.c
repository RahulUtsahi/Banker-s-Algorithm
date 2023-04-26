#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_RESOURCES 5

typedef struct {
    int allocated[MAX_RESOURCES];
    int max_required[MAX_RESOURCES];
    int need[MAX_RESOURCES];
    bool finished;
} process_state;

int n_resources;
int available[MAX_RESOURCES];

pthread_mutex_t lock_resources;
pthread_cond_t condition;

bool is_safe_state(process_state *states, int n_processes);
int request_resources(process_state *state, int *request);
int release_resources(process_state *state, int *release);
void *process_code(void *arg);

int main(int argc, char **argv) {
    srand(time(NULL));

    printf("\nNumber of processes? ");
    int n_processes;
    scanf("%d", &n_processes);

    printf("\nNumber of resources? ");
    scanf("%d", &n_resources);

    printf("\nCurrently Available resources (R1 R2 ...)? ");
    for (int i = 0; i < n_resources; i++) {
        scanf("%d", &available[i]);
    }

    process_state states[n_processes];
    for (int i = 0; i < n_processes; i++) {
        printf("\nResource allocated to process %d (R1 R2 ...)? ", i + 1);
        for (int j = 0; j < n_resources; j++) {
            scanf("%d", &states[i].allocated[j]);
        }
    }

    for (int i = 0; i < n_processes; i++) {
        printf("\nMaximum resource required by process %d (R1 R2 ...)? ", i + 1);
        for (int j = 0; j < n_resources; j++) {
            scanf("%d", &states[i].max_required[j]);
            states[i].need[j] = states[i].max_required[j] - states[i].allocated[j];
        }
        states[i].finished = false;
    }

    if (!is_safe_state(states, n_processes)) {
        printf("\nUnsafe State! The processes leads the system to an unsafe state.\n\n");
        exit(-1);
    }

    printf("\n\nSafe Sequence Found : ");
    for (int i = 0; i < n_processes; i++) {
        printf("%-3d", i + 1);
    }

    printf("\nExecuting Processes...\n\n");
    sleep(1);

    // run threads
    pthread_t processes[n_processes];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int process_number[n_processes];
    for (int i = 0; i < n_processes; i++) {
        process_number[i] = i;
        pthread_create(&processes[i], &attr, process_code, (void *)(&process_number[i]));
    }

    for (int i = 0; i < n_processes; i++) {
        pthread_join(processes[i], NULL);
    }

    printf("\nAll Processes Finished\n");

    return 0;
}

bool is_safe_state(process_state *states, int n_processes) {
    int work[n_resources];
    for (int i = 0; i < n_resources; i++) {
        work[i] = available[i];
    }

    bool finish[n_processes];
    for (int i = 0; i < n_processes; i++) {
        // process code
        void* processCode(void *arg) {
        int p = *((int *) arg);

        // lock resources
        pthread_mutex_lock(&lockResources);

        // condition check
        while(p != safeSeq[nProcessRan])
            pthread_cond_wait(&condition, &lockResources);

        printf("\n--> Process %d", p+1);
        printf("\n\tAllocated : ");
        for(int i=0; i<nResources; i++)
            printf("%3d", allocated[p][i]);

        printf("\n\tNeeded    : ");
        for(int i=0; i<nResources; i++)
            printf("%3d", need[p][i]);

        printf("\n\tAvailable : ");
        for(int i=0; i<nResources; i++)
            printf("%3d", resources[i]);

        printf("\n"); sleep(1);

        printf("\tResource Allocated!");
        printf("\n"); sleep(1);
        printf("\tProcess Code Running...");
        printf("\n"); sleep(rand()%3 + 2); // process code
        printf("\tProcess Code Completed...");
        printf("\n"); sleep(1);
        printf("\tProcess Releasing Resource...");
        printf("\n"); sleep(1);

        // release resources
        for(int i=0; i<nResources; i++)
            resources[i] += allocated[p][i];

        printf("\tResource Released!");
        printf("\n");

        printf("\tNew Available : ");
        for(int i=0; i<nResources; i++)
            printf("%3d", resources[i]);

        printf("\n\n");

        // update nProcessRan
        nProcessRan++;
        pthread_cond_broadcast(&condition);
        pthread_mutex_unlock(&lockResources);
        pthread_exit(NULL);
        }
    }
}


