#ifndef MGK_LOGGER_H
#define MGK_LOGGER_H
#include <stdio.h>

typedef int errlvl_t;

#define DEBUG   0  // Just debug information
#define INFO    1  // Global information about process
#define WARNING 2  // Warning. Possible undefined / unplanned behaviour;
#define ERROR   3  // Error. Programm errored, but it is still able to accurate finish.
#define FATAL   4  // Fatal error. No actions can be done. It's crash.

enum class ConsoleOutputMode{
    NONE   = 0,
    SIMPLE = 1,
    FANCY  = 2,
};

enum class ConsoleStyle{
    NONE = 0,
    BOLD,
    UNDERSCORE,
    REVERSED,
    BLINK,
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
};

struct Logger
{
    FILE* logFile = NULL;
    ConsoleOutputMode consoleOutput = ConsoleOutputMode::FANCY;  
    int nTabs = 1;
};

class _AUTO_LOG_TAB{
    public:
    _AUTO_LOG_TAB();
    ~_AUTO_LOG_TAB();
};

#define $TAB _AUTO_LOG_TAB _mgk_auto_log_tab();

extern const Logger* const MGK_LOGGER;

#ifndef GLOBAL_LOG_LEVEL
#define GLOBAL_LOG_LEVEL DEBUG
#endif

#define LOG_MESSAGE(level, format, ...) logger_log(MGK_LOGGER, level, format, ## __VA_ARGS__)
#define LOG_STYLE(style) logger_style(MGK_LOGGER, style)
#define LOG_FILLER(...)  logger_filler(MGK_LOGGER, ##__VA_ARGS__);

#define LOG_INC_TAB(...) incTab(__VA_ARGS__)
#define LOG_DEC_TAB(...) decTab(__VA_ARGS__)
#define LOG_SET_TAB(...) setTab(__VA_ARGS__)

#if GLOBAL_LOG_LEVEL <= DEBUG

#define LOG_DEBUG(format, ...)   LOG_MESSAGE(DEBUG  , format, ## __VA_ARGS__)
#define LOG_INFO(format, ...)    LOG_MESSAGE(INFO   , format, ## __VA_ARGS__)
#define LOG_WARNING(format, ...) LOG_MESSAGE(WARNING, format, ## __VA_ARGS__)
#define LOG_ERROR(format, ...)   LOG_MESSAGE(ERROR  , format, ## __VA_ARGS__)
#define LOG_FATAL(format, ...)   LOG_MESSAGE(FATAL  , format, ## __VA_ARGS__)

#elif GLOBAL_LOG_LEVEL <= INFO

#define LOG_DEBUG(format, ...)   /******************************************/
#define LOG_INFO(format, ...)    LOG_MESSAGE(INFO   , format, ## __VA_ARGS__)
#define LOG_WARNING(format, ...) LOG_MESSAGE(WARNING, format, ## __VA_ARGS__)
#define LOG_ERROR(format, ...)   LOG_MESSAGE(ERROR  , format, ## __VA_ARGS__)
#define LOG_FATAL(format, ...)   LOG_MESSAGE(FATAL  , format, ## __VA_ARGS__)

#elif GLOBAL_LOG_LEVEL <= WARNING

#define LOG_DEBUG(format, ...)   /******************************************/
#define LOG_INFO(format, ...)    /******************************************/
#define LOG_WARNING(format, ...) LOG_MESSAGE(WARNING, format, ## __VA_ARGS__)
#define LOG_ERROR(format, ...)   LOG_MESSAGE(ERROR  , format, ## __VA_ARGS__)
#define LOG_FATAL(format, ...)   LOG_MESSAGE(FATAL  , format, ## __VA_ARGS__)

#elif GLOBAL_LOG_LEVEL <= ERROR

#define LOG_DEBUG(format, ...)   /******************************************/
#define LOG_INFO(format, ...)    /******************************************/
#define LOG_WARNING(format, ...) /******************************************/
#define LOG_ERROR(format, ...)   LOG_MESSAGE(ERROR  , format, ## __VA_ARGS__)
#define LOG_FATAL(format, ...)   LOG_MESSAGE(FATAL  , format, ## __VA_ARGS__)

#elif GLOBAL_LOG_LEVEL <= FATAL

#define LOG_DEBUG(format, ...)   /******************************************/
#define LOG_INFO(format, ...)    /******************************************/
#define LOG_WARNING(format, ...) /******************************************/
#define LOG_ERROR(format, ...)   /******************************************/
#define LOG_FATAL(format, ...)   LOG_MESSAGE(FATAL  , format, ## __VA_ARGS__)

#else 

#define LOG_DEBUG(format, ...)   /******************************************/
#define LOG_INFO(format, ...)    /******************************************/
#define LOG_WARNING(format, ...) /******************************************/
#define LOG_ERROR(format, ...)   /******************************************/
#define LOG_FATAL(format, ...)   /******************************************/

#endif

void logger_log(const Logger* logger, errlvl_t level, const char* format, ...);

void incTab(int n = 1);

void decTab(int n = 1);

void setTab(int n = 0);

void logger_set_file(const char* filename);

void logger_close_file();

void set_consoleOutput(ConsoleOutputMode val = ConsoleOutputMode::SIMPLE);

void logger_print_header(const Logger* logger, errlvl_t level);

const char* getLevelString(errlvl_t lvl);

const char* getStyleStr(ConsoleStyle color);

void logger_style(const Logger* logger, ConsoleStyle style);

void logger_filler(const Logger* logger, const char* text = NULL);
#endif