#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include "patients.h"

#define INPUT_SIZE 100

enum options{INVALID, LIST, ADD, REMOVE, MODIFY, SIMULATE, EXIT};

void load(Patients *arr, FILE* fp);
void save(Patients* arr, FILE* fp);
void _read(Patients* arr);
void add(Patients* arr);
void _remove(Patients* arr, char* id);
void modify(Patients* arr);
void menu(Patients* arr);

#endif