#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define LOG_EMERG	0
#define LOG_ALERT	1
#define LOG_CRIT	2
#define LOG_ERR		3
#define LOG_WARN	4
#define LOG_NOTICE	5
#define LOG_INFO	6
#define LOG_DEBUG	7

#define LOG_LEVEL	7

#define STD_OUT stdout
#define STD_ERR stderr

#define log_msg(prio, stream, msg, ...) \
    do {\
        char *str;\
        if ((prio == LOG_EMERG) && (LOG_LEVEL >= LOG_EMERG)) {\
            str = "EMERG";\
            fprintf(stream, "[%s] : %s : %s : "msg" \n", str, __FILE__, __func__, ##__VA_ARGS__);\
        }\
        else if ((prio == LOG_ALERT) && (LOG_LEVEL >= LOG_ALERT)) {\
            str = "ALERT";\
            fprintf(stream, "[%s] : %s : %s : "msg" \n", str, __FILE__, __func__, ##__VA_ARGS__);\
        }\
        else if ((prio == LOG_CRIT) && (LOG_LEVEL >= LOG_CRIT)) {\
            str = "CRIT";\
            fprintf(stream, "[%s] : %s : %s : "msg" \n", str, __FILE__, __func__, ##__VA_ARGS__);\
        }\
        else if ((prio == LOG_ERR) && (LOG_LEVEL >= LOG_ERR)) {\
            str = "ERR";\
            fprintf(stream, "[%s] : %s : %s : "msg" \n", str, __FILE__, __func__, ##__VA_ARGS__);\
        }\
        else if ((prio == LOG_WARN) && (LOG_LEVEL >= LOG_WARN)) {\
            str = "WARN";\
            fprintf(stream, "[%s] : %s : %s : "msg" \n", str, __FILE__, __func__, ##__VA_ARGS__);\
        }\
        else if ((prio == LOG_NOTICE) && (LOG_LEVEL >= LOG_NOTICE)) {\
            str = "NOTICE";\
            fprintf(stream, "[%s] : %s : %s : "msg" \n", str, __FILE__, __func__, ##__VA_ARGS__);\
        }\
        else if ((prio == LOG_INFO) && (LOG_LEVEL >= LOG_INFO)) {\
            str = "INFO";\
            fprintf(stream, "[%s] : %s : %s : "msg" \n", str, __FILE__, __func__, ##__VA_ARGS__);\
        }\
        else if ((prio == LOG_DEBUG) && (LOG_LEVEL >= LOG_DEBUG)) {\
            str = "DEBUG";\
            fprintf(stream, "[%s] : %s : %s : "msg" \n", str, __FILE__, __func__, ##__VA_ARGS__);\
        }\
    } while(0)

//log_msg(LOG_ERR, STD_ERR, "\nError in opening %s", filename);

#ifdef __cplusplus
}
#endif

#endif
