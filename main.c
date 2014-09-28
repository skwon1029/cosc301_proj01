/*
 *
 * Soo Bin Kwon -- Colgate University '16
 * 09/28/2014
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/resource.h>

#include "list.h"

/*
 * Returns a pointer to an array that contains all the
 * valid integers found in the input string s
 *
 * Some parts adapted from lab02 tokenify.c
 */
void tokenify(const char *s, struct node **head) {
	char *copy_s = strdup(s); //copy of the input string	
    char *reserve; // temporary space to save the remainder of the string after strtok_r
	
	char *token = strtok_r(copy_s," \n\t", &reserve); // take the first token
	//go through each token
    while(token!=NULL){
		char *temp = strdup(token);	// allocate the token from the heap	
		
		//ignore the rest if the token starts with '#'
		if(temp[0]=='#'){		
			free(copy_s);
			return;
		}		
		//otherwise, go through each character of the token
		bool comment = false;
		bool valid = true;	
		for(int i=0 ;i<strlen(temp) ;i++){
			//if the character is '#',
			//set boolean 'comment' to true & set the char to null
			if(temp[i]=='#'){
				comment = true;
				temp[i] = '\0';
				break;	//we can ignore the rest of the token
			//if the character is not a number or negative sign,
			//set boolean 'valid' to false
			} else if ((temp[i]<48 || temp[i]>57) & (temp[i]!=45)){
				valid = false;
			}
		}
		
		//if the token is a valid integer, append it
		if(valid){
			list_append(atoi(temp),head);
		}
		
		//if there was any comment character in the token
		//exit the function (so that we can ignore the rest of the string s)
		if(comment){
			free(copy_s);
			return;
		//if not, move on to the next token
		} else {
			token = strtok_r(reserve," \n\t", &reserve);
		}
    }
    free(copy_s);
}

void process_data(FILE *input_file) {
	struct rusage usage;
	struct timeval user_end, sys_end;
	
	//assume each line contains less than or equal to 100 characters
	char *line = malloc(101*sizeof(char));
	
	//initialize linked list
	struct node *tokens = NULL;

	while(fgets(line, 100, input_file)!=NULL){
		tokenify(line, &tokens);			
	}
	
	list_sort(&tokens);
	printf("*** List Contents Begin ***\n");
	list_print(tokens);
	printf("*** List Contents End ***\n");
    free(line);    
    list_clear(tokens);
    
    getrusage(RUSAGE_SELF, &usage);
    user_end = usage.ru_utime;
    sys_end = usage.ru_stime;
	
	//print out resource stat
	printf("User time:\t%f\n", user_end.tv_sec + (user_end.tv_usec)/1000000.);
	printf("System time:\t%f\n", sys_end.tv_sec + (sys_end.tv_usec)/1000000.);
}

void usage(char *program) {
    fprintf(stderr, "usage: %s [<datafile>]\n", program);
    exit(1);
}

#ifndef AUTOTEST
int main(int argc, char **argv) {
    FILE *datafile = NULL;

    /* find out how we got invoked and deal with it */
    switch (argc) {
        case 1:
            /* only one program argument (the program name) */ 
            /* just equate stdin with our datafile */
            datafile = stdin;        
            break;

        case 2:
            /* two arguments: program name and input file */
            /* open the file, assign to datafile */
            datafile = fopen(argv[1], "r");
            if (datafile == NULL) {
                printf("Unable to open file %s: %s\n", argv[1], strerror(errno));
                exit(-1);
            }
            break;

        default:
            /* more than two arguments?  throw hands up in resignation */
            usage(argv[0]);
    }

    /* 
     * you should be able to just read from datafile regardless 
     * whether it's stdin or a "real" file.
     */
    process_data(datafile);
    fclose(datafile);
    return 0;
}
#endif
