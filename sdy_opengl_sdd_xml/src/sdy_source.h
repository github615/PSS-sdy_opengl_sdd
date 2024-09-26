#ifndef __SDY_SOURCE_H__
#define __SDY_SOURCE_H__

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
#include "common_definitions.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量声明 -----------------------------*/
extern language_t language;
extern const str_and_enum_t str_and_tag_list[];
extern int object_id;
extern object_t *root;
extern int object_count_for_main_source;
extern int object_count;
extern object_t **object_list;
extern int layer_info_count;
extern layer_info_t *layer_info_list;
extern int global_ogfx_info_count;
extern ogfx_info_t *global_ogfx_info_list;
extern int variable_dictionary_info_count;
extern variable_dictionary_info_t *variable_dictionary_info_list;
extern int ref_parameter_info_count;
extern ref_parameter_info_t *ref_parameter_info_list;
extern int indexes_count;

extern int codegen_include_path_count;
extern  char **codegen_include_path_list;
extern int codegen_source_path_count;
extern  char **codegen_source_path_list;

extern char sgfx_ratio_horizontal[ATTR_LENGTH];//2018-12-7
extern char sgfx_ratio_vertical[ATTR_LENGTH];//2018-12-7
extern char sgfx_dimension_height[ATTR_LENGTH];//2018-12-7
extern char sgfx_dimension_width[ATTR_LENGTH];//2018-12-7

/* --------------------------- 函数声明 -----------------------------*/
void set_language(char *language_seletion);
void parse_sdy_source(char *sdy_source_file_name, void *father_object, int inputs_scope_id);
void parse_trace_file(char *trace_file_name_in);
void enhance_layer_info(void);
void enhance_object_info(void);
void generate_aside_sdy_object_relation(void);
void generate_aside_sdy_object_info(void);
object_t *search_object_by_oid(char *oid);
void set_object_class_and_image_name(int object_id);
void set_specific_draw_callee(char *sgfx_ogfx_file_name);
char *get_specific_draw_callee(void);
char *convert_tag_to_str(sdy_source_tag_t tag);
void gen_sdd_xml(char *output_path, char *dd_file_name);
variable_dictionary_info_t *search_input_info_by_oid(char *input_oid);
void set_including_ogfx_children(char *option);
int get_including_ogfx_children(void);

void inherit_trace_comment(char *key_word);
void gen_ogfx_next_variable_dictionary_info_id(void);
void process_mask_info(void);

#endif    /*  __SDY_SOURCE_H__  */
