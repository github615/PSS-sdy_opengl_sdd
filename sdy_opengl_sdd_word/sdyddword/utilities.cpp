/*-------------------------- FILE PROLOGUE --------------------------*/

/**
 **********************************************************************
 * @notice
 * (C) 2018 SHAVIATION
 *
 * @brief
 *
 **********************************************************************
*/

/*------------------------- FILE INCLUSION --------------------------*/
#include "stdafx.h"

/* ----------------------- MACRO DEFINITIONS ------------------------*/

/* ----------------------- TYPE DEFINITIONS -------------------------*/

/* ---------------------- OBJECT DEFINITIONS ------------------------*/
static LCID lcid;
static FILE *log_file;

/* ---------------------- FUNCTION DEFINTIONS -----------------------*/
/**
 **********************************************************************
 *
 * @brief
 *
 **********************************************************************
*/
void init_lcid()
{
  lcid = ::GetThreadLocale();
}
/**
 **********************************************************************
 *
 * @brief
 *
 **********************************************************************
*/
void check_file_existence(TCHAR *file_name)
{
  if (_taccess(file_name, 0) == -1)
  {
    fatal_error(_T("check_file_existence: %s does not exists\n"), file_name);
  }
}
/**
 **********************************************************************
 *
 * @brief
 *
 **********************************************************************
*/
void change_to_output_dir(TCHAR *output_dir)
{
  char output_dir_char[MAX_PATH];
  int len = WideCharToMultiByte(CP_ACP, 0, output_dir, -1, NULL, 0, NULL, NULL);
  WideCharToMultiByte(CP_ACP, 0, output_dir, -1, output_dir_char, len, NULL, NULL);
  _chdir(output_dir_char);
}
/**
 **********************************************************************
 *
 * @brief
 *
 **********************************************************************
*/
void open_log_file(TCHAR *outputDirName, TCHAR *logFileName)
{
  TCHAR fullLogFileName[MAX_PATH * 2];
  _stprintf(fullLogFileName, _T("%s\\%s"), outputDirName, logFileName);
  log_file = _tfopen(fullLogFileName, _T("w"));
}
/**
 **********************************************************************
 *
 * @brief
 *
 **********************************************************************
*/
void close_log_file()
{
  if (log_file)
  {
    fclose(log_file);
    log_file = NULL;
  }
}
/**
 **********************************************************************
 *
 * @brief
 *
 **********************************************************************
*/
void dump_memory(BYTE *memory, int bytes)
{
  if (log_file)
  {
    for (int i = 0; i < bytes; i++)
    {
      if ((i % 16) == 0)
      {
        fprintf(log_file, "\n%04X:", i);
      }
      fprintf(log_file, " %02X", *memory++);
    }
    fprintf(log_file, "\n");
  }
}
/**
 **********************************************************************
 *
 * @brief
 *
 **********************************************************************
*/
void dump_log_file(TCHAR *format_string, ...)
{
  if (log_file)
  {
    va_list args;
    va_start(args, format_string);
    _vftprintf_s(stderr, format_string, args);
    _vftprintf_s(log_file, format_string, args);
    va_end(args);
  }
}
/**
 **********************************************************************
 *
 * @brief
 *
 **********************************************************************
*/
void fatal_error(TCHAR *format_string, ...)
{
  va_list args;

  va_start(args, format_string);
  _vftprintf(stderr, format_string, args);
  if (log_file)
  {
    _vftprintf(log_file, format_string, args);
  }
  va_end(args);
  close_log_file();
  exit(-1);
}
/**
**********************************************************************
* @brief
*
**********************************************************************
*/
void *get_cmem (size_t size)
{
  void *p;
  p = calloc(size, sizeof(char));
  if (p == NULL)
  {
    fatal_error(_T("Get memory failed\n"));
  }
#if 0
  if (((int)p&0xffff)==0x4DC8)
  {
    printf("get_cmem=%x size=%d\n", p, size);
    SET_BREAK;
  }
#endif
  return (p);
}
/**
**********************************************************************
* @brief
*
**********************************************************************
*/
void free_mem(void *mem)
{
  free(mem);
}
