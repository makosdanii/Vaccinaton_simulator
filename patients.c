#include "patients.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void init(Patients *array, size_t init_size) 
{
    Person *ptr;
    ptr = (Person *)malloc(init_size * sizeof(Person));

    if (ptr == NULL)
    {       
        printf("Failed to create\n");
        free(ptr);
        exit(1);
    }
    else
    {
        array->head = ptr; 
        array->arr_size = init_size-1;
        for(int i = 0; i<init_size; i++)
        {
            memset(&array->head[i],0,sizeof(Person));
        }
    }
}

void _add(Patients *array, Person person) 
{
    Person *ptr;

    ptr = (Person *)realloc(array->head, ++array->arr_size * sizeof(Person));

    if (ptr == NULL)
    {       
        printf("Failed to expand\n");
        free(ptr);
        exit(1);
    }
    else
    {
        // free(array->head);
        array->head = ptr;
        array->head[array->arr_size-1].name = (char*)malloc((strlen(person.name) + 1)*sizeof(char));
        strcpy(array->head[array->arr_size-1].name, person.name);

        array->head[array->arr_size-1].date = (char*)malloc((strlen(person.date) + 1)*sizeof(char));
        strcpy(array->head[array->arr_size-1].date, person.date);

        array->head[array->arr_size-1].phone = (char*)malloc((strlen(person.phone) + 1)*sizeof(char));
        strcpy(array->head[array->arr_size-1].phone, person.phone);
        array->head[array->arr_size-1].paid = person.paid;

    }
}

void _delete(Patients *array, int index) 
{
    Patients temp;
    init(&temp, 1);

    for(int i=0; i<array->arr_size; i++)
    {
        if(i!=index) _add(&temp, array->head[i]);
    }

    _free(array);
    array->head = temp.head; 
    array->arr_size = temp.arr_size;

}

void _free(Patients *array) 
{
    free(array->head);
    array->head = NULL;
    array->arr_size = 0;
}
