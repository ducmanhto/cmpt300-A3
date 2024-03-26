// Process Control Block
#ifndef _PCB_H_
#define _PCB_H_

typedef enum  
{
    READY,
    RUNNING,
    BLOCKED
} ProcessState;

typedef struct
{
    int senderPID;
    int recipientPID;
    char body[41];
} PROC_MSG;

typedef struct
{
    int pid;           
    int priority;       
    ProcessState state; 
    PROC_MSG* message;       
} PCB;

extern PCB* currentProcess;

PCB* PCB_create(int priority);
void PCB_print(PCB* process);
void PCB_free(PCB* pcb);

#endif