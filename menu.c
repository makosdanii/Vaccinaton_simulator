#include "menu.h"
#include "pipe.h"

void _read(Patients* arr){
    for(int i = 0; i < arr->arr_size; i++){
        printf("\nPatient no. %d\nName: %s\nDate of birth: %s\nPhone no.: %s\nExtra fee: ",
                i, arr->head[i].name, arr->head[i].date, arr->head[i].phone);
        printf("%s", arr->head[i].paid ? "Yes\n" : "No\n");
    }
}

void save(Patients* arr, FILE* fp){
    
    for(int i = 0; i < arr->arr_size; i++){
        fprintf(fp, "%s\n%s\n%s\n",
                arr->head[i].name, arr->head[i].date, arr->head[i].phone);
        fprintf(fp, arr->head[i].paid ? "1\n" : "0\n");
    }
    
    fclose(fp);
}

void load(Patients *arr, FILE* fp){
    init(arr, 1);

    int char_cnt = 0;
    int newline_cnt = 0;

    char c;    
    char input[INPUT_SIZE];
    Person x;

    while ((c = fgetc(fp)) != EOF){
        
        if(c != '\n'){
            input[char_cnt++] = c;
        }else{
            input[char_cnt++] = '\0';

            switch(newline_cnt){
                case 0:
                    x.name = (char*)malloc(char_cnt*sizeof(char));
                    strcpy(x.name, input);
                    break;
                case 1:
                    x.date = (char*)malloc(char_cnt*sizeof(char));
                    strcpy(x.date, input);
                    break;
                case 2:
                    x.phone = (char*)malloc(char_cnt*sizeof(char));
                    strcpy(x.phone, input);
                    break;
                case 3:
                    x.paid = input[0] == '1';
                    break;
            }

            char_cnt = 0;
            memset(&input, 0, INPUT_SIZE);
            newline_cnt++;
        }

        if (newline_cnt == 4){
            _add(arr, x);
            free(x.name); free(x.date); free(x.phone);
            newline_cnt = 0;
        }

    }
    fclose(fp);
}

void add(Patients* arr){
    Person x;
    char input[INPUT_SIZE];

    printf("\nName: ");
    scanf("%99s", &input);
    x.name=(char*)malloc((strlen(input) + 1)*sizeof(char));
    strcpy(x.name, input);
    memset(&input, 0, INPUT_SIZE);

    printf("Date of birth: ");
    scanf("%99s", &input);
    x.date=(char*)malloc((strlen(input) + 1)*sizeof(char));
    strcpy(x.date, input);
    memset(&input, 0, INPUT_SIZE);

    printf("Phone no.: ");
    scanf("%99s", &input);
    x.phone=(char*)malloc((strlen(input) + 1)*sizeof(char));
    strcpy(x.phone, input);
    memset(&input, 0, INPUT_SIZE);

    bool cond = true;
    while (cond){ 
        cond = false;
        printf("Extra fee (Y/N): ");
        scanf("%1s", &input);
        switch(input[0]){
            case 'Y':;
            case 'y': x.paid = true; break;
            case 'N':;
            case 'n':x.paid = false; break;
            default: printf("Invalid\n");
                cond = true; break;
        }
    }

    _add(arr, x);
}

void _remove(Patients* arr, char id[]){
    if (id != ""){
        for(int i = 0; i<arr->arr_size; i++){
            if (strcmp(arr->head[i].phone, id) == 0){
                _delete(arr, i);
                return;
            }
        }
        printf("Algorithm err"); exit(1);
    }

    if (arr->arr_size == 0){
        printf("\nPatient list is empty already\n");
        return;
    }

    bool cond = true;
    while(cond){
        printf("\nPatient no. to remove: ");
        char input[3];
        scanf("%2s", &input);
        if (atoi(input) == EOF){
            printf("Invalid input (number expected)");
            memset(&input, 0, 3);
            continue;
        }

        if (-1 < atoi(input) && atoi(input) < arr->arr_size){
            cond = false;
            _delete(arr, atoi(input));
        }else{
            printf("Patient no. non-exsistent");
            memset(&input, 0, 3);
        }
    }
}

void modify(Patients* arr){
    if (arr->arr_size == 0){
        printf("\nPatient list is empty yet\n");
        return;
    }

    bool cond = true;
    bool cond2 = true;
    while(cond){
        printf("\nPatient no. to modify data: ");
        char input[3];
        scanf("%2s", &input);
        if (atoi(input) == EOF){
            printf("Invalid input (number expected)");
            memset(&input, 0, 3);
            continue;
        }

        if (-1 < atoi(input) && atoi(input) < arr->arr_size){
            
            printf("\nData to modify (Name - 0, date - 1, phone - 2, fee - 3): ");
            char input2[2];
            scanf("%1s", &input2);
            if (atoi(input2) == EOF){
                printf("Invalid input (number expected)");
                memset(&input2, 0, 2);
                memset(&input, 0, 3);
                continue;
            }


            if (0 <= atoi(input2) && atoi(input2) <= 3){
                cond = false;
                char input3[INPUT_SIZE];
                switch(atoi(input2)){
                    case 0: 
                        printf("\nName: ");
                        scanf("%99s", &input3);
                        arr->head[atoi(input)].name=(char*)malloc((strlen(input3) + 1)*sizeof(char));
                        strcpy(arr->head[atoi(input)].name, input3);
                    break;
                    case 1: 
                        printf("Date of birth: ");
                        scanf("%99s", &input3);
                        arr->head[atoi(input)].date=(char*)malloc((strlen(input3) + 1)*sizeof(char));
                        strcpy(arr->head[atoi(input)].date, input3);
                    break;
                    case 2: 
                        printf("Phone no.: ");
                        scanf("%99s", &input3);
                        arr->head[atoi(input)].phone=(char*)malloc((strlen(input3) + 1)*sizeof(char));
                        strcpy(arr->head[atoi(input)].phone, input3);
                    break;
                    case 3:
                        while (cond2){ 
                            cond2 = false;
                            printf("Extra fee (Y/N): ");
                            scanf("%1s", &input3);
                            switch(input3[0]){
                                case 'Y':;
                                case 'y': arr->head[atoi(input)].paid = true; break;
                                case 'N':;
                                case 'n':arr->head[atoi(input)].paid = false; break;
                                default: printf("Invalid\n");
                                    cond2 = true; break;
                            }
                        }
                    break;
                }
            }else{
                printf("Invalid data index");
                memset(&input2, 0, 2);
                memset(&input, 0, 3);
            }

        }else{
            printf("Patient no. non-exsistent");
            memset(&input, 0, 3);
        }
    }
}

void menu(Patients* arr){
    while (true){
        printf("\n--PLEASE SELECT!--\nLIST - 1\nADD - 2\nREMOVE - 3\nMODIFY - 4\nSIMULATE - 5\nEXIT - 6\n>>");
        char c[2];
        scanf("%1s", &c);

        switch (atoi(c))
        {
            case LIST: _read(arr); break;
            case ADD: add(arr); break;
            case REMOVE: _remove(arr, ""); break;
            case MODIFY: modify(arr); break;
            case SIMULATE: simulate(arr); break;
            case EXIT: printf("\nBye!\n"); break;
            default: printf("\nInvalid command (number expected)\n"); break;
        }

        if (*c == '6') break;
    }
};