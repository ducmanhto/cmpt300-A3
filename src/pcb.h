// Process Control Block
#ifndef _PCB_H_
#define _PCB_H_

typedef enum  
{
    READY,
    RUNNING,
    BLOCKED
} ProcessState;

typedef struct PCB_s PCB;
struct PCB
{
    int pid;           
    int priority;       
    ProcessState state; 
    char message[41];       
};

#endif