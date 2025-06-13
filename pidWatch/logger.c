#include "logger.h"

cJSON *get_current_time(){
    time_t *now = NULL;
    char *time = NULL;

    now  = malloc(sizeof(time_t));
    time = asctime(localtime(now));

    cJSON *cJSON_logObject = cJSON_CreateObject();
    cJSON *current_time = cJSON_CreateString(time);

    cJSON_AddItemToObject(cJSON_logObject,"current_time",current_time);
    free(now);
    
    return cJSON_logObject;
}