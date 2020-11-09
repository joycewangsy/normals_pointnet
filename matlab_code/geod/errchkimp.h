/*
 *  errchk - generic error check system
 *  Copyright (c) 2005-2010
 *      Oliver Matias van Kaick <ovankaic@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details (file COPYING).
 */


/* *************** */
/* * errchkimp.h * */
/* *************** */

#ifndef __ERRCHKIMP_H__
#define __ERRCHKIMP_H__


#ifndef _WIN32
#define _snprintf snprintf
#endif
        
/* Necessary includes */
#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef __ERRCHK_H__
#include <errchk.h>
#endif


/* Check if low-level error handling is possible */
#ifdef ERRCHK_COMPILE_LOW_LEVEL_ERROR_CHECK
#if HAVE_SIGNAL_H && HAVE_SETJMP_H 
#define ERRCHK_USE_LOW_LEVEL_ERROR_CHECK
#include <signal.h>
#include <setjmp.h>
#endif
#endif


/* Macros that add library name prefix */
#define ERRCHK_PREFIXATE(prefix, name) prefix##name
#define ERRCHK_PASS(prefix, name) ERRCHK_PREFIXATE(prefix, name)
#define ERRCHK_ADD_PREFIX(name)\
    ERRCHK_PASS(ERRCHK_PREFIX, name)

#define ERRCHK_STR(s) #s
#define ERRCHK_INT(s) ERRCHK_STR(lib##s)
#define ERRCHK_PASSSTR(s) ERRCHK_INT(s)
#define ERRCHK_NAME ERRCHK_PASSSTR(ERRCHK_PREFIX)


/* Error handling */
/* Error number */
errchk_t ERRCHK_ADD_PREFIX(_errcode);


/* Error string */
#define ERRCHK_STRING_SIZE  2000
#define ERRCHK_STACK_SIZE      5
#define errchk_reset\
    do {\
        ERRCHK_ADD_PREFIX(_errcode) = errchk_none;\
        _snprintf(ERRCHK_ADD_PREFIX(_error),\
                ERRCHK_STRING_SIZE, "No error");\
        _snprintf(ERRCHK_ADD_PREFIX(_cascade_error),\
                ERRCHK_STRING_SIZE, "No error");\
    } while (0)
