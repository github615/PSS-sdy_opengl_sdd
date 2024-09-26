#ifndef __UTILITIES_H__
#define __UTILITIES_H__

/* --------------------------- 序言 ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2016 上海爱韦讯信息技术有限公司。
 *
 * @功能描述
 *
 **********************************************************************
*/

/* --------------------------- 文件包含 -----------------------------*/
#include <stdio.h>
#include <stdarg.h>

/* --------------------------- 宏定义 -------------------------------*/
#undef  bool
#undef  true
#undef  false
#undef  __bool_true_false_are_defined

#define  bool        _Bool
#define  true        1
#define  false       0
#define  __bool_true_false_are_defined  1

#if  __STDC_VERSION__ < 199901
typedef int  _Bool;      /* not built into pre-C99 compilers */
#endif

#define SET_BREAK printf("SET_BREAK\n")

#define UNKNOWN_ENUM 0
#define STRCMP 1
#define STRSTR 2
#define STRNCMP 3

/* --------------------------- 类型定义 -----------------------------*/
typedef struct
{
  char *str;
  int  enum_val;
} str_and_enum_t;

/* --------------------------- 函数声明 -----------------------------*/
void fatal_error (char *format_string, ...);
void warning_msg(char *format_string, ...);
void set_error_log_file(char *error_log_file_name);
void *get_mem (size_t size);
void *get_cmem (size_t size);
void *reget_mem (void *block, size_t size);
void adjust_mem( void **block, int *block_count, int *block_size, int block_base_size, size_t item_size);
void free_mem(void *mem);
int convert_str_to_enum(char *str, const str_and_enum_t *str_and_enum_list, int strcmp_or_strstr);
char *convert_enum_to_str(int enum_val, const str_and_enum_t *str_and_enum_list);
char *ltrim (char *str, char c);
void rtrim (char *str, char c);
void convert_sql_str (char *old_str, char *new_str);
void check_buffer_overflow (unsigned current_size, unsigned max_size, char *size_name);
void execute_dos_cmd(char *cmd, char *pipe_buffer, int buffer_size);
void convert_dir_format_for_mingw(char *cmd_in, char *cmd_out);
void convert_dir_format_for_windows(char *dir_name);
void create_string_item(char **item, char *str);
void strrpl(char* pDstOut, char* pSrcIn, const char* pSrcRpl, const char* pDstRpl);

#endif    /*  __UTILITIES_H__  */
