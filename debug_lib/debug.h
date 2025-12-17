# ifndef DEBUG_H
# define DEBUG_H

#include <stdio.h>
#include "font_styles.h"

#define HTML_DEBUG_MODE

extern FILE* log_ptr;

enum err_t
{
    ok,
    error,
    null_data
};

enum md_t
{
    off,
    on
};

void initialize_log(const char* file_name, const char* log_title);
void end_debugging();

#if defined(TEXT_DEBUG_MODE) || defined(HTML_DEBUG_MODE)

#define printf_both(...) {                                                                      \
    printf(__VA_ARGS__);                                                                        \
    fprintf(log_ptr, __VA_ARGS__);                                                              \
}                                               

#define printf_debug_msg(...) {                                                                 \
    printf(__VA_ARGS__);                                                                        \
    fprintf(log_ptr, __VA_ARGS__);                                                              \
}

#define printf_log_only(...) {                                                                  \
    fprintf(log_ptr, __VA_ARGS__);                                                              \
}
#endif

#if defined(TEXT_DEBUG_MODE)

#define printf_log_bold(text, ...){                                                             \
    printf(MAKE_BOLD(text), __VA_ARGS__);                                                       \
    fprintf(log_ptr, text, __VA_ARGS__);                                                        \
}

#define printf_log_err(...) {                                                                   \
    printf("\n" MAKE_BOLD_RED("ERROR: ") __VA_ARGS__);                                          \
    fprintf(log_ptr, "\n" "ERROR: " __VA_ARGS__);                                               \
}

#define printf_log_warn(...) {                                                                  \
    printf("\n" MAKE_BOLD("WARNING: ") __VA_ARGS__);                                            \
    fprintf(log_ptr, "\n" "WARNING: " __VA_ARGS__);                                             \
}

#define printf_log_note(text, ...) {                                                            \
    printf("\n" MAKE_GREY("Note: " text), __VA_ARGS__);                                         \
    fprintf(log_ptr, "\n" "Note: " text , __VA_ARGS__);                                         \
}

#define printf_log_abortion(text, ...) {                                                        \
    printf("\n" MAKE_BOLD_RED(text), __VA_ARGS__);                                              \
    fprintf(log_ptr, "\n" text, __VA_ARGS__);                                                   \
}

#elif defined(HTML_DEBUG_MODE)

#define printf_log_bold(text, ...){                                                             \
    printf(MAKE_BOLD(text), __VA_ARGS__);                                                       \
    fprintf(log_ptr, "<b>" text "</b>", __VA_ARGS__);                                           \
}

#define printf_log_err(...) {                                                                   \
    printf("\n" MAKE_BOLD_RED("ERROR: ") __VA_ARGS__);                                          \
    fprintf(log_ptr, "\n" "<b><font color = \"#FF2B00\">ERROR:</font></b> " __VA_ARGS__);     \
}

#define printf_log_warn(...) {                                                                  \
    printf("\n" MAKE_BOLD("WARNING: ") __VA_ARGS__);                                            \
    fprintf(log_ptr, "\n" "<b>WARNING:</b> " __VA_ARGS__);                                      \
}

#define printf_log_note(text, ...) {                                                            \
    printf("\n" MAKE_GREY("Note: " text), __VA_ARGS__);                                         \
    fprintf(log_ptr, "\n" "<i><font color = \"#a9a9a9\">Note: " text "</i>", __VA_ARGS__);    \
}

#define printf_log_abortion(text, ...) {                                                        \
    printf("\n" MAKE_BOLD_RED(text), __VA_ARGS__);                                              \
    fprintf(log_ptr, "\n" "<b><font color = \"#FF2B00\">" text "</font></b> ", __VA_ARGS__);  \
}

#else

#define printf_both(...) printf(__VA_ARGS__);

#define printf_debug_msg(...)

#define printf_log_only(...)

#define printf_log_bold(text, ...)

#define printf_log_err(...) printf( MAKE_BOLD_RED("ERROR: ") __VA_ARGS__);

#define printf_log_warn(...) printf("\n" MAKE_BOLD("WARNING: ") __VA_ARGS__);

#define printf_log_note(text, ...) printf("\n" MAKE_GREY("Note: " text), __VA_ARGS__);

#define printf_log_abortion(text, ...) printf("\n" MAKE_BOLD_RED(text), __VA_ARGS__);

#endif

#endif

