#include <assert.h>
#include "debug.h"
#include <stdlib.h>

FILE* log_ptr = NULL;
err_t global_err_stat = ok;

void initialize_log(const char* file_name, const char* log_title)
{
    log_ptr = fopen(file_name, "w");

    #ifdef TEXT_DEBUG_MODE
    
    printf_log_only("+++ %s +++\n\n", log_title);

    #endif
    #ifdef HTML_DEBUG_MODE

    system("rm -f dump_img/*.svg");
    system("rm -f dump_img/code/*.txt");

    printf_log_only("<pre>\n");
    printf_log_only("<h3> +++ %s +++ </h3>\n\n", log_title);

    #endif
}

void end_debugging()
{
    #if defined(HTML_DEBUG_MODE) ||  defined(TEXT_DEBUG_MODE)

    assert(log_ptr != NULL);
    fclose(log_ptr);

    #endif
}