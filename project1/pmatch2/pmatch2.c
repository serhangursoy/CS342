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
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

#define MAX_LINE_LENGTH 400
#define MAX_FILE_NAME 64
#define MAX_WORD_LENGTH 64
#define READ 0
#define WRITE 1

char *word;
FILE *files;



void process(FILE *input , int pipe_arr[] , int file_no){
    
    int counter = 0;
    char *current_word = (char *)malloc(MAX_WORD_LENGTH * sizeof(char));
    char *current_line = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
    char *temp_line = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
    char *temp_line2 = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
    close(pipe_arr[READ]);
    while(fgets(current_line , MAX_LINE_LENGTH, input) != NULL){
        counter++;
        strcpy(temp_line , current_line);
        char *p = strchr(temp_line, '\n');
        if (p != NULL) *p = '\0';
        while((current_word = strsep(&temp_line , " "))) {
            if(strcmp(current_word , word) == 0){
                sprintf(temp_line2, "%d: %s" , counter, current_line);
                sprintf(current_line , "input%d, %s", file_no , temp_line2);
                write(pipe_arr[WRITE], current_line, MAX_LINE_LENGTH);
                //sleep(1);
                break;
            }
        }
        temp_line = (char *)malloc((MAX_LINE_LENGTH) * sizeof(char));
    }
    close(pipe_arr[WRITE]); /* Close used end*/
}


int main(int argc , char* argv[]) {
    
    word = argv[1];
    int num_of_files = atoi(argv[2]);
    int pipe_arr[num_of_files][2];
    pid_t idkeyf[num_of_files];
    int i = 0;
    files = (FILE *) malloc(sizeof(FILE) * num_of_files);
    
    char msgBuffer[MAX_LINE_LENGTH];
    
    
    
    for(; i < num_of_files; i++){
        pipe(pipe_arr[i]); /* Create Pipe */
        idkeyf[i] = fork();
        if(idkeyf[i] == 0){
            /* CHILD */
            //close([READ]);
            files[i] = *fopen(argv[3 + i], "r");
            process(&files[i], pipe_arr[i] ,i + 1);
            //close(pipe_arr[i][READ]);
            fclose(&files[i]);
            exit(0);
            //break;
            
        }
    }
    /* PARENT */
    
    for (int k = 0 ; k < num_of_files;k++)
        waitpid(idkeyf[k],0,0);
    
    
    //if(n != 0) {
    FILE *outfile = fopen(argv[3 + num_of_files] ,"w");
    for(int j = 0; j < num_of_files; j++) {
        close(pipe_arr[j][WRITE]);
        while (read(pipe_arr[j][READ], msgBuffer, MAX_LINE_LENGTH)) {
            fprintf(outfile ,"%s", msgBuffer);
        }
        
        close(pipe_arr[j][READ]);
    }
    
    //}
    
    exit(EXIT_SUCCESS);
}







