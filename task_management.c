#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define MAX_NAME_LENGTH 100
#define MAX_DATE_LENGTH 20
//Razan Tartir
//1230922


typedef struct Task{
int tskId;
char tskName[MAX_NAME_LENGTH];
char tskDate[MAX_DATE_LENGTH];
float tskDuration;
struct Task* next;
} Task;


typedef struct Stack{
Task* top;
}Stack;

void push(Stack* stack, Task* task);
Task* pop(Stack* stack);


typedef struct Queue{
Task* front;
Task* rear;
}Queue;


typedef struct LinkedList{
Task* head;
}LinkedList;


void LoadTasksFromFile(LinkedList*unformedTasks);
void AddNewTask(LinkedList*unformedTasks);
void DeleteTasks(LinkedList*unformedTasks, int tskId);
Task*searchTask(LinkedList*unformedTasks,int tskId,char*tskName);
void ViewUnperformedTasks(LinkedList*unperformedTasks);
void PerformTask(LinkedList*unperformedTasks,Stack*performedTasks,int tskId);
void UndoLastPerformedTask(LinkedList*unperformedTasks,Stack*performedTasks,Queue*undoneTasks);
void ViewPerformedTasks(Stack*performedTasks);
void GenerateSummaryReport(LinkedList*unperformedTasks,Stack*performedTasks,Queue*undoneTasks);
void FreeList(LinkedList*list);


void menu(){
printf("\nTask Management System Menu:\n");
printf("1. Load Tasks File\n");
printf("2. Add a New Task\n");
printf("3. Delete a Task\n");
printf("4. Search for a Task\n");
printf("5. View Unperformed Tasks\n");
printf("6. Perform a Task\n");
printf("7. Undo Last Perform Task\n");
printf("8. View Performed Tasks\n");
printf("9. Generate a Summary Report\n");
printf("10. Exit\n");
}

Task* createTask(int tskId,char* tskName,char* tskDate,float tskDuration){
Task* newTask =(Task*)malloc(sizeof(Task));
newTask->tskId=tskId;
strcpy(newTask->tskName,tskName);
strcpy(newTask->tskDate,tskDate);
newTask->tskDuration=tskDuration;
newTask->next=NULL;
return newTask;
}

void push(Stack* stack, Task* task) {
    task->next = stack->top;
    stack->top = task;
}
Task* pop(Stack* stack) {
    if (stack->top == NULL) {
        return NULL;
    }
    Task* poppedTask = stack->top;
    stack->top = stack->top->next;
    return poppedTask;
}

void addTaskToList(LinkedList* list,Task* task){
  if(list->head==NULL){
      list->head=task;
  }
    else{
    Task* temp=list->head;
     while(temp->next !=NULL){
       temp=temp->next;
     }
     temp->next=task;
   }
}


int main()
{
    LinkedList unperformedTasks={NULL};
    Stack performedTasks={NULL};
    Queue undoneTasks={NULL,NULL};

    int select;

    do {
    menu();
    printf("Enter your choice:\n");
    scanf("%d", &select);

    switch(select) {
        case 1:
            LoadTasksFromFile(&unperformedTasks);
            break;

        case 2:
            AddNewTask(&unperformedTasks);
            break;

        case 3:
            {
                int tskId;
                printf("Enter Task ID to delete: ");
                scanf("%d", &tskId);
                DeleteTasks(&unperformedTasks, tskId);
            }
            break;

        case 4:
            {
        int tskId;
        char tskName[MAX_NAME_LENGTH];
        printf("Enter Task ID (-1 if not searching by ID): ");
        scanf("%d", &tskId);
        getchar();

    if (tskId == -1) {
       printf("Enter Task Name: ");
       gets(tskName);

    } else {
      tskName[0] = '\0';
    }
   Task* task = searchTask(&unperformedTasks, tskId, tskName);
    if (task) {
    printf("Task Found: %d#%s#%s#%.2f\n", task->tskId, task->tskName, task->tskDate, task->tskDuration);
    } else {
    printf("Task not Found.\n");
          }
            }
            break;

        case 5:
            ViewUnperformedTasks(&unperformedTasks);
            break;

        case 6:
            {
                int tskId;
                printf("Enter Task ID to perform:");
                scanf("%d", &tskId);
                PerformTask(&unperformedTasks, &performedTasks, tskId);
            }
            break;

        case 7:
            UndoLastPerformedTask(&unperformedTasks, &performedTasks, &undoneTasks);
            break;

        case 8:
            ViewPerformedTasks(&performedTasks);
            break;

        case 9:
            GenerateSummaryReport(&unperformedTasks, &performedTasks, &undoneTasks);
            break;

        case 10:
            FreeList(&unperformedTasks);
            printf("Exiting System! Good Bye.\n");
            return 0;

        default:
            printf("Invalid choice.Please Try again.\n");
    }
} while (select != 10);

    return 0;
}

