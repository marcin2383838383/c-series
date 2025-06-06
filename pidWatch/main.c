#include "utils.h"

// #define DEBUG 1


int main()
{
    const char * monitored_dir = "/proc";
    const char * path = "proc_database";
    int sleep_time = 0.1;

    if(DEBUG)
    {
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
            printf("[*] tracking: %s\n",monitored_dir);
            track_DirectoryChanges(monitored_dir);
            sleep(sleep_time);
        };
    }else
    {
        if(assert_Database(path)){
            ;
        }
        else{
            init_Database();
        }

        while (true)
        {
            track_DirectoryChanges(monitored_dir);
            sleep(sleep_time);
        };
    }

    
    return 0;
}
