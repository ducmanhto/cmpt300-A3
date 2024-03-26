#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "pcb.h"
#include "queues.h"
#include "list.h"
#include "semaphore.h"

extern List* highPriorityReadyQueue;
extern List* normalPriorityReadyQueue;
extern List* lowPriorityReadyQueue;
extern List* waitingOnSendQueue;
extern List* waitingOnReceiveQueue;

// extern Semaphore semaphores[5];

PCB* currentProcess = NULL;
PCB* initProcess = NULL;

void selectNextProcessToRun() 
{
    if (List_count(highPriorityReadyQueue) != 0) {
        currentProcess = (PCB*)List_remove(highPriorityReadyQueue);
    } else if (List_count(normalPriorityReadyQueue) != 0) {
        currentProcess = (PCB*)List_remove(normalPriorityReadyQueue);
    } else if (List_count(lowPriorityReadyQueue) != 0) {
        currentProcess = (PCB*)List_remove(lowPriorityReadyQueue);
    } else {
        currentProcess = initProcess;
    }

    if (currentProcess != NULL) {
        currentProcess->state = RUNNING;
    }
    printf("Currently running process: Process (%d) with priority (%d)\n", currentProcess->pid, currentProcess->priority);
}

void initializeInitProcess();
void handleCreateCommand();
void handleForkCommand();
void handleKillCommand();
void handleExitCommand();
void handleQuantumCommand();
void handleSendCommand();
void handleReceiveCommand();
void handleReplyCommand();
void handleNewSemaphoreCommand();
void handleSemaphorePCommand();
void handleSemaphoreVCommand();
void handleProcinfoCommand();
void handleTotalinfoCommand();

int main() {
    char command;

    initializeQueues();
    initializeInitProcess();

    while (1)
    {
        printf("\nOperating System Simulation Command Menu\n");
        printf("---------------------------------------\n");
        printf("Create (C) - Create a new process\n");
        printf("Fork (F) - Copy the currently running process\n");
        printf("Kill (K) - Kill a process\n");
        printf("Exit (E) - Kill the currently running process\n");
        printf("Quantum (Q) - Time quantum of running process expires\n");
        printf("Send (S) - Send a message to another process\n");
        printf("Receive (R) - Receive a message - block until 1 arrives\n");
        printf("Reply (Y) - Unblocks sender and delivers reply\n");
        printf("New Semaphore (N) - Initialize the named semaphore with the value given\n");
        printf("Semaphore P (P) - P operation\n");
        printf("Semaphore V (V) - V operation\n");
        printf("Procinfo (I) - Display information of process to screen\n");
        printf("Totalinfo (T) - Display all process queues\n");
        printf("---------------------------------------\n");
        printf("Enter command: ");
        
        scanf(" %c", &command);

        switch (command)
        {
            case 'C':
                handleCreateCommand();
                break;
            case 'F':
                handleForkCommand();
                break;
            case 'K':
                handleKillCommand();
                break;
            case 'E':
                handleExitCommand();
                break;
            case 'Q':
                handleQuantumCommand();
                break;
            case 'S':
                handleSendCommand();
                break;
            case 'R':
                handleReceiveCommand();
                break;
            case 'Y':
                handleReplyCommand();
                break;
            case 'N':
                handleNewSemaphoreCommand();
                break;
            case 'P':
                handleSemaphorePCommand();
                break;
            case 'V':
                handleSemaphoreVCommand();
                break;
            case 'I':
                handleProcinfoCommand();
                break;
            case 'T':
                handleTotalinfoCommand();
                break;
            default:
                printf("Unkown command.\n");
        }
    }
    return 0;
}


