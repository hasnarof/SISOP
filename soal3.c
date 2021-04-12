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
    int i=1;
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
    fp = fopen ("/home/ghifari/code/Modul-02/Soal Latihan/error.txt", "r");
    
    while (1){
        // fgets hanya membaca satu baris saja
        while(fgets (str, 100, fp) != NULL) {
            char num[100];
            //memasukkan kata ke dalam variabel tanpa harus menggunakan fungsi scanf() atau gets() dll.
            sprintf(num,"%d",i);
            
            char file[1000] ="/home/ghifari/code/Modul-02/Soal Latihan/error.log.";
            strcat (file,num);
            //search file and reading with w+

            log = fopen(file,"w+"); 
            //menginput string kedalam sebuah file, baik berupa tulisan normal ataupun kode format.
            fprintf(log,"%s",str);
            fclose(log);
            i++;
        }
        sleep(10);
        //menghapus diary
        remove ("/home/ghifari/code/Modul-02/Soal Latihan/error.txt");
         
    }
    exit(EXIT_SUCCESS);
}
