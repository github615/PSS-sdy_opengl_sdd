#ifndef __ASIDE_DECISION_H__
#define __ASIDE_DECISION_H__

/* --------------------------- ���� ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2013 �Ϻ���ΤѶ��Ϣ�������޹�˾��
 *
 * @��������
 *
 **********************************************************************
*/

/* --------------------------- �ļ����� -----------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include "aside_sdy_object_relation.h"

/* --------------------------- �궨�� -------------------------------*/

/* --------------------------- ���Ͷ��� -----------------------------*/

/* --------------------------- �������� -----------------------------*/
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

/* --------------------------- �������� -----------------------------*/

#endif    /*  __ASIDE_DECISION_H__  */