void LoadTasksFromFile(LinkedList*unperformedTasks){
  FILE*file=fopen("tasks.txt","r");
  char sen[200];
 int tskId;
 char tskName[MAX_NAME_LENGTH];
 char tskDate[MAX_DATE_LENGTH];
 float tskDuration;

    while (fgets(sen, sizeof(sen),file)) {
           char* token=strtok(sen,"#");
           if(token !=NULL){
            tskId=atoi(token);
           }
           token=strtok(NULL,"#");
           if(token != NULL){
            strncpy(tskName,token,MAX_NAME_LENGTH-1);
            tskName[MAX_NAME_LENGTH-1]='\0';
           }
           token=strtok(NULL,"#");
           if(token != NULL){
            strncpy(tskDate,token,MAX_DATE_LENGTH-1);
            tskDate[MAX_DATE_LENGTH-1]='\0';
           }
           token=strtok(NULL,"#");
           if(token != NULL){
            tskDuration=atof(token);
           }
        Task* newTask = createTask(tskId, tskName, tskDate, tskDuration);
        addTaskToList(unperformedTasks, newTask);
    }
   fclose(file);
   printf("Task Loaded successfully.\n");
}


void AddNewTask(LinkedList* unperformedTasks) {
    int tskId;
    char tskName[MAX_NAME_LENGTH];
    char tskDate[MAX_DATE_LENGTH];
    float tskDuration;

    printf("Enter Task ID: ");
    scanf("%d", &tskId);
    getchar();

    if (searchTask(unperformedTasks, tskId, "")) {
        printf("Task with ID %d already exists. The Task will not be added.\n", tskId);
        return;
    }
    printf("Enter Task Name: ");
    gets(tskName);
    printf("Enter Task Date (D/M/Y): ");
    gets(tskDate);
    printf("Enter Task Duration (in hours): ");
    scanf("%f", &tskDuration);

    Task* newTask = createTask(tskId, tskName, tskDate, tskDuration);
    addTaskToList(unperformedTasks, newTask);
    printf("Task added successfully.\n");
}


void DeleteTasks(LinkedList*unperformedTasks, int tskId){
  Task* temp=unperformedTasks->head;
  Task* prev=NULL;
  if(temp != NULL && temp->tskId==tskId){
    unperformedTasks->head=temp->next;
    free(temp);
    printf("Task with ID %d Deleted successfully.\n",tskId);
    return;
}
  while(temp!= NULL && temp->tskId != tskId){
    prev=temp;
    temp=temp->next;
}
  if(temp==NULL){
    printf("Task with ID %d not found.\n",tskId);
    return;
}
  prev->next=temp->next;
  free(temp);
  printf("Task  with ID %d Deleted successfully.\n",tskId);
}


   Task* searchTask(LinkedList*unformedTasks,int tskId,char* tskName){
   Task* temp= unformedTasks->head;
   while(temp != NULL){
   if ((tskId != -1 && temp->tskId == tskId)){
        return temp;
      }
      if(tskName[0] != '\0' && strcmp(temp->tskName,tskName)==0){
        return temp;
      }
       temp= temp->next;
}
    return NULL;
}


void ViewUnperformedTasks(LinkedList*unperformedTasks){
   Task* temp=unperformedTasks->head;
    if(temp==NULL){
    printf(" There are No unperformed Tasks.\n");
    return;
}
  printf("\nUnperformed Tasks:\n");
  while(temp != NULL){
  printf("%d#%s#%s#%.2f \n",temp->tskId,temp->tskName,temp->tskDate,temp->tskDuration);
  temp=temp->next;
}
}

