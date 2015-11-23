//First Task. Part One. USELESS.
//Name of input file is input.txt

//Some useful libraries.
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

//File variable.
int file;
//c - temporary char variable. str & cur - pointers to strings.
char c,*str,*cur;
//Temporary variable to store string size.
ssize_t size;


//Function that executes command stored in string.
int useless(char *str){
    //Variable that stores pause time.
    int pause;
    //Dynamic string to store commands.
    char *command=malloc(100),*p;
    //Child process PID.
    pid_t pid;
    //Detecting whether child process is created.
    if((pid=fork())<0){
	   printf("%s\n","Can't create child process.");
	   exit(-1);
    }
    if (pid>0) return 0;
    //This function converts string to long int. 10 is the count basis. End of long int string value stores into P.
    pause=strtol(str,&p,10); 
    sleep(pause);
    //Copying command string to COMMAND variable.
    strcpy(command,p+1);
    //Executing the command. (may not be the best way to use here execl?)
    if(execl(command,0)<0){
        printf("%s%s%s\n","Program ",command," failed to load.");
        exit(0);
    }
}


//Function main.
int main(){ 
    //Opening file "input.txt".
    if((file=open("input.txt",O_RDONLY))<0){
    	    printf("%s\n","Can't open file!");
            exit(-1);
    }
    //Creating dynamic string to store entered commands.
    str=malloc(100);
    cur=str;
	size=1;
    //This while reads string and loads USELESS function.
    while (size!=0){
        //Reading one symbool from file.
        size=read(file,&c,1);
        //Maximum string size is 10.
	    if((c==10)||(size==0)){ 	    
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