void initializeInitProcess()
{
    initProcess = (PCB*)malloc(sizeof(PCB));
    if (!initProcess)
    {
        fprintf(stderr, "Failed to allocate memory for the init process\n");
        exit(EXIT_FAILURE);
    }

    initProcess->pid = 0;
    initProcess->state = RUNNING;
    currentProcess = initProcess;
}
void handleCreateCommand()
{
    int priority;
    printf("Enter the priority of the new process (0=high, 1=normal, 2=low): ");
    scanf("%d", &priority);

    // Validate the priority input
    if (priority < 0 || priority > 2)
    {
        printf("Invalid priority. Please enter a value between 0 and 2.\n");
        printf("Failed to create a new process.\n");
        return ;
    }

    PCB* newProcess = PCB_create(priority);
    if (newProcess == NULL)
    {
        printf("Failed to create a new process.\n");
    }

    int addedProcessResult = addProcessToReadyQueue(newProcess);
    if (addedProcessResult == -1)
    {
        printf("Failed to add the new process to the ready queue.\n");
        return;
    } else if (addedProcessResult == 0) 
    {
        printf("New process created successfully with PID: %d.\n", newProcess->pid);
    }

    if (currentProcess == initProcess) {
        selectNextProcessToRun();
    }
}
void handleForkCommand()
{
    if (currentProcess == initProcess)
    {
        printf("Failed to copy the init process.\n");
        return;
    }

    PCB* newProcess = PCB_create(currentProcess->priority);
    if (!newProcess)
    {
        printf("Failed to fork the current process.\n");
        return;
    }

    int addedProcessResult = addProcessToReadyQueue(newProcess);
    if (addedProcessResult == -1)
    {
        printf("Failed to add the new process to the ready queue.\n");
        free(newProcess);
        return;
    }

    printf("Process forked successfully with PID %d and priority %d\n", newProcess->pid, newProcess->priority);
}

bool findAndRemoveProcessByPID(int pid)
{
    for (PCB* pcb = List_first(highPriorityReadyQueue); pcb != NULL; pcb = List_next(highPriorityReadyQueue)) {
        if (pcb->pid == pid) {
            List_remove(highPriorityReadyQueue);
            PCB_free(pcb);
            return true;
        }
    }

    for (PCB* pcb = List_first(normalPriorityReadyQueue); pcb != NULL; pcb = List_next(normalPriorityReadyQueue)) {
        if (pcb->pid == pid) {
            List_remove(normalPriorityReadyQueue);
            PCB_free(pcb);
            return true;
        }
    }

    for (PCB* pcb = List_first(lowPriorityReadyQueue); pcb != NULL; pcb = List_next(lowPriorityReadyQueue)) {
        if (pcb->pid == pid) {
            List_remove(lowPriorityReadyQueue);
            PCB_free(pcb);
            return true;
        }
    }

    return false;
}

void handleKillCommand()
{
    int pid;
    printf("Enter the pid of the process to be killed: ");
    scanf("%d", &pid);

    if (pid == 0)
    {
        if (List_count(highPriorityReadyQueue) != 0 || List_count(normalPriorityReadyQueue) != 0 || List_count(lowPriorityReadyQueue) != 0)
        {
            printf("Init process can't be killed unless it is the last process in the system!\n");
            return;
        } else 
        {
            PCB_free(initProcess);
            exit(0);
        }
    }

    if (currentProcess != NULL && currentProcess->pid == pid)
    {
        PCB_free(currentProcess);
        printf("Process with PID %d has been killed.\n", pid);
        selectNextProcessToRun();
        return;
    }

    if (findAndRemoveProcessByPID(pid)) {
        printf("Process with PID %d has been killed.\n", pid);
    } else {
        printf("No process found with PID %d.\n", pid);
    }
}

void handleExitCommand()
{
    if (currentProcess == initProcess) {
        PCB_free(initProcess);
        exit(0);
    }

    int currentPID = currentProcess->pid;
    PCB_free(currentProcess);
    printf("Process with PID %d has been killed.\n", currentPID);
    selectNextProcessToRun();
}

void handleQuantumCommand()
{
    if (currentProcess != NULL && currentProcess != initProcess)
    {
        currentProcess->state = READY;
        PCB* temp = currentProcess;

        selectNextProcessToRun();
        int addedProcessResult = addProcessToReadyQueue(temp);
        if (addedProcessResult == -1)
        {
            printf("Failed to add the current process to the ready queue.\n");
            return;
        } else if (addedProcessResult == 0) 
        {
            printf("Process with PID %d preempted and moved back to ready queue.\n", temp->pid);
        }
    } else {
        printf("No process is currently running or only init process is running!\n");
    }
}

