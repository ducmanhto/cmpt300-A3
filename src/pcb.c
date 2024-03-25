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

    return pcb;
}