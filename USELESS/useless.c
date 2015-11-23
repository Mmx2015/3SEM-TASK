//First Task. Part One. USELESS.
//Name of input file is input.txt

//Some useful headers.
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

//Global constants.
//"\n" symbol as End of Line.
char NextLine = 10;
//Calculus base for "strtol" function.
int CalculusBase = 10;
//Maximum string size.
int MaxStringSize = 100;

//No global variables.

//Function that executes command stored in string.
int useless(char *str){

    //Variables.
    //Variable that stores pause time.
    int pause;
    //Dynamic string to store commands.
    char *command=malloc(MaxStringSize),*p;
    //Child process PID.
    pid_t pid;

    //Detecting whether child process is created.
    if((pid=fork())<0){
	   printf("%s\n","Can't create child process.");
	   exit(-1);
    }
    if (pid>0) return 0;

    //This function converts string to long int. 10 is the count basis. End of long int string value stores into P.
    pause=strtol(str,&p,CalculusBase);

    //Pausing for PAUSE milliseconds. 
    sleep(pause);

    //Copying command string to COMMAND variable.
    strcpy(command,p+1);

    //We use "execl" this way, because it's easy not to resolve string by arguments.
    if(execl(command,NULL)<0){
        printf("%s%s%s\n","Program ",command," failed to load.");
        exit(0);
    }

    //To get rid of warning: "Control may reach the end of non-void function".
    exit(-1);
}


//Function main.
int main(){ 

    //Variables.
    //File variable.
    int file;
    //c - temporary char variable. str & cur - pointers to strings.
    char c,*str,*cur;
    //Temporary variable to store string size.
    ssize_t size;

    //Opening file "input.txt".
    if((file=open("input.txt",O_RDONLY))<0){
    	    printf("%s\n","Can't open file!");
            exit(-1);
    }

    //Creating dynamic string to store entered commands.
    str=malloc(MaxStringSize);
    cur=str;
	size=1;
    //This while reads string and loads USELESS function.
    while (size!=0){
        //Reading one symbool from file.
        size=read(file,&c,1);
	    if((c == NextLine)||(size==0)){ 	    
		  *cur=0;
		  if(strlen(str)>0){ 
		      useless(str);
          }
          cur=str;
	    }
	    else{
		  *cur=c;
          //Asking for next symbol.
		  cur++;
	    }
	}    
    //Detecting if file is closed now.
    if(close(file)<0){
        printf("%s\n","Can't close file!");
	    exit(-1);
    }
    //I use this on Mac to wait for terminal to write out log for functions.
	while(wait(0)!=-1);
}