PCB* PCB_find(int pid)
{
    if (pid == 0) {
        return initProcess;
    }

    if (currentProcess != NULL && currentProcess->pid == pid)
    {
        return currentProcess;
    }

    for (PCB* pcb = List_first(highPriorityReadyQueue); pcb != NULL; pcb = List_next(highPriorityReadyQueue)) {
        if (pcb->pid == pid) {
            return pcb;
        }
    }

    for (PCB* pcb = List_first(normalPriorityReadyQueue); pcb != NULL; pcb = List_next(normalPriorityReadyQueue)) {
        if (pcb->pid == pid) {
            return pcb;
        }
    }

    for (PCB* pcb = List_first(lowPriorityReadyQueue); pcb != NULL; pcb = List_next(lowPriorityReadyQueue)) {
        if (pcb->pid == pid) {
            return pcb;
        }
    }

    for (PCB* pcb = List_first(waitingOnSendQueue); pcb != NULL; pcb = List_next(waitingOnSendQueue)) {
        if (pcb->pid == pid) {
            return pcb;
        }
    }

    for (PCB* pcb = List_first(waitingOnReceiveQueue); pcb != NULL; pcb = List_next(waitingOnReceiveQueue)) {
        if (pcb->pid == pid) {
            return pcb;
        }
    }

    return NULL;
}

void handleSendCommand()
{
    int recipientPID;
    printf("Enter the pid of the receipient process: ");
    scanf("%d", &recipientPID);
    while (getchar() != '\n');

    char messageContent[41];
    printf("Enter the message to send (max 40 characters): ");
    if (fgets(messageContent, sizeof(messageContent), stdin) == NULL) {
        printf("Error reading message.\n");
        return;
    }
    messageContent[strcspn(messageContent, "\n")] = 0;

    PROC_MSG* newMessage = (PROC_MSG*)malloc(sizeof(PROC_MSG));
    if (newMessage == NULL) {
        fprintf(stderr, "Error: Memory allocation for newMessage failed.\n");
        return;
    }

    int senderPID = currentProcess->pid;

    newMessage->senderPID = senderPID;
    newMessage->recipientPID = recipientPID;
    strncpy(newMessage->body, messageContent, sizeof(newMessage->body) - 1);
    newMessage->body[sizeof(newMessage->body) - 1] = '\0'; // Ensure null-termination

    PCB* recipientProccess = PCB_find(recipientPID);

    if (recipientProccess->message != NULL)
    {
        printf("A process can only receive 1 message at a time.\n");
        printf("Failed to send the message to process %d\n", recipientPID);
        return;
    }

    recipientProccess->message = newMessage;

    printf("Successfully sent a message from Process %d to Process %d\n", senderPID, recipientPID);

    if (recipientProccess->state == BLOCKED)
    {
        recipientProccess->state = READY;
        for (PCB* pcb = List_first(waitingOnReceiveQueue); pcb != NULL; pcb = List_next(waitingOnReceiveQueue)) {
            if (pcb->pid == recipientPID) {
                List_remove(waitingOnReceiveQueue);
            }
        }
        addProcessToReadyQueue(recipientProccess);
    }

    if (currentProcess != initProcess) {
        currentProcess->state = BLOCKED;
        List_append(waitingOnSendQueue, currentProcess);
        selectNextProcessToRun();
    }
}

void handleReceiveCommand()
{
    if (currentProcess->message != NULL)
    {
        printf("There is a message waiting for the currently executing process\n");
        printf("Source of message: Process %d\n", currentProcess->message->senderPID);
        printf("Message: %s\n", currentProcess->message->body);
        currentProcess->message = NULL;
    } else {
        printf("No message waiting for the currently executing process!\n");
        currentProcess->state = BLOCKED;
        List_append(waitingOnReceiveQueue, currentProcess);
        selectNextProcessToRun();
    }
}

