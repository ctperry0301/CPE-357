// Cole Perry
// CPE 357 Project 3
// Various Tasks including Forking, directories, and signaling.
// IF THE CHILD IS KILLED, THIS PROGRAM WILL WAIT 10 Seconds than create a new child
// Feb 20 2020

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <sys/dirent.h>
#include <sys/_types/_timespec.h>
#include <sys/types.h>

//Declared manually because #include <sys/sysmacros.h> doesnt work on mac
#define st_ctime st_ctimespec.tv_sec
#define st_atime st_atimespec.tv_sec
#define st_mtime st_mtimespec.tv_sec
#define S_IFMT 0170000
#define S_IFBLK 0060000
#define S_IFCHR 0020000
#define S_IFDIR 0040000
#define S_IFIFO 0010000
#define S_IFLNK 0120000
#define S_IFREG 0100000
#define S_IFREG 0100000

void perform_fork();
void attempted_term() {
    printf("Child Process Ended, waiting 10 seconds\n");
    sleep(10);
    perform_fork();
}  

void doNothing() {
    printf("");
}   

int main() {  
    signal(SIGINT, doNothing);
    signal(SIGTERM, doNothing);
    signal(SIGTSTP, doNothing);
    perform_fork();
    return 0;
}

void perform_fork() {
    int *childPID = mmap(NULL, 4, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0); //shared memory
    int *parentPID = mmap(NULL, 4, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0); //shared memory

    *parentPID = getpid();
    while (1) {
        if (fork() == 0) {
            //Prints User Location in Blue
            *childPID = getpid();
            while (1) {
                printf("Child PID: %i\n", *childPID);
                printf("Enter a File, \"list\" or \"q\" \n");
                printf("\033[0;34m"); //Set the text to the color Blue
                char cwd[100];
                printf("Current Location: %s",getcwd(cwd, sizeof(cwd)));
                printf("\033[0m"); //Resets the text to default color
                printf("$ ");
                fflush(0);

                char input[100];
                scanf("%s", input);
                fflush(0);
                //Quit program
                if (strcmp(input, "q") == 0) {
                    kill(*parentPID, SIGKILL);
                    kill(*childPID, SIGKILL);
                    int a1;
                    wait(&a1); //PREVENT ZOMBIES
                    exit(0);
                }
                //list contents of current directory 
                else if (strcmp(input, "list") == 0) {
                    printf("\nContent in Current Directory\n");
                    printf("---------------------------\n");
                    DIR *fileName;
                    struct dirent *dir;
                    fileName = opendir(".");
                    if (fileName) {
                        while ((dir = readdir(fileName)) != NULL) {
                            printf("%s\n", dir->d_name);
                        }
                        closedir(fileName);
                    }
                    printf("---------------------------\n");
                }
                //if user enters file
                else {
                    FILE *file;
                    file = fopen(input, "r");
                    if (file == NULL && input[0] != '/'){ 
                        printf("File Not Found\n");
                    }
                    else {
                        //for user to go back a directory
                        if (strcmp(input, "..") == 0) {
                            chdir(".."); 
                        }

                        //if user enters in a folder
                        else if(input[0] == '/') {
                            memmove(input, input+1, strlen(input));
                            if (chdir(input) != 0)  
                                perror("Folder does not exist");
                        }
                        else {
                            struct stat sb;
                            stat(input, &sb);
                            printf("\nInformation of %s\n", input);
                            printf("---------------------------\n");
                            // Here until next comment is 
                            // Copied from http://man7.org/linux/man-pages/man2/stat.2.html
                            printf("ID of containing device:  [%lx,%lx]\n", (long) major(sb.st_dev), (long) minor(sb.st_dev));
                            printf("File type:                ");
                            switch (sb.st_mode & S_IFMT) {
                            case S_IFBLK:  printf("block device\n");            break;
                            case S_IFCHR:  printf("character device\n");        break;
                            case S_IFDIR:  printf("directory\n");               break;
                            case S_IFIFO:  printf("FIFO/pipe\n");               break;
                            case S_IFLNK:  printf("symlink\n");                 break;
                            case S_IFREG:  printf("regular file\n");            break;
                            case S_IFSOCK: printf("socket\n");                  break;
                            default:       printf("unknown?\n");                break;
                            }
                            printf("I-node number:            %ld\n", (long) sb.st_ino);
                            printf("Mode:                     %lo (octal)\n", (unsigned long) sb.st_mode);
                            printf("Link count:               %ld\n", (long) sb.st_nlink);
                            printf("Ownership:                UID=%ld   GID=%ld\n", (long) sb.st_uid, (long) sb.st_gid);
                            printf("Preferred I/O block size: %ld bytes\n", (long) sb.st_blksize);
                            printf("File size:                %lld bytes\n", (long long) sb.st_size);
                            printf("Blocks allocated:         %lld\n", (long long) sb.st_blocks);
                            printf("Last status change:       %s", ctime(&sb.st_ctime));
                            printf("Last file access:         %s", ctime(&sb.st_atime));
                            printf("Last file modification:   %s", ctime(&sb.st_mtime));
                            // End of Copied Content  

                            printf("---------------------------\n");
                            printf("Information Printed\n");
                        }
                    }
                }
                printf("\n");
            }
        }
        //Parent
        else {
            int i;
            wait(&i);
            kill(*childPID, SIGKILL);
            int a1;
            wait(&a1); //PREVENT ZOMBIES
            attempted_term();
        }
    }
}
