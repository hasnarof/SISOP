#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>


int main(){
    pid_t child_id;
    int status;
    child_id = fork ();
   time_t rawtime;
   struct tm*timeinfo;
    time(&rawtime);
    char my_time[100],namaFolder[100], path[100];
   timeinfo = localtime(&rawtime);
    strftime(my_time, sizeof(my_time), "%d-%m-%Y_%H:%M:%S", timeinfo);
    strcpy(namaFolder, my_time);
    strcpy(path,"/home/ghifari/code/Modul-02/Soal Latihan/");
	strcat(path,namaFolder);
    strcat(path, "/");
    if ( child_id == 0) {
       
        char *argv[] = {"mkdir", path, NULL};

        execv("/bin/mkdir", argv);
    }
    else {
        while ((wait(&status)) > 0);

        char *argv[] = {"cp","-r", "/home/ghifari/Downloads/", path, NULL};
        execv("/bin/cp", argv);
    }
}
