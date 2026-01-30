#ifndef ERROR_H
void handle_init_failure(const char* function_name);
#define ERROR_H
#else
#warning "Multiple inclusions of error.h"
#endif