void PerformTask(LinkedList* unperformedTasks, Stack* performedTasks, int tskId) {
    if (unperformedTasks->head == NULL) {
        printf("There are NO tasks to Perform.\n");
        return;
    }
    Task* taskToPerform = NULL;
    Task* temp = unperformedTasks->head;
    Task* prev = NULL;

    while (temp != NULL) {
        if (temp->tskId == tskId) {
            taskToPerform = temp;
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    if (taskToPerform == NULL) {
        printf("Task ID %d not found.\n", tskId);
        return;
    }
    if (prev == NULL) {
        unperformedTasks->head = taskToPerform->next;
    } else {
        prev->next = taskToPerform->next;
    }
    taskToPerform->next = performedTasks->top;
    performedTasks->top = taskToPerform;

    printf("Task ID %d performed successfully.\n", tskId);
}

void UndoLastPerformedTask(LinkedList*unperformedTasks,Stack*performedTasks,Queue*undoneTasks){
    if(performedTasks->top==NULL){
    printf("No performed tasks to Undo.\n");
    return;
}
   Task* taskToUndo=performedTasks->top;
   performedTasks->top=taskToUndo->next;

   taskToUndo->next=NULL;
    if(undoneTasks->rear==NULL){
      undoneTasks->front=undoneTasks->rear=taskToUndo;
    }else{
    undoneTasks->rear->next=taskToUndo;
    undoneTasks->rear=taskToUndo;
}
if(unperformedTasks->head==NULL){
    unperformedTasks->head=taskToUndo;
}else{
Task* temp=unperformedTasks->head;
while(temp->next != NULL){
    temp= temp->next;
}
temp->next=taskToUndo;
}
taskToUndo->next=NULL;

   printf("Last performed task undone.\n");
}


void ViewPerformedTasks(Stack* performedTasks) {
    if (performedTasks->top == NULL) {
        printf("No Performed Tasks.\n");
        return;
    }

    printf("\nPerformed Tasks:\n");
    Stack* tempStack = (Stack*)malloc(sizeof(Stack));
    tempStack->top = NULL;

    Task* temp = performedTasks->top;
    while (temp != NULL) {
        Task* taskCopy = (Task*)malloc(sizeof(Task));
        *taskCopy = *temp;
        push(tempStack, taskCopy);
        temp = temp->next;
    }
    while (tempStack->top != NULL) {
        Task* task = pop(tempStack);
        printf("%d#%s#%s#%.2f \n",task->tskId, task->tskName, task->tskDate, task->tskDuration);
        free(task);
    }
    free(tempStack);
}


void GenerateSummaryReport(LinkedList* unperformedTasks, Stack* performedTasks, Queue* undoneTasks) {
    FILE* file = fopen("Report.txt", "w");

    fprintf(file, "Razan Tartir\n");
    fprintf(file, "1230922\n\n");
    fprintf(file,"sec_5\n");

    fprintf(file, "Unperformed Tasks:\n");
    Task* temp = unperformedTasks->head;
    while (temp != NULL) {
        fprintf(file, "#%d# %s# %s# %.2f\n", temp->tskId, temp->tskName, temp->tskDate, temp->tskDuration);
        temp = temp->next;
    }

    fprintf(file, "\nUndone Tasks:\n");
    temp = undoneTasks->front;
    while (temp != NULL) {
        fprintf(file, "#%d# %s# %s# %.2f\n", temp->tskId, temp->tskName, temp->tskDate, temp->tskDuration);
        temp = temp->next;
    }

    fprintf(file, "\nPerformed Tasks:\n");
    temp = performedTasks->top;
    while (temp != NULL) {
        fprintf(file, "#%d# %s# %s# %.2f\n", temp->tskId, temp->tskName, temp->tskDate, temp->tskDuration);
        temp = temp->next;
    }

    fclose(file);
    printf("Summary Report Generated Successfully and saved to Report file.\n");
}

void FreeList(LinkedList*list){
    Task* temp=list->head;
    while(temp != NULL){
    Task* ToFree=temp;
    temp=temp->next;
    free(ToFree);
}
   list->head=NULL;
}
