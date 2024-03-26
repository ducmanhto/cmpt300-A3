// pcb.c
#include "pcb.h"
#include <stdlib.h>
#include <stdio.h>

static int nextPID = 1;

PCB* PCB_create(int priority)
{
    PCB* pcb = (PCB*)malloc(sizeof(PCB));
    if (pcb == NULL)
    {
        printf("PCB_create: Failed to create new PCB!");
        return NULL;
    }

    pcb->pid = nextPID++;
    pcb->priority = priority;
    pcb->state = READY;

    return pcb;
}

const char* processStateToString(ProcessState state) {
    switch (state) {
        case RUNNING:
            return "RUNNING";
        case READY:
            return "READY";
        case BLOCKED:
            return "BLOCKED";
        default:
            return "UNKNOWN";
    }
}

void PCB_print(PCB* process)
{
    if (process == NULL)
    {
        printf("Process is NULL.\n");
    }

    printf("\tProcess: %d, Priority: %d, State: %s\n", process->pid, process->priority, processStateToString(process->state));
}

void PCB_free(PCB* process) {
    if (process == NULL)
    {
        return;
    }

    free(process);
}

