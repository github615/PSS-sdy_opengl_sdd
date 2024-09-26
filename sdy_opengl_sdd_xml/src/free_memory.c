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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common_definitions.h"
#include "sdy_config.h"
#include "sdy_source.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void free_object(object_t *object)
{
  object_prop_t *prop = object->prop_head;
  object_prop_t *temp;

  free_mem(object->full_name_with_slash);
  free_mem(object->father_list);
  if (object->oid)
  {
    free_mem(object->oid);
  }
  if (object->comment)
  {
    free_mem(object->comment);
  }
  if (object->traceinfo)
  {
    free_mem(object->traceinfo);
  }
  if (object->indexes_info)
  {
    free_mem(object->indexes_info->condition_index_list);
    free_mem(object->indexes_info);
  }

  if (object->mask_info)
  {
    mask_info_t *m = object->mask_info;
    mask_info_t *t;
    while (m)
    {
      t = m;
      m = m->next_mask_info;
      free_mem(t);
    }
  }
  free_mem(object->inactive_mask_list);

  while (prop)
  {
    free_mem(prop->name);
    free_mem(prop->value_and_expr.value);
    free_mem(prop->value_and_expr.expr);
    temp = prop;
    prop = prop->next;
    free_mem(temp);
  }
  free_mem(object);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void free_objects(void)
{
  int i;
  for (i = 0; i < object_count; i++)
  {
    object_t *object = object_list[i];
    free_object(object);
  }
  free_mem(object_list);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void free_codegen_include_path(void)
{
  int i;
  for (i = 0; i < codegen_include_path_count; i++)
  {
    free_mem(codegen_include_path_list[i]);
  }
  free_mem(codegen_include_path_list);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void free_codegen_source_path(void)
{
  int i;
  for (i = 0; i < codegen_source_path_count; i++)
  {
    free_mem(codegen_source_path_list[i]);
  }
  free_mem(codegen_source_path_list);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void free_input_output_info(void)
{
  int i;
  variable_dictionary_info_t *info;
  for (i = 0, info = variable_dictionary_info_list; i < variable_dictionary_info_count; i++, info++)
  {
    free_mem(info->init_value);
  }
  free_mem(variable_dictionary_info_list);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void free_ref_parameter_info_info(void)
{
  int i;
  ref_parameter_info_t *info;
  for (i = 0, info = ref_parameter_info_list; i < ref_parameter_info_count; i++, info++)
  {
    free_mem(info->init_value);
    free_mem(info->real_para_name);
  }
  free_mem(ref_parameter_info_list);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void free_memory(void)
{
  free_objects();
  free_codegen_include_path();
  free_codegen_source_path();
  free_mem(layer_info_list);
  free_input_output_info();
  free_ref_parameter_info_info();
  free_mem(global_ogfx_info_list);
  free_color_table();
  free_line_width_table();
  free_line_stipple_table();
  free_texture_table();
  free_font_table();
}