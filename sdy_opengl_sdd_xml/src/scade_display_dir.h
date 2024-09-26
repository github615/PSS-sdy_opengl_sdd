#ifndef __SCADE_DISPLAY_DIR_H__
#define __SCADE_DISPLAY_DIR_H__

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

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 函数声明 -----------------------------*/
void search_scade_display_dir(void);
char *get_scade_display_dir(void);
char *get_ScadeDisplayCGSimu_exe(void);
char *get_ScadeDisplayKCG_exe(void);
char *get_scade_display_config_dir(void);
char *get_scade_display_oglx_extras_dir(void);
char *get_scade_suite_dir(void);

#endif    /*  __SCADE_DISPLAY_DIR_H__  */
