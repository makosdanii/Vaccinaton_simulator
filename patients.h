#include <stdbool.h>
#include <stddef.h>

#ifndef PATIENTS
#define PATIENTS

typedef struct{
    char* name;
    char* date;
    char* phone;
    bool paid;
}Person;

typedef struct{
    Person* head;
    int arr_size;
}Patients;

void init(Patients *array, size_t init_size);

void _add(Patients *array, Person person);

void _delete(Patients *array, int index);

void _free(Patients *array);

#endif