/* --------------------------- 序言 ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2016, 2018 上海爱韦讯信息技术有限公司。
 *
 * @功能描述
 *
 **********************************************************************
*/

/* --------------------------- 文件包含 -----------------------------*/
#include <windows.h>
#include <stdio.h>
#include <direct.h>
#include "common_definitions.h"
#include "sdy_config.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
static char scade_display_dir[FILE_NAME_LENGTH];
static char scade_display_oglx_extras_dir[FILE_NAME_LENGTH];
static char ScadeDisplayCGSimu_exe[FILE_NAME_LENGTH];
static char ScadeDisplayKCG_exe[FILE_NAME_LENGTH];
static char scade_display_config_dir[FILE_NAME_LENGTH];
static char scade_suite_dir[FILE_NAME_LENGTH];

static char cmd[CMD_LENGTH];

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void search_scade_display_dir(void)
{
  char *p =getenv("SCADEDISPLAY_HOME");

  if (!p)
  {
    fatal_error("SCADE Display is not found\n");
  }
  else
  {
    strcpy(scade_display_dir, p);

    strcpy(scade_suite_dir, p);
    p = strrchr(scade_suite_dir, ' ');
    *p = 0;

    sprintf(ScadeDisplayCGSimu_exe, "%s\\bin\\ScadeDisplayCGSimu.exe", scade_display_dir);
    sprintf(ScadeDisplayKCG_exe, "%s\\KCG66\\bin\\ScadeDisplayKCG.exe", scade_display_dir);
    sprintf(scade_display_oglx_extras_dir, "%s\\KCG66\\oglx\\extras", scade_display_dir);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
char *get_scade_display_dir(void)
{
  return scade_display_dir;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
char *get_ScadeDisplayCGSimu_exe(void)
{
  return ScadeDisplayCGSimu_exe;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
char *get_ScadeDisplayKCG_exe(void)
{
  return ScadeDisplayKCG_exe;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
char *get_scade_display_config_dir(void)
{
  return scade_display_config_dir;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
char *get_scade_display_oglx_extras_dir(void)
{
  return scade_display_oglx_extras_dir;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
char *get_scade_suite_dir(void)
{
  return scade_suite_dir;
}
