#include "utils.h"
#define DEBUG 1


int main()
{
    const char * path = "proc_database";
    
    if(assert_Database(path)){
        printf("[*] Databse exist!\n");
        printf("[*] Skipping ...\n");
    }
    else{
        printf("[*] Init database!\n"); 
        init_Database();
    }

    printf("[*] Start tracking %s\n",path);

    while (true)
    {
        printf("[*] tracking: %s\n","/proc");
        track_DirectoryChanges("/proc");
        sleep(5);
    };

    
    return 0;
}
