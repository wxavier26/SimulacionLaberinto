/*

Universidad del Valle de Guatemala
Algoritmos y estructura de datos

Xavier Cifuentes Pérez 13316
Pablo de León Rodas 13227
Ana Fernanda Solares 13125
Jose Rosales 12576
Julio Isaac Rodriguez 14298
Julian Estuardo Escobedo 14684


 * Programa en C para implementar una pila utilizando linked list
 *
 * Referencia:
 * http://www.sanfoundry.com/c-program-stack-using-linked-list/
 */
 
#include <stdio.h>
#include <stdlib.h>
 
struct node
{
    int info;
    struct node *ptr;
}*top,*top1,*temp;

int countS = 0;
 
/* Create empty stack */
void create()
{
    top = NULL;
}
 
/* Count stack elements */
void stack_count()
{
    printf("\n No. of elements in stack : %d", countS);
}
 
/* Push data into stack */
void push(int data)
{
    if (top == NULL)
    {
        top =(struct node *)malloc(1*sizeof(struct node));
        top->ptr = NULL;
        top->info = data;
    }
    else
    {
        temp =(struct node *)malloc(1*sizeof(struct node));
        temp->ptr = top;
        temp->info = data;
        top = temp;
    }
    countS++;
}
 
/* Display stack elements */
void display()
{
    top1 = top;
 
    if (top1 == NULL)
    {
        printf("Stack is empty");
        return;
    }
 
    while (top1 != NULL)
    {
        printf("%d ", top1->info);
        top1 = top1->ptr;
    }
 }
 
/* Pop Operation on stack */
void pop()
{
    top1 = top;
 
    if (top1 == NULL)
    {
        printf("\n Error : Trying to pop from empty stack");
        return;
    }
    else
        top1 = top1->ptr;
    printf("\n Popped value : %d", top->info);
    free(top);
    top = top1;
    countS--;
}
 
/* Return top element */
int topelement()
{
    return(top->info);
}
 
/* Check if stack is empty or not */
void empty()
{
    if (top == NULL)
        printf("\n Stack is empty");
    else
        printf("\n Stack is not empty with %d elements", countS);
}
 
/* Destroy entire stack */
void destroy()
{
    top1 = top;
 
    while (top1 != NULL)
    {
        top1 = top->ptr;
        free(top);
        top = top1;
        top1 = top1->ptr;
    }
    free(top1);
    top = NULL;
 
    printf("\n All stack elements destroyed");
    countS = 0;
}