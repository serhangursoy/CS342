//
//  pmatch1.c
//
//
//  Created by Ege Yosunkaya on 22/02/2017.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define MAX_LINE_LENGTH 280
#define MAX_FILE_NAME 64
#define MAX_WORD_LENGTH 64


char *word;
FILE *files;

typedef struct Node{
    char* line;
    struct Node* next;
} Node;

typedef struct t_params{
    struct Node* head;
    FILE * input_file;
    int file_no;
} t_params;

//args --> INPUT FILE
//Returns --> Node struct * head
void* threadRoutine(void * arg) {
    int counter = 0;
    
    t_params *params;
    params = (t_params *)arg;
    struct Node* head = params->head;
    head->next = NULL;
    head->line = NULL;
    FILE *input = params->input_file;
    int file_no = params->file_no;
    struct Node *current = head;
    
    char *current_word = (char *)malloc(MAX_WORD_LENGTH * sizeof(char));
    char *current_line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    char *temp_line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    char *temp_line2 = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
    while(fgets(current_line , MAX_LINE_LENGTH , input) != NULL){
        counter++;
        strcpy(temp_line , current_line);
        char *p = strchr(temp_line, '\n');
        if (p != NULL) *p = '\0';
        while((current_word = strsep(&temp_line , " ")))
            if(strcmp(current_word , word) == 0){
                sprintf(temp_line2, "%d: %s" , counter, current_line);
                sprintf(current_line , "input%d, %s", file_no , temp_line2);
                current->line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
                strcpy(current->line , current_line);
                //printf("%s", current->line);
                current->next = (struct Node *)malloc(sizeof(struct Node));
                current = current->next;
                break;
            }
        temp_line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    }
    current->next = NULL;
    current->line = NULL;
    fclose(input);
    return 0;
}

int main(int argc, char *argv[])
{
    word = argv[1];
    int num_of_files = atoi(argv[2]);
    files = (FILE *) malloc(sizeof(FILE) * num_of_files);
    pthread_t thread_list[num_of_files];
    t_params * paramslist[num_of_files];
    for(int k = 0; k < num_of_files ; k++) {
        paramslist[k] = (struct t_params *) malloc(sizeof(struct t_params));
    }
    struct Node heads[num_of_files];
    
    
    
    for(int i = 0; i < num_of_files ; i++){
        paramslist[i]->head = &heads[i];
        paramslist[i]->input_file = fopen(argv[3 + i] , "r");
        paramslist[i]->file_no = i + 1;
        pthread_create(&thread_list[i] , NULL , threadRoutine , paramslist[i]);
        
    }
    
    
    
    
    for(int i = 0; i< num_of_files ;i++){
        pthread_join(thread_list[i] , NULL);
    }
    
    FILE *output = fopen(argv[num_of_files + 3] , "w" );
    struct Node * nodePtr;
    for(int i = 0; i< num_of_files ;i++){
        nodePtr = &heads[i];
        while(nodePtr != NULL){
            if(nodePtr->line != NULL)
                fprintf(output, "%s" ,nodePtr->line);
            if(nodePtr->next != NULL)
                nodePtr = nodePtr->next;
            else
                break;
        }
    }
    
    
    
    exit(0);
}







