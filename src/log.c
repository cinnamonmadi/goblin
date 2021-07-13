#include "log.h"

#include <string.h>
#include <stdlib.h>

Log* log_init(){

    Log* log = malloc(sizeof(Log));
    log->messages = malloc(sizeof(char*) * LOG_SIZE);
    for(int i = 0; i < LOG_SIZE; i++){

        log->messages[i] = malloc(sizeof(char) * LOG_STR_SIZE);
        strcpy(log->messages[i], "\0");
    }
    log->length = 0;
    log->position = -1;

    return log;
}

void log_free(Log* log){

    for(int i = 0; i < LOG_SIZE; i++){

        free(log->messages[i]);
    }
    free(log->messages);
    free(log);
}

void log_push_back(Log* log, const char* message){

    if(log->length < LOG_SIZE){

        strcpy(log->messages[log->length], message);
        log->position++;
        log->length++;

        return;
    }

    log->position = (log->position + 1) % LOG_SIZE;
    strcpy(log->messages[log->position], message);
}