#define errchk_set_error(merrcode, string, ...)\
    do {\
        ERRCHK_ADD_PREFIX(_errcode) = merrcode;\
        _snprintf(ERRCHK_ADD_PREFIX(_error), ERRCHK_STRING_SIZE,\
                "%s %s error: %s[%d]: %s(): "string,\
                ERRCHK_NAME,\
                ERRCHK_VERSION,\
                __FILE__ , __LINE__ ,\
               __FUNCTION__ , ##__VA_ARGS__);\
        if (ERRCHK_ADD_PREFIX(_error_func) != NULL)\
            ERRCHK_ADD_PREFIX(_error_func)(\
                    ERRCHK_ADD_PREFIX(_errcode),\
                    ERRCHK_ADD_PREFIX(_error));\
    } while (0)
#define errchk_cascade_error(merrcode, string, ...)\
    do {\
        ERRCHK_ADD_PREFIX(_errcode) = merrcode;\
        _snprintf(ERRCHK_ADD_PREFIX(_cascade_error), ERRCHK_STRING_SIZE,\
                "%s %s error: %s[%d]: %s(): "string": %s",\
                ERRCHK_NAME,\
                ERRCHK_VERSION,\
                __FILE__ , __LINE__ ,\
               __FUNCTION__ , ##__VA_ARGS__,\
                ERRCHK_ADD_PREFIX(_error));\
        _snprintf(ERRCHK_ADD_PREFIX(_error), ERRCHK_STRING_SIZE,\
                "%s",\
                ERRCHK_ADD_PREFIX(_cascade_error));\
        if (ERRCHK_ADD_PREFIX(_error_func) != NULL)\
            ERRCHK_ADD_PREFIX(_error_func)(\
                    ERRCHK_ADD_PREFIX(_errcode),\
                    ERRCHK_ADD_PREFIX(_error));\
    } while (0)
#define errchk_set_err(string, ...)\
    do {\
        ERRCHK_ADD_PREFIX(_errcode) = errchk_unknown;\
        _snprintf(ERRCHK_ADD_PREFIX(_error), ERRCHK_STRING_SIZE,\
                "%s %s error: %s[%d]: %s(): "string,\
                ERRCHK_NAME,\
                ERRCHK_VERSION,\
                __FILE__ , __LINE__ ,\
               __FUNCTION__ , ##__VA_ARGS__);\
        if (ERRCHK_ADD_PREFIX(_error_func) != NULL)\
            ERRCHK_ADD_PREFIX(_error_func)(\
                    ERRCHK_ADD_PREFIX(_errcode),\
                    ERRCHK_ADD_PREFIX(_error));\
    } while (0)
#define errchk_cascade_err(string, ...)\
    do {\
        ERRCHK_ADD_PREFIX(_errcode) = errchk_unknown;\
        _snprintf(ERRCHK_ADD_PREFIX(_cascade_error), ERRCHK_STRING_SIZE,\
                "%s %s error: %s[%d]: %s(): "string": %s",\
                ERRCHK_NAME,\
                ERRCHK_VERSION,\
                __FILE__ , __LINE__ ,\
               __FUNCTION__ , ##__VA_ARGS__,\
                ERRCHK_ADD_PREFIX(_error));\
        _snprintf(ERRCHK_ADD_PREFIX(_error), ERRCHK_STRING_SIZE,\
                "%s",\
                ERRCHK_ADD_PREFIX(_cascade_error));\
        if (ERRCHK_ADD_PREFIX(_error_func) != NULL)\
            ERRCHK_ADD_PREFIX(_error_func)(\
                    ERRCHK_ADD_PREFIX(_errcode),\
                    ERRCHK_ADD_PREFIX(_error));\
    } while (0)
#define errchk_push_error()\
    do {\
        if (ERRCHK_ADD_PREFIX(_error_stack_count) < ERRCHK_STACK_SIZE){\
            _snprintf(ERRCHK_ADD_PREFIX(_error_stack)[\
                        ERRCHK_ADD_PREFIX(_error_stack_count)],\
                    ERRCHK_STRING_SIZE,\
                    "%s", ERRCHK_ADD_PREFIX(_error));\
            ERRCHK_ADD_PREFIX(_error_stack_count)++;\
        }\
    } while (0)
#define errchk_pop_error()\
    do {\
        if (ERRCHK_ADD_PREFIX(_error_stack_count) > 0){\
            ERRCHK_ADD_PREFIX(_error_stack_count)--;\
            _snprintf(ERRCHK_ADD_PREFIX(_error),\
                    ERRCHK_STRING_SIZE, "%s", \
                    ERRCHK_ADD_PREFIX(_error_stack)[\
                        ERRCHK_ADD_PREFIX(_error_stack_count)]);\
        }\
    } while (0)

#ifdef ERRCHK_INCLUDE_CODE
char ERRCHK_ADD_PREFIX(_error)[ERRCHK_STRING_SIZE] = "No error";
char ERRCHK_ADD_PREFIX(_error_stack)[ERRCHK_STACK_SIZE][ERRCHK_STRING_SIZE];
int ERRCHK_ADD_PREFIX(_error_stack_count) = 0;
char ERRCHK_ADD_PREFIX(_cascade_error)[ERRCHK_STRING_SIZE] = "No error";
#else
extern char ERRCHK_ADD_PREFIX(_error)[];
extern char ERRCHK_ADD_PREFIX(_error_stack)[ERRCHK_STACK_SIZE][];
extern int ERRCHK_ADD_PREFIX(_error_stack_count);
extern char ERRCHK_ADD_PREFIX(_cascade_error)[];
#endif
/* User error function */
#ifdef ERRCHK_INCLUDE_CODE
errchk_func_t ERRCHK_ADD_PREFIX(_error_func) = NULL;
#else
extern errchk_func_t ERRCHK_ADD_PREFIX(_error_func);
#endif


/* Options */
#ifdef ERRCHK_INCLUDE_CODE
/* Check for low level errors */
char ERRCHK_ADD_PREFIX(_low_level_error_check) = 0;
/* Compiled with low-level error check possibility */
#ifdef ERRCHK_USE_LOW_LEVEL_ERROR_CHECK
char ERRCHK_ADD_PREFIX(_low_level_error_check_support) = 1;
#else
char ERRCHK_ADD_PREFIX(_low_level_error_check_support) = 0;
#endif
/* Library version */
char *ERRCHK_ADD_PREFIX(_version) = ERRCHK_VERSION;
#else
/* Check for low level errors */
extern char ERRCHK_ADD_PREFIX(_low_level_error_check);
/* Compiled with low-level error check possibility */
extern char ERRCHK_ADD_PREFIX(_low_level_error_check_support);
/* Library version */
extern char *ERRCHK_ADD_PREFIX(_version);
#endif


/* Macros */
#ifdef ERRCHK_USE_LOW_LEVEL_ERROR_CHECK
jmp_buf ERRCHK_ADD_PREFIX(_jmp_buf);
#define ERRCHK_BEGIN(return_value) \
    volatile sig_t ERRCHK_ADD_PREFIX(_sig);\
    int ERRCHK_ADD_PREFIX(_err);\
    do {\
        errchk_reset;\
        if (ERRCHK_ADD_PREFIX(_low_level_error_check)){\
            ERRCHK_ADD_PREFIX(_sig) = signal(SIGSEGV,\
                        ERRCHK_ADD_PREFIX(_signal));\
            if ((ERRCHK_ADD_PREFIX(_err) =\
                        setjmp(ERRCHK_ADD_PREFIX(_jmp_buf))) != 0){\
                switch(ERRCHK_ADD_PREFIX(_err)){\
                    case 1:\
                        errchk_set_error(errchk_sigsegv,\
                                "segmentation fault");\
                        break;\
                    default:\
                        errchk_set_error(errchk_unknown,\
                                "unknown fatal error");\
                        break;\
                }\
                signal(SIGSEGV, ERRCHK_ADD_PREFIX(_sig));\
                return return_value;\
            }\
        }\
    } while(0)
#define ERRCHK_END \
    do {\
        if (ERRCHK_ADD_PREFIX(_low_level_error_check)){\
            signal(SIGSEGV, ERRCHK_ADD_PREFIX(_sig));\
        }\
    } while(0)
#else
#define ERRCHK_BEGIN(return_value) \
    do {\
        errchk_reset;\
    } while (0)
#define ERRCHK_END    
#endif


/* Prototypes */
#ifdef ERRCHK_USE_LOW_LEVEL_ERROR_CHECK
void ERRCHK_ADD_PREFIX(_signal)(int sig);
#endif


/* Functions */
#ifdef ERRCHK_INCLUDE_CODE
#ifdef ERRCHK_USE_LOW_LEVEL_ERROR_CHECK
void ERRCHK_ADD_PREFIX(_signal)(int sig){

    switch (sig){
        case SIGSEGV:
            longjmp(ERRCHK_ADD_PREFIX(_jmp_buf), 1);
            break;
    }
    longjmp(ERRCHK_ADD_PREFIX(_jmp_buf), 2);
}
#endif
#endif


#endif /* __ERRCHKIMP_H__ */

/* ******* */
/* * End * */
/* ******* */
