#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static  const  char *dirpath = "/home/erzajanitra/Downloads";

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];
    //kalau merupakan directory
    // / --> cwd lat1.c skrg
    if(strcmp(path,"/")==0){
        //wd jadi relatif, diubah ke downloads
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    //tunjuk ke file
    else{
        //ambil nama asli
        //biar ilangg /
        char *namaAsli= strrchr(path,'/')+1;
        char dirAsli[1024];
        strncpy(dirAsli,path,strlen(path)-strlen(namaAsli));//dapetin nama folder
        
        //remove jago--> cek nama file yg ada jago_
        char *temp=0;
        char nama[1024];

        if(strncmp(namaAsli,"Jago_",5)==0){
            temp=strchr(namaAsli,'_')+1; //ambil nama file asli
        }
        //kalo namanya gaada Jagonya
        if(!temp){
            strcpy(nama,namaAsli);
        }
        //kalo namanya ada jagonya
        else{
            strcpy(nama,temp);
        }
        sprintf(fpath,"%s%s%s",dirpath,dirAsli,nama);//path asal yg mau diambil
    }
    res = lstat(fpath, stbuf);

    if (res == -1) return -errno;

    return 0;
}


//ubah nama file
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    } 
    else sprintf(fpath,"%s%s",dirpath,path);
    
    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;//mode file

        //cek file apa bukan 
        if(de->d_type & DT_DIR){
            //directory
            res = (filler(buf, de->d_name, &st, 0));
        }
        //file
        else{
            char buffer[1024];
            sprintf(buffer,"Jago_%s",de->d_name);
            res = (filler(buf, buffer, &st, 0));
        }
        
        //filler : ngisi buffer dgn nama file
        if(res!=0) break;
    }

    closedir(dp);

    return 0;
}


//baca isi file
static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;

        sprintf(fpath,"%s",path);
    }
    else {
        char *namaAsli= strrchr(path,'/')+1;
        char dirAsli[1024];
        //ambil nama dir
        strncpy(dirAsli,path,strlen(path)-strlen(dirAsli));
        
        //remove jago
        char *temp;
        if(strncmp(namaAsli,"Jago_",5)==0){ //cek 5 index pertama di string
            temp=strchr(namaAsli,'_')+1;
        }
        char nama[1024];
   
        //kalo ga ada nama jago
        if(!temp){
            strcpy(nama,namaAsli);
        }
        //kalo namanya ada jagonya
        else{
            strcpy(nama,temp);
        }
        sprintf(fpath,"%s%s%s",dirpath,dirAsli,nama);
        
    }
    int res = 0;
    int fd = 0 ;

    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}



static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,//ambil atribut
    .readdir = xmp_readdir,//directory yg kita minta
    .read = xmp_read,
};



int  main(int  argc, char *argv[])
{
    umask(0);

    return fuse_main(argc, argv, &xmp_oper, NULL);
}
