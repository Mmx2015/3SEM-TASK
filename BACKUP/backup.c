//First Task. Part Two. BACKUP.
//Name of output file is a.out.

//Some usefull headers.
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

//src - source file or directory path, dest - destination path for archive.
char *src,*dest;
//lsrc - length of "source" string, ldest - length of "dest" string, lmax - maximum of these two lengths.
int lsrc,ldest,lmax;

//Main function of this application. Creates backup of file or directory.
int backup(char *path,char *pathgz,time_t mtime){

    int i;
    //Statistics variable to detect time of last file access.
    struct stat stgz;
    //Parameter string for gzip procedure.
    char *param=(char*)malloc((unsigned long)(strlen(path)+strlen(pathgz)+20));
	
    //Concatenation of strings to make it look like .gz file path.
	strcat(pathgz,".gz");
	//Printing not to console, but to string value calleld "param".
	sprintf(param,"gzip -c %s > %s",path,pathgz);
	//Printing backup path to console.
	printf("%s ",path);

	//Detecting whether file exists.
	//F_OK - existance of file.
	if(access(pathgz,F_OK)){
	  //Detects whether "param" string can be executed.
	  i=system(param);
	  //Erasing memory.
	  free(param);
	  //Successfull message!
	  printf("- BackUpped\n");
	  //Returns signal if file is backuped.
	  return i;
	}

	//Detecting whether file can be read.
	//R_OK - readability of file.
	if(access(pathgz,R_OK)){
	  //Prints error with name of file if file can't be read.
	  perror(pathgz);
	  return -1;
	}

	//Assigning "stgz" variable with statistics for "pathgz" file of directory.
	i=stat(pathgz,&stgz);
	//Returns error if occured.
	if(i){
		return i;
	}
	//Detecting old files to update backup.
	if(mtime>stgz.st_mtime){
		//Detects whether "param" string can be executed.
		i=system(param);
		//Erasing memory.
		free(param);
		//Successfull message!
		printf("- BackUpped\n");
		return i;
	}
	//If file is already up-to-date, do nothing.
	printf("- Skipped\n");
	//Erases memory.
	free(param);

	//To get rid of warning: "Control may reach the end of non-void function".
	return 0;
}


//Pre-backup function that reads directories.
int read_dir(const char *_drname){

	//Directory variable.
    DIR* dr;
    //Structure for directory stream.
    struct dirent* nm;
    //Path to directory.
    char *path=(char*)malloc(strlen(_drname)+lmax+NAME_MAX+10);
    //Path to archive that will be created.
    char *pathgz=(char*)malloc(strlen(_drname)+lmax+NAME_MAX+10);
    
    char *relpath=(char*)malloc(strlen(_drname)+NAME_MAX+5);
    //Name of directory.
    char *drname=(char*)malloc(strlen(_drname)+lsrc+5);
    //Statistics structure to detect last access time and to detect access mode.
    struct stat st;
    //Temporary integer value.
    int i;
    //String to console formatting variable.
    char *s=(*_drname)?"%s/%s":"%s%s";
	

	//Printing full path to string "drname".
	sprintf(drname,s,src,_drname);
	//Opening directory.
	dr=opendir(drname);

	//If it is empty, we will have an error.
	if(!dr){
  		perror(drname);
		return -1;
	}
	
	//Entering directory.
	printf("Enter to dir %s .\n",drname);
	
	for(i=0;i<3;i++){ 
		//Reads contents of directory.
	 	nm=readdir(dr);
	}

	//While directory isn't empty...
	while(nm){
		sprintf(path,"%s/%s",drname,nm->d_name);
		i=stat(path,&st);
		if(i==-1){
			perror(path);
		}
		else{
			sprintf(pathgz,"%s/%s/%s",dest,_drname,nm->d_name);
			switch (st.st_mode&S_IFMT){
		 		case S_IFDIR:{
					mkdir(pathgz,0777);
					sprintf(relpath,s,_drname,nm->d_name);
					read_dir(relpath);
					break;
		       	}
		    	case S_IFREG:{
					i=backup(path,pathgz,st.st_mtime);
					break;
		       	}
		   	}	
	    }
		nm=readdir(dr);	
	 }

	//Closing directory. 
	closedir(dr);

	//Erasing all the memory.
	free(drname);
	free(path);
	free(pathgz);
	free(relpath);
	
	//Leaving directory.
	printf("Leaving dir %s\n",drname);
	return 0;
}


//Input format for our prgram should be like {source, destination}.
int main(int argn,char **argv){

	//Temporary integer value.
    int i;
   
   //If input format is wrong, make a message.   
   if (argn!=3){
        printf("%s\n","Usage: [source] [destination]");
        exit(-1);
    }
    //Looking for "/" at the end of string. 
    //If there is "/" then makes it "NILL".
    //"0" refers to ASCII's "NILL".
    i=strlen(argv[1])-1;
    if((argv[1][i]=='/')&&(i!=0)){ 
    	argv[1][i]=0;
    }  
    //Looking for "/" at the end of string. 
    //If there is "/" then makes it "NILL".
    //"0" refers to ASCII's "NILL".  
    i=strlen(argv[2])-1;
    if((argv[2][i]=='/')&&(i!=0)){ 
        argv[2][i]=0;
    }
    //strdup - non-standard C function. Creates copy of string in dynamic array (uses malloc) and returns pointer to it.
    //In this case we save "src" string for future use.
    src=strdup(argv[1]);  
    if (!src){ 
        return -2;
    }
    //strdup - non-standard C function. Creates copy of string in dynamic array (uses malloc) and returns pointer to it.
    //In this case we save "dest" string for future use.
    dest=strdup(argv[2]); 
    if(!dest){ 
        return -2;
    }
    //Comparing lengths of "src" and "dest" strings.
    lsrc=strlen(src);
    ldest=strlen(dest);
    //Detecting maximum value of "src" and "dest".
    lmax=((lsrc>ldest)?lsrc:ldest);
    //Creates new directory with "rwx" parameters.
	mkdir(dest,0777);
	//Return result, see read_dir() description for more info.
	return read_dir("");
}