void handleReplyCommand()
{
    int pid;
    printf("Enter the PID of the process to make the reply to: ");
    scanf("%d", &pid);

    while (getchar() != '\n');

    char messageContent[41];
    printf("Enter the message to reply (max 40 characters): ");
    if (fgets(messageContent, sizeof(messageContent), stdin) == NULL) {
        printf("Error reading message.\n");
        return;
    }
    messageContent[strcspn(messageContent, "\n")] = 0;

    PROC_MSG* newMessage = (PROC_MSG*)malloc(sizeof(PROC_MSG));
    if (newMessage == NULL) {
        fprintf(stderr, "Error: Memory allocation for newMessage failed.\n");
        return;
    }

    int senderPID = currentProcess->pid;

    newMessage->senderPID = senderPID;
    newMessage->recipientPID = pid;
    strncpy(newMessage->body, messageContent, sizeof(newMessage->body) - 1);
    newMessage->body[sizeof(newMessage->body) - 1] = '\0'; // Ensure null-termination

    PCB* repliedToProccess = PCB_find(pid);
    if (repliedToProccess->message != NULL)
    {
        printf("A process can only receive 1 message at a time.\n");
        printf("Failed to reply the message to process %d\n", pid);
        return;
    }

    repliedToProccess->message = newMessage;

    printf("Successfully replied a message from Process %d to Process %d\n", senderPID, pid);
    

    PCB* sender = NULL;
    for (PCB* pcb = List_first(waitingOnSendQueue); pcb != NULL; pcb = List_next(waitingOnSendQueue)) {
        if (pcb->pid == pid) {
            sender = pcb;
        }
    }

    if (sender == NULL) {
        printf("There is no process waiting on send operation with PID %d", pid);
    } else {
        sender->state = READY;
        for (PCB* pcb = List_first(waitingOnSendQueue); pcb != NULL; pcb = List_next(waitingOnSendQueue)) {
            if (pcb->pid == pid) {
                List_remove(waitingOnSendQueue);
            }
        }
        addProcessToReadyQueue(sender);
    }
}

void handleProcinfoCommand()
{
    int pid;
    printf("Enter the PID of the process to display information for: ");
    scanf("%d", &pid);

    PCB* process = PCB_find(pid);
    if (process == NULL) 
    {
        printf("No process found with PID %d.\n", pid);
        return;
    }

    PCB_print(process);
}
void handleTotalinfoCommand()
{
    printf("High Priority Ready Queue:\n");
    Queue_print(highPriorityReadyQueue);

    printf("Normal Priority Ready Queue:\n");
    Queue_print(normalPriorityReadyQueue);

    printf("Low Priority Ready Queue:\n");
    Queue_print(lowPriorityReadyQueue);

    printf("Waiting on a Send operation Queue:\n");
    Queue_print(waitingOnSendQueue);

    printf("Waiting on a Receive operation Queue:\n");
    Queue_print(waitingOnReceiveQueue);
}

void handleNewSemaphoreCommand()
{
    int semaphoreID;
    printf("Enter the semaphore ID: ");
    scanf("%d", &semaphoreID);

    int initialValue;
    printf("Enter the initial value: ");
    scanf("%d", &initialValue);

    int result = initializeSemaphore(semaphoreID, initialValue);
    if (result == 0)
    {
        printf("Failed to initialize a new semaphore!");
    } else {
        printf("Semaphore %d with initial value: %d successfully created!\n", semaphoreID, initialValue);
    }
}

void handleSemaphorePCommand()
{
    int semaphoreID;
    printf("Enter the semaphore ID: ");
    scanf("%d", &semaphoreID);

    int value = semaphoreWait(semaphoreID, currentProcess);
    if (value < 0 ) {
        printf("Process %d is blocked on semaphore %d\n", currentProcess->pid, semaphoreID);
    } else {
        printf("Value of semaphore %d is currently %d\n", semaphoreID, value);
    }
}

void handleSemaphoreVCommand()
{
    int semaphoreID;
    printf("Enter the semaphore ID: ");
    scanf("%d", &semaphoreID);

    int value = semaphoreSignal(semaphoreID);
    if (value <= 0) {
        printf("A Process is now READY\n");
    } else {
        printf("Value of semaphore %d is currently %d\n", semaphoreID, value);
    }
}