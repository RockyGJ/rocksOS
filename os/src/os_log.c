/* -----------------------------------------------------------------------------
 * os_stream.c                                             (c) 2017 Rocks
 * -----------------------------------------------------------------------------
 * Author: Gertjan Rocks
 * Web:    www.gertjanrocks.com
 * Mail:   gertjanrocks@outlook.com
 * -----------------------------------------------------------------------------
 * Description: 
 * -----------------------------------------------------------------------------
 * Created on: Feb 24, 2017
 * -----------------------------------------------------------------------------
 */


/* --------------*
 * Include files *
 * --------------*
 */
#include "os.h"
#include <stdio.h>
#include <stdarg.h>
/* -------------------------------*
 * Constant and macro definitions *
 * -------------------------------*
 */


#define ANSI_RESET 		 "\x1B[0m"
#define ANSI_COLOUR_RED  "\x1B[31m"
#define ANSI_COLOUR_GRN  "\x1B[32m"
#define ANSI_COLOUR_YEL  "\x1B[33m"
#define ANSI_COLOUR_BLU  "\x1B[34m"
#define ANSI_COLOUR_MAG  "\x1B[35m"
#define ANSI_COLOUR_CYN  "\x1B[36m"
#define ANSI_COLOUR_WHT  "\x1B[37m"
/* -----------------*
 * Type definitions *
 * -----------------*
 */
 

/* ---------------------*
 * File-scope variables *
 * ---------------------*
 */

 
/* ----------------------*
 * Function declarations *
 * ----------------------*
 */


/* ---------------------*
 * Function definitions *
 * ---------------------*
 */

void os_log(os_log_level_t level, const char *fmt, ...) {
    va_list arg;
    //For now ignore log level
    if(level == os_log_level_os)    {
    	fprintf (stdout, ANSI_COLOUR_YEL);
    }
    //Write the log
    va_start(arg, fmt);
    vfprintf (stdout, fmt, arg);
    va_end(arg);

    if(level == os_log_level_os)    {
    	fprintf (stdout, ANSI_RESET);
    }

}
