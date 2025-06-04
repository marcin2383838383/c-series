#include "utils.h"


bool assert_OpenDir(const char *path,DIR **dstDirStream)
{

    if(
        (*dstDirStream = opendir(path))== NULL
    ) {
        printf("assert_OpenDir: error, cant open directory -> %s",path);
        exit(-1);
    }
    return true;
}

void init_Database()
{

    DIR *dirStream = NULL;
    struct dirent *dir = NULL;
    char *path = "/proc";
    char *databasePath = "proc_database";
    int fd = 0;

    if( (fd = open(databasePath,O_WRONLY | O_CREAT | O_TRUNC,0755))==-1){
        printf("Can't open database file: %s with error code: %d\n", databasePath, errno);
    }

    assert_OpenDir(path,&dirStream);

    while ( (dir = readdir(dirStream)) != NULL) {

        char dirName[256]   = {0};
        size_t dirNameLen   = strlen(dir->d_name);
        size_t newDirNameLen    = strlen(dirName)+1;

        strncpy(dirName,dir->d_name, dirNameLen);
        strcat(dirName,"\n");
        write(fd,dirName,newDirNameLen);
        
    };
    close(fd);
};

bool assert_Database(const char* path)
{
    if(access(path,F_OK) == -1){
        return false;
    }
    return true;
}

bool has_NewEntry(const char* direntry,const char *path)
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

void add_NewEntry(const char* newPid)
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


void get_PidDetail(const char*pid){

    FILE *fStream = NULL;
    char path[256] = {0};
    strcat(path,"/proc/");
    strcat(path,pid);
    strcat(path,"/status");

    fStream = fopen(path,"r");
    if(fStream == NULL){
        printf("Error, can't open file: %s -->%s\n",path, errno);
    }else{
        char *line = NULL;
        size_t lineLen = 0;

        while((getline(&line,&lineLen,fStream))!=-1){
            if(strstr(line,"Name")!=NULL){
                printf("%s\n",line);
            }
        }
        free(line);
    }

    fclose(fStream);


}

void track_DirectoryChanges(const char *path)
{
    DIR *dirStream = NULL;
    struct dirent *dirNameAsPid = NULL;
    assert_OpenDir("/proc",&dirStream);


    while ( (dirNameAsPid = readdir(dirStream)) != NULL) {

            char pid[256] = {0};
            strncpy(pid,dirNameAsPid->d_name,strlen(dirNameAsPid->d_name));

            if(has_NewEntry(pid,"proc_database")){

                get_PidDetail(pid);
                printf("[*] New entry detect! --> %s\n", pid);
                add_NewEntry(pid);
            };
    };
    
}
