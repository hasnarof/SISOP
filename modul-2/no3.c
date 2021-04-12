#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(){
    FILE *fp;
    FILE *log;
    char str[100];
    int done =0;
    pid_t pid, sid;
    int i;
    pid = fork();

    if (pid < 0){
        exit (EXIT_FAILURE);
    }
    if (pid > 0){
        exit (EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
    if (sid <0){
        exit(EXIT_FAILURE);
    }
    if ((chdir("/")) < 0) {
   exit(EXIT_FAILURE);
 }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //open the file for reading only
    fp = fopen ("/home/hasna/kuliah/SISOP/modul-2/error.txt", "r");
    //search file and reading with w+
    while (1){
        // fgets hanya membaca satu baris saja
        while(fgets (str, 100, fp) != NULL) {
            char num[100];
            sprintf(num,"%d",i);
            
            char file[1000] ="/home/hasna/kuliah/SISOP/modul-2/error.log.";
            strcat (file,num);
            log = fopen(file,"w+"); 
          fprintf(log,"%s",str);
            fclose(log);
            i++;
        }
        sleep(10);
        remove ("/home/hasna/kuliah/SISOP/modul-2/error.txt");
         
    }
    exit(EXIT_SUCCESS);
}