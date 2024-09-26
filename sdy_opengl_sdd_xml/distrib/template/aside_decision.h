#ifndef __ASIDE_DECISION_H__
#define __ASIDE_DECISION_H__

/* --------------------------- 序言 ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2013 上海爱韦讯信息技术有限公司。
 *
 * @功能描述
 *
 **********************************************************************
*/

/* --------------------------- 文件包含 -----------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include "aside_sdy_object_relation.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量声明 -----------------------------*/
void open_log_file(char *image_file_dir_name);
void close_log_file(void);
int get_active_object_id(void);
int increase_active_object_id(void);
int aside_sdy_is_enable(int object_id);
int aside_sdy_in_same_scope(int object_id);
int aside_sdy_condition_expr(int condition_expr_id);
void reset_parameters(void);
int get_active_object_is_reached(void);
void log_object_is_not_reached(int object_id);
void log_object_summary(int active_object_id);
void log_message(char *message);

/* --------------------------- 函数声明 -----------------------------*/

#endif    /*  __ASIDE_DECISION_H__  */
