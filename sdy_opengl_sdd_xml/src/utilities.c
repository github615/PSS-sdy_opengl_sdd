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
#include <io.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "utilities.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
static FILE *error_log_file;

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void fatal_error(char *format_string, ...)
{
  va_list args;

  va_start(args, format_string);
  fprintf(stderr, "fatal error: ");
  vfprintf(stderr, format_string, args);
  if (error_log_file)
  {
    fprintf(error_log_file, "fatal error: ");
    vfprintf(error_log_file, format_string, args);
  }
  va_end(args);
#ifdef _DEBUG
  exit(-1);
#else
  exit(0);
#endif
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void warning_msg(char *format_string, ...)
{
  va_list args;

  va_start(args, format_string);
  fprintf(stderr, "warning: ");
  vfprintf(stderr, format_string, args);
  if (error_log_file)
  {
    fprintf(error_log_file, "warning: ");
    vfprintf(error_log_file, format_string, args);
  }
  va_end(args);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void set_error_log_file(char *error_log_file_name)
{
  error_log_file = fopen(error_log_file_name, "w");
  if (error_log_file == NULL)
  {
    fatal_error("Can't create %s\n", error_log_file_name);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void *get_mem (size_t size)
{
  void *p;
  p = malloc(size);
  if (p == NULL)
  {
    fatal_error("Get memory failed\n");
  }
#if 0
  if (((int)p&0xffff)==0xF0A8)
  {
    printf("get_mem=%x size=%d\n", p, size);
    SET_BREAK;
  }
#endif
  return (p);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void *get_cmem (size_t size)
{
  void *p;
  p = calloc(size, sizeof(char));
  if (p == NULL)
  {
    fatal_error("Get memory failed\n");
  }
#if 0
  if (((int)p&0xffff)==0xF0A8)
  {
    printf("get_cmem=%x\n", p);
    SET_BREAK;
  }
#endif
  return (p);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void *reget_mem (void *block, size_t size)
{
  void *p;
  p = realloc(block, size);
  if (p == NULL)
  {
    fatal_error("ReGet memory failed\n");
  }
  return (p);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void adjust_mem( void **block, int *block_count, int *block_size, int block_base_size, size_t item_size)
{
  if ((*block) == NULL)
  {
    *block_size = block_base_size;
    *block = get_mem(item_size * (*block_size));
  }
  else if (*block_count == (*block_size))
  {
    (*block_size) += ((*block_size) >> 1);
    *block = reget_mem((*block), item_size * (*block_size));
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void free_mem(void *mem)
{
  free(mem);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
int convert_str_to_enum(char *str, const str_and_enum_t *str_and_enum_list, int strcmp_or_strstr)
{
  str_and_enum_t *p = (str_and_enum_t *)str_and_enum_list;
  int enum_val = UNKNOWN_ENUM;
  int found;

  if (str) while(p->str)
  {
    if (strcmp_or_strstr == STRCMP)
    {
      found = (_stricmp(str, p->str) == 0);
    }
    else if (strcmp_or_strstr == STRSTR)
    {
      found = (strstr(str, p->str) != NULL);
    }
    else /* STRNCMP */
    {
      found = (strncmp(str, p->str, strlen(p->str)) == 0);
    }
    if (found)
    {
      enum_val = p->enum_val;
      break;
    }
    p++;
  }
  return (enum_val);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
char *convert_enum_to_str(int enum_val, const str_and_enum_t *str_and_enum_list)
{
  str_and_enum_t *p = (str_and_enum_t *)str_and_enum_list;

  while (p->str && p->enum_val != enum_val)
  {
    p++;
  }
  if (p->enum_val == 0)
  {
    fatal_error("convert_enum_to_str: can't find string for enum=%d\n", enum_val);
  }
  return (p->str);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
char *ltrim(char *str, char c)
{
  char *p = str;
  while (*p == c) p++;
  return p;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void rtrim(char *str, char c)
{
  char *p = str + strlen(str) - 1;
  while (*p == c)
  {
    *p = 0;
    p--;
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void convert_sql_str(char *old_str, char *new_str)
{
  char c;
  int j = 0;
  while (c = (*old_str++))
  {
    if (c == '\'')
    {
      new_str[j++] = '\'';
    }
    new_str[j++] = c;
  }
  new_str[j] = 0;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void check_buffer_overflow (unsigned current_size, unsigned max_size, char *size_name)
{
  if (current_size >= max_size)
  {
    fatal_error("%s is not enough\n", size_name);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void execute_dos_cmd(char *cmd, char *pipe_buffer, int buffer_size)
{
  FILE *pipe;
  size_t l;

  if( (pipe = _popen(cmd, "rt" )) == NULL )
  {
    fatal_error("DOS cmd \"%s\" is failed\n", cmd);
  }

/* Read pipe until end of file.
while( !feof( pipe ) )
{
if( fgets( pipe_buffer, 128, pPipe ) != NULL )
printf( pipe_buffer );
} */

  /*读最后一行:
  例如：
  D:\Program Files (x86)\Esterel Technologies\SCADE R15\SCADE Display\KCG64\bin\ScadeDisplayKCG.exe
  D:\Program Files (x86)\Esterel Technologies\SCADE R15\SCADE Display\KCG65\bin\ScadeDisplayKCG.exe
  */
  if( fgets(pipe_buffer, buffer_size, pipe) == NULL)
  {
    fatal_error("DOS cmd \"%s\" does not return anything\n", cmd);
  }
  fgets(pipe_buffer, buffer_size, pipe);//再读一行
  l = strlen(pipe_buffer); //删除结尾\n
  pipe_buffer[l-1] = 0;
  _pclose(pipe);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void convert_dir_format_for_mingw(char *cmd_in, char *cmd_out)
{
  char c;
  char *p_in = cmd_in;
  char *p_out = cmd_out;
  while (c = (*p_in++))
  {
    if (c == '\\')
    {
      (*p_out++) = '/';
    }
    else if (c == ' ' || c == '(' || c == ')')
    {
      (*p_out++) = '\\';
      (*p_out++) = c;
    }
    else
    {
      (*p_out++) = c;
    }
  }
  *p_out = 0;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void create_string_item(char **item, char *str)
{
  char *p = (char *)get_mem(strlen(str) + 1);
  strcpy(p, str);
  *item = p;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void strrpl(char* pDstOut, char* pSrcIn, const char* pSrcRpl, const char* pDstRpl)
{ 
  char* pi = pSrcIn; 
  char* po = pDstOut; 

  int nSrcRplLen = strlen(pSrcRpl); 
  int nDstRplLen = strlen(pDstRpl); 

  char *p = NULL; 
  int nLen = 0; 

  do 
  {
    // 找到下一个替换点
    p = strstr(pi, pSrcRpl); 

    if(p != NULL) 
    { 
      // 拷贝上一个替换点和下一个替换点中间的字符串
      nLen = p - pi; 
      memcpy(po, pi, nLen);

      // 拷贝需要替换的字符串
      memcpy(po + nLen, pDstRpl, nDstRplLen); 
    } 
    else 
    { 
      strcpy(po, pi); 

      // 如果没有需要拷贝的字符串,说明循环应该结束
      break;
    } 

    pi = p + nSrcRplLen; 
    po = po + nLen + nDstRplLen; 

  } while (p != NULL); 
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void convert_dir_format_for_windows(char *dir_name)
{
  char c;
  char *p = dir_name;
  while (c = (*p))
  {
    if (c == '/')
    {
      (*p) = '\\';
    }
    p++;
  }
}
