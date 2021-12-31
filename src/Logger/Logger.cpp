#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include "Logger.h"

Logger MGK_LOGGER_OBJ = {NULL, ConsoleOutputMode::FANCY, 0};

const Logger* const MGK_LOGGER = &MGK_LOGGER_OBJ;

#define RAW(format, ...) {if(logger->consoleOutput != ConsoleOutputMode::NONE) fprintf(stderr, format, ## __VA_ARGS__); if(logger->logFile) {fprintf(logger->logFile, format, ## __VA_ARGS__); fflush(logger->logFile);}}
#define STYLE(style) logger_style(logger, style)

void incTab(int n){
    MGK_LOGGER_OBJ.nTabs += n;
}

void decTab(int n){
    MGK_LOGGER_OBJ.nTabs -= n;
}

void setTab(int n){
    MGK_LOGGER_OBJ.nTabs = n;
}

//*********************************************************************************************

void logger_set_file(const char* filename){
    if(MGK_LOGGER_OBJ.logFile)
        fclose(MGK_LOGGER_OBJ.logFile);

    if(filename == NULL){
        MGK_LOGGER_OBJ.logFile = NULL;
        return;
    }

    MGK_LOGGER_OBJ.logFile = fopen(filename, "a");
    atexit(logger_close_file);
}

void logger_close_file(){
    const Logger* logger = MGK_LOGGER;
    RAW("\n");

    if(MGK_LOGGER_OBJ.logFile){
        fclose(MGK_LOGGER_OBJ.logFile);
    }
}

//*********************************************************************************************


void set_consoleOutput(ConsoleOutputMode val){
    MGK_LOGGER_OBJ.consoleOutput = val;
}

//*********************************************************************************************

void logger_print_header(const Logger* logger, errlvl_t level){
    if(!logger) return;

    time_t curTime = time(NULL);

    const size_t timeStampSz = 10;
    char timeStamp[timeStampSz] = "";
    strftime(timeStamp, timeStampSz, "%H:%M:%S", localtime(&curTime));
    
    RAW("%s [",timeStamp);
    STYLE(ConsoleStyle::BOLD);
    switch(level){
        case DEBUG:   STYLE(ConsoleStyle::WHITE);   break;
        case INFO:    STYLE(ConsoleStyle::GREEN);   break;
        case WARNING: STYLE(ConsoleStyle::YELLOW);  break;
        case ERROR:   STYLE(ConsoleStyle::RED);     break;
        case FATAL:   STYLE(ConsoleStyle::RED);     break;
        default:                                    break;
    }
    RAW("%5s", getLevelString(level));
    STYLE(ConsoleStyle::NONE);
    RAW("] ");
}

const char* getLevelString(errlvl_t lvl){
    switch (lvl)
    {
    case DEBUG:   return "DEBUG";
    case INFO:    return "INFO";    
    case WARNING: return "WARN";
    case ERROR:   return "ERROR";
    case FATAL:   return "FATAL";
    default:
        return NULL;
    }
}

#define STYLE_APPLY(style, x) case ConsoleStyle::style: return "\033[" #x "m"
const char* getStyleStr(ConsoleStyle color){
    switch(color){
        STYLE_APPLY(NONE,       0);
        STYLE_APPLY(BOLD,       1);
        STYLE_APPLY(UNDERSCORE, 4);           
        STYLE_APPLY(REVERSED,   7);       
        STYLE_APPLY(BLINK,      5);       
        STYLE_APPLY(BLACK,      30);       
        STYLE_APPLY(RED,        31);   
        STYLE_APPLY(GREEN,      32);       
        STYLE_APPLY(YELLOW,     33);       
        STYLE_APPLY(BLUE,       34);   
        STYLE_APPLY(MAGENTA,    35);       
        STYLE_APPLY(CYAN,       36);   
        STYLE_APPLY(WHITE,      37);       
        default:
            return NULL;
    }
}
#undef STYLE_APPLY

void logger_style(const Logger* logger, ConsoleStyle style){
    if(logger && logger->consoleOutput == ConsoleOutputMode::FANCY){
        fprintf(stderr, "%s", getStyleStr(style));
    }
}
//*********************************************************************************************


void logger_log(const Logger* logger, errlvl_t level, const char* format, ...){
    if(!logger || !format) return;

    va_list ap, ap2;
    va_start(ap, format);
    va_copy(ap2, ap);
    
    int shift = 0;

    if(*format != '\b'){
        RAW("\n");
        logger_print_header(logger, level);
        
        for(int i = 0; i < logger->nTabs; ++i) RAW("\t");
        
    }
    else shift = 1;

    if(logger->consoleOutput != ConsoleOutputMode::NONE)
        vfprintf(stderr,          format + shift, ap);
    if(logger->logFile)
        vfprintf(logger->logFile, format + shift, ap2);
    va_end(ap);
    va_end(ap2);
    fflush(logger->logFile);
}

void logger_filler(const Logger* logger, const char* text){
    if(text == NULL) text = "";
    RAW("\n\n\n#######################################%s#####################################\n\n", text);
}

_AUTO_LOG_TAB::_AUTO_LOG_TAB(){
    incTab();
}

_AUTO_LOG_TAB::~_AUTO_LOG_TAB(){
    decTab();
}