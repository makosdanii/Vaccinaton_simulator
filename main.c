#include "menu.h"
#include "patients.h"
#include <unistd.h>
#include <wait.h>

void foo(){
    int pid = fork();
    if(pid == 0){
        printf("child");
        exit(0);
    }else{printf("parent");wait(NULL);}
    printf("over");

}

int main(int argc, char** argv){
    if (argc < 2){
        foo();
        printf("Insufficient args");
        exit(1);
    }

    FILE* fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL){
        printf("Failed to open file");
        exit(1);
    }

    Patients patients;
    load(&patients, fp);

    menu(&patients);

        fp = fopen(argv[1], "w");
    if (fp == NULL){
        printf("Failed to open file");
        exit(1);
    }

    save(&patients, fp);

    _free(&patients);
}
