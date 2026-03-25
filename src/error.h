#ifndef ERROR_H
void handle_deprecation(const char* function_name);
void handle_load_error(const char* error, const char* function_name);
void handle_init_error(const char* error, const char* function_name);
void handle_init_failure(const char* function_name);
#define ERROR_H
#else
#warning "Multiple inclusions of error.h"
#endif
