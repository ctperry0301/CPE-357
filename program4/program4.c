#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>

//Globals and Function declarations
int fd[2];
int overridemode=0;
int get_argument(char* line, int argn, char* result);
void myfct(int y);
void add_null_term(char *txt);
void recursiveSearch(char *path, int is_recursive, char *input);
char *finds;
int wasFound = 0;

//Core of assignment and logic
int main() {
    int *childpids = mmap(0,sizeof(int)*10,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    //char **inputArray; 
    //inputArray = mmap(0,10*sizeof(char*),PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANON,-1,0);  
    finds = (char *)malloc(100000);
    
    for(int i=0; i<10; i++) childpids[i]=0;

    signal(SIGUSR1,myfct);
    char input[1000];
    int parentPid = getpid();
    pipe(fd);
    int save_stdin = dup(STDIN_FILENO);
    while (1) {
        char cwd[200];
        char* path = getcwd(cwd, sizeof(cwd));
        printf("\nEnter a File, Directory, \"list\" or \"q\" or \"kill\" \n");
        printf("\033[0;34m"); //Set the text to the color Blue
        printf("Current Location: %s", path);
        printf("\033[0m"); //Resets the text to default color
        printf("$ ");
        fflush(0);
        dup2(save_stdin,STDIN_FILENO);
        overridemode = 0;
        read(STDIN_FILENO,input,1000);
        if (overridemode == 0)
            add_null_term(input);//to get a NULL at the end of the string in case of a user input
        if ((strcmp(input, "q\n") == 0) || (strcmp(input, "quit\n") == 0)) {
            for(int i=0;i<10;i++) {
                kill(childpids[i], SIGKILL);
                int a3;
                wait(&a3); //kill zombies
            }
            kill(parentPid, SIGKILL);
            exit(0);
        }
        else if (strncmp(input, "kill", 4) == 0) {
            char temp[1000];
            get_argument(input, 1, temp);
            int targetPid = atoi(temp);
            for(int i=0;i<10;i++) {
                if (targetPid == childpids[i]) {
                    waitpid(childpids[i],0,WNOHANG);
                    kill(childpids[i], SIGKILL);
                    childpids[i] = 0;
                    int a3;
                    wait(&a3); //kill zombies
                    printf("CHILD with PID %d was killed \n", targetPid);
                }
            }
        }
        //list kids and their pids
        else if (strcmp(input, "list\n") == 0) {
            printf("List of Children \n");
            printf("---------------------------\n");
            for(int i=0;i<10;i++) {
                if(childpids[i]!=0) {   
                    printf("Child #%d has the PID: %d\n", i, childpids[i]); 
                }
            }
        }
        else if (strncmp(input,"find",4) == 0) {
            if (fork() == 0) {
                //child process
                struct timespec start, end;
	            clock_gettime(CLOCK_REALTIME, &start);

                char childreport[10000];
                //search for an empty spot in the child list
                int kidnum = 0;

                for(int i=0;i<10;i++) if(childpids[i]==0) {
                    childpids[i]=getpid();
                    kidnum=i;
                    break;
                } 

                close(fd[0]); //close read   
                //finding stuff here...
                //sleep(25);
                char temp3[1000];
                char temp2[1000];
                char temp1[1000];
                char temp0[1000];
                char flag3[1000];
                char flag2[1000];
                char flag1[1000];
                char flag0[1000];
                
                //if input has 3 flags set
                int THREEFLAGS = get_argument(input, 3, temp3);
                strcat(flag3, temp3);
                //if input has 2 flags set
                int TWOFLAGS = get_argument(input, 2, temp2);
                strcat(flag2, temp2);
                //if input has 1 flag set
                int ONEFLAG = get_argument(input, 1, temp1);
                strcat(flag1, temp1);
                //Gets 0 flag
                int ZEROFLAG = get_argument(input, 0, temp0);
                strcat(flag0, temp0);

                if ((strncmp(flag2, "-s", 2) == 0) || (strncmp(flag3, "-s", 2) == 0)) {
                    //search through all directores
                    recursiveSearch(path, 1, flag1);
                }
                else {
                    //search through current directory
                    recursiveSearch(path, 0, flag1);
                }                
                clock_gettime(CLOCK_REALTIME, &end);
                double time_spent = (end.tv_sec - start.tv_sec) +
                                    (end.tv_nsec - start.tv_nsec) / 1000000000.0;
                sprintf(childreport,"\n\nKid %d is reporting after %f seconds!", kidnum, time_spent);
                if (wasFound == 1) {
                    strcat(childreport,"\nYour Input: ");
                    strcat(childreport, input);
                    strcat(childreport, "WAS FOUND INSIDE: ");
                    strcat(childreport, finds);
                    strcat(childreport,"\n\0");
                }
                else {
                    strcat(childreport,"\nYour Input: ");
                    strcat(childreport, input);
                    strcat(childreport, "WAS NOT FOUND");
                    strcat(childreport,"\n\0");
                }
                wasFound = 0;
                write(fd[1],childreport,strlen(childreport));
                close(fd[1]); //close write

                kill(parentPid,SIGUSR1);
                childpids[kidnum] = 0;
                return 0;
            }
        }
        else {
            int x = 0;
        }
        //killing the kid for good: if found
        for(int i=0;i<10;i++) {
            if(childpids[i]!=0) {   
                //printf("HI\n");   
                waitpid(childpids[i],0,WNOHANG);
            }
        }
        printf("input check: %s",input);
    }
    return 0;
}

void myfct(int y) {
    dup2(fd[0], STDIN_FILENO); //Overwrite userinput
    //printf("OVERWRITING USER INPUT\n\n");
    overridemode = 1;
}

void add_null_term(char *txt) {
    for(int i=0;i<100;i++) {
        if(txt[i]=='\n') {
            txt[i+1]=0;
            break;
        }
    }
}

// search file function 
void recursiveSearch(char *basePath, int is_recursive, char *input)
{
    char path[550];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    // Unable to open directory stream
    if (!dir)
        return;
    while ((dp = readdir(dir)) != NULL)
    { 
        if (strcmp(dp -> d_name, ".") != 0 && strcmp(dp -> d_name, "..") != 0)
        {
            if (strncmp(dp -> d_name, input, strlen(input)-1) == 0) {
                strcpy(finds, basePath);
                wasFound = 1;
            }
                 
            // Construct new path from our base path
            if (is_recursive == 1) {
                strcpy(path, basePath);
                strcat(path, "/");
                strcat(path, dp -> d_name);
            }
                strcat(finds, "\0");
            recursiveSearch(path, is_recursive, input);
        }       
    }
    closedir(dir);
}

// get argument function
int get_argument(char* line, int argn, char* result) {
	//firstly: remove all spaces at the front
	char temp[1000];
	int start_space = 1;
	for (int i = 0, u = 0; i <= strlen(line); i++)
		if (line[i] == ' ' && start_space == 1) continue;
		else {
			temp[u++] = line[i]; 
			start_space = 0;
		}
	//now remove an double or tripple spaces
	char temp2[1000];
	int space_on = 0;
	for (int i = 0, u = 0; i <= strlen(temp); i++) {
		if (space_on == 1 && temp[i] == ' ') continue;
		temp2[u++] = temp[i];
		if (temp[i] == ' ') space_on = 1;
		else space_on = 0;
    }
	//finally extract the arguments
	int start = 0;
	int end = 0;
	int count = 0;
	int quote = 0;
	for (int i = 0; i <= strlen(temp2); i++)
		if (temp2[i] == '\"') quote = !quote;
		else if (quote == 0 && (temp2[i] == ' ' || temp2[i] == 0)) {
			end = i;
			if (argn == count) {
				int length = end - start;
				strncpy(result, temp2 + start, length);
				result[length] = 0;
				return 1;
			}
			start = end + 1;
			count++;
		}
	return 0;
}