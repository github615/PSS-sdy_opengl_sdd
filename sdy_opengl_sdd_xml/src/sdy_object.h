#ifndef __SDY_OBJECT_H__
#define __SDY_OBJECT_H__

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
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "parsifal.h"
#include "common_definitions.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 函数声明 -----------------------------*/
void create_object_prop(object_t *object, char *prop_name);
void set_prop_init_value(parsing_state_t *state);
void set_prop_expr(parsing_state_t *state);
void handle_object_prop_xy(parsing_state_t *state, char *x_name, char *y_name);
void handle_object_prop(parsing_state_t *state, char *name);
void handle_orientation(LPXMLVECTOR atts, parsing_state_t *state);
void handle_static(LPXMLVECTOR atts, parsing_state_t *state);
void handle_point(parsing_state_t *state);
void handle_x(parsing_state_t *state);
void handle_y(parsing_state_t *state);
void handle_u(parsing_state_t *state);
void handle_v(parsing_state_t *state);
void handle_arc_segment(parsing_state_t *state);
void handle_first_point(parsing_state_t *state);
void handle_third_point(parsing_state_t *state);
void handle_first_arc(parsing_state_t *state);
void handle_second_arc(parsing_state_t *state);
void handle_third_arc(parsing_state_t *state);
void handle_fourth_arc(parsing_state_t *state);
void handle_line_width(parsing_state_t *state);
void handle_texture(LPXMLVECTOR atts, parsing_state_t *state);
void handle_ratio_prop(parsing_state_t *state, char *ratio_horizontal, char *ratio_vertical);
void handle_index_and_show_all(LPXMLVECTOR atts, parsing_state_t *state);
void handle_format(LPXMLVECTOR atts, parsing_state_t *state);
void handle_inactivemasks(LPXMLVECTOR atts, parsing_state_t *state);

void gen_variable_specification_table(
  xmlNodePtr father_node,
  variable_dictionary_info_t *info);
void gen_object_specification_table(
  xmlNodePtr father_node,
  object_t *object);
void gen_object_ref_parameter_table(
  xmlNodePtr father_node,
  object_t *object, 
  int count, 
  int first_ref_parameter_info_id);
void gen_mask_table(
  xmlNodePtr father_node,
  mask_info_t *mask_info);
void dump_table_length_info();

#endif    /*  __SDY_OBJECT_H__  */
