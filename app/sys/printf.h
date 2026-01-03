#ifndef _PRINTF_H_
#define _PRINTF_H_

#ifdef __cplusplus
    extern "C" {  /* define c style exports for c plus plus*/
#endif

//#define DEBUG
#ifdef DEBUG
extern int db_printf(const char *format, ...);
#else
#define db_printf(fmt, ...)
#endif

#define API_DBG_LEVEL_NOW 0
 
typedef enum enAPI_DBG_LEVEL
{
    API_DBG_LEVEL_TIP = 1,
    API_DBG_LEVEL_WAR,
    API_DBG_LEVEL_ERR,
}API_DBG_LEVEL;
 
//API接口的调试数据打印
#define API_DBG(level,fmt, args...) \
        if (API_DBG_LEVEL_NOW > level) { \
            printf("API DEBUG(%s): " fmt, __func__, ##args); \
        } 
 
#define API_DBG_TIP(fmt, args...)	\
        if (API_DBG_LEVEL_NOW > API_DBG_LEVEL_TIP) { \
            printf("API DEBUG(%s): " fmt, __func__, ##args); \
        } 

#define API_DBG_WAR(fmt, args...)	\
        if (API_DBG_LEVEL_NOW > API_DBG_LEVEL_WAR) { \
            printf("API DEBUG(%s): " fmt, __func__, ##args); \
        } 

int v_printf(char* vbuf, const char *format, ...);
int mprintf(const char *format, ...);
int printf(const char *fmt, ...);


#ifdef __cplusplus
}
#endif

#endif
