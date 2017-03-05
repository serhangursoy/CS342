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

#define MAX_LINE_LENGTH 260
#define MAX_FILE_NAME 64
#define MAX_WORD_LENGTH 64

char *word;
FILE *files;


void process(FILE *input , int file_no){
    int counter = 0;
    char* out_name;
    sprintf(out_name, "out%d.txt" , file_no);
    FILE *output = fopen(out_name , "w");
    
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
                
                fprintf(output , "%s", current_line);
                //printf("%s", current_line);
                break;
            }
        temp_line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
        
        
    }
    fclose(&files[file_no - 1]);
    
}

void transfer(FILE *input , FILE *output){
    char *current_line = (char *)malloc(MAX_LINE_LENGTH * sizeof(char));
    while(fgets(current_line , MAX_LINE_LENGTH , input) != NULL) {
        fprintf(output , "%s", current_line);
    }
    //fclose(input);
}


int main(int argc , char* argv[]){
    word = argv[1];
    int num_of_files = atoi(argv[2]);
    files = (FILE *) malloc(sizeof(FILE) * num_of_files);
    
    int j = 0;
    int pid_arr[num_of_files];
    FILE *intermediate_files[num_of_files];
    for(int m = 0; m < num_of_files ; m++)
        
        
        for(; j < num_of_files ; j++){
            pid_arr[j] = fork();
            if(pid_arr[j] == 0) {
                files[j] = *fopen(argv[3 + j], "r");
                process(&files[j] , j + 1);
                fclose(&files[j]);
                exit(EXIT_SUCCESS);
            }else{
                wait(NULL);
            }
        }
    for(int k = 0; k < num_of_files; k++) {
        waitpid(pid_arr[k], 0, 0);
    }
    char name[12];
    FILE *temp_file;
    for(int a = 0; a < num_of_files; a++){
        
        FILE *output_final = fopen(argv[3 + num_of_files] , "a");
        sprintf(name, "out%d.txt" , a + 1);
        temp_file = fopen(name , "r");
        transfer(temp_file, output_final);
    }
    
    
    
    
    
    exit(EXIT_SUCCESS);
}
