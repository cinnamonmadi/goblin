#pragma once

static const int LOG_CHAR_WIDTH = 102;
static const int LOG_STR_SIZE = 128;
static const int LOG_SIZE = 7;

typedef struct Log{
    char** messages;
    int length;
    int position;
} Log;

Log* log_init();
void log_free(Log* log);
void log_push_back(Log* log, const char* message);
