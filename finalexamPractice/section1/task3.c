#include <dirent.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

void list_dir (const char * dir_name, int r, char *search) {
    DIR * d;
    d = opendir (dir_name);
    while (1) {
        struct dirent * entry;
        const char * d_name;
        entry = readdir(d);
        if (!entry)  break;    
        d_name = entry->d_name;
	    if(strcmp(d_name, search)==0) {
            printf("%s/%s\n", dir_name, d_name);
        }

        if (entry->d_type & DT_DIR) {
            if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
                char path[255];
                snprintf (path, 255, "%s/%s", dir_name, d_name);
                if(r==1) list_dir (path, 1, search);
            }
	    }
    }
    closedir (d);
    return;
}



int main(int argc, char *argv[]) {
    char dir[100];
    int f=0;
    if (argv[2] && strncmp(argv[2], "s", 1) == 0) {
        f = 1;
    }
    list_dir(getcwd(dir, 100), f, argv[1]);
    return 0;
}
