#define _GNU_SOURCE
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>


bool assertOpenDir(const char *path,DIR **dstDirStream){

    if((*dstDirStream = opendir(path))== NULL) {
        printf("opendir: error, cant open directory -> %s",path);
        return false;
    }
    return true;
}

void initDatabase()
{
    printf("[*] Init database!\n"); 
    DIR *dirStream = NULL;
    struct dirent *dir = NULL;
    char *path = "/proc";
    char *databasePath = "proc_database";
    int filePointer = 0;

    filePointer = open(databasePath,O_WRONLY | O_CREAT | O_TRUNC,0755);

    if(assertOpenDir(path,&dirStream));
    int totalByteWroiten = 0;
    while ( (dir = readdir(dirStream)) != NULL) {

        char tmpDir[256] = {0};
        size_t dirLen = strlen(dir->d_name);
        size_t newDirLen=0;
        strncpy(tmpDir,dir->d_name, dirLen);
        newDirLen = strlen(tmpDir);
        strcat(tmpDir,"\n");

        write(filePointer,tmpDir,newDirLen+1);


    };
    close(filePointer);
};

bool assertDatabase(const char* path){

    if(access(path,F_OK) == -1){
        return false;
    }
    return true;

}

bool hasNewEntry(const char* direntry,const char *path)
{

    bool newPidDetected = true;
    FILE *filePointer = NULL;
    size_t n = 0;

    if((filePointer = fopen(path,"r"))== NULL){
        printf("openDatabaseHandle() Errno: %d",errno); 
    };

    char *line= NULL;
    size_t readBytes = 0;

    while (
        getline(&line,&readBytes,filePointer)!=-1 &&  newPidDetected
    ){

        char newString[256] = {0};
        int lineLen = strlen(line);
      
        strncpy(newString,line,lineLen-1);
        if(strcmp(newString,direntry)==0){
            newPidDetected= false;
        };
    }
    fclose(filePointer);

    return newPidDetected;
}

void addNewEntry(const char* newPid)
{
    char pidNewLineC[256] = {0};
    int newEntryWithNewlineChar_len = 0;
    int newPid_len = strlen(newPid);
    newEntryWithNewlineChar_len = open("proc_database",O_WRONLY | O_APPEND,0755);
    strcpy(pidNewLineC,newPid);
    strcat(pidNewLineC,"\n");

    // +1 for newline char
    write(newEntryWithNewlineChar_len,pidNewLineC,newPid_len+1);
    close(newEntryWithNewlineChar_len);
}

void trackDirectoryChanges(const char *path)
{
    DIR *dirStream = NULL;
    struct dirent *dirNameAsPid = NULL;
    if(assertOpenDir("/proc",&dirStream));


    while ( (dirNameAsPid = readdir(dirStream)) != NULL) {
            if(hasNewEntry(dirNameAsPid->d_name,"proc_database")){
                printf("[*] New entry detect! --> %s\n", dirNameAsPid->d_name);
                addNewEntry(dirNameAsPid->d_name);
            };
    };
    
}

int main()
{
    const char * path = "proc_database";
    
    if(assertDatabase(path)){
        printf("[*] Databse exist!\n");
        printf("[*] Skipping ...\n");
    }
    else{
        initDatabase();
    }

    printf("[*] Start tracking %s\n",path);

    while (true)
    {
        printf("[*] tracking: %s\n","/proc");
        trackDirectoryChanges("/proc");
        sleep(5);
    };

    
    return 0;
}

