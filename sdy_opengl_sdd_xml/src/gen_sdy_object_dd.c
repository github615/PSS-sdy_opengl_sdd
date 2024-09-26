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
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "common_definitions.h"
#include "sdy_config.h"
#include "sdy_source.h"

/* --------------------------- 宏定义 -------------------------------*/
#define TABLE_VARIABLE_ROW_NUMBER 3
#define MAX_TABLE_LENGTH_INFO_SUMMARY 50

/* --------------------------- 类型定义 -----------------------------*/
typedef struct  
{
  int table_length;
  int table_count;
} table_length_info_t;

/* --------------------------- 变量定义 -----------------------------*/
extern char *bgcolor;
extern int sdd_image_number;

static table_length_info_t table_length_info_summary[MAX_TABLE_LENGTH_INFO_SUMMARY];
static int table_length_info_summary_size;

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void save_table_length_info(int table_length)
{
  int i;
  for (i = 0; i < table_length_info_summary_size; i++)
  {
    if (table_length_info_summary[i].table_length == table_length)
    {
      table_length_info_summary[i].table_count++;
      return;
    }
  }
  if (table_length_info_summary_size < MAX_TABLE_LENGTH_INFO_SUMMARY)
  {
    table_length_info_summary[table_length_info_summary_size].table_length = table_length;
    table_length_info_summary[table_length_info_summary_size].table_count = 1;
    table_length_info_summary_size++;
  }
  else
  {
    fatal_error("MAX_TABLE_LENGTH_INFO_SUMMARY is not enough\n");
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void dump_table_length_info()
{
  int i;
  for (i = 0; i < table_length_info_summary_size; i++)
  {
    printf("table_length=%d table_count=%d\n",
      table_length_info_summary[i].table_length,
      table_length_info_summary[i].table_count);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_variable_specification_table(
  xmlNodePtr father_node,
  variable_dictionary_info_t *info)
{
  xmlNodePtr table_node;
  xmlNodePtr row_node;

  table_node = xmlNewChild(father_node, NULL, BAD_CAST "var_spec_table", NULL);
  xmlNewProp(table_node, BAD_CAST "column_a", BAD_CAST "Property");
  xmlNewProp(table_node, BAD_CAST "column_b", BAD_CAST "Value");

  row_node = xmlNewChild(table_node, NULL, BAD_CAST "row", NULL);
  xmlNewProp(row_node, BAD_CAST "cell_a", BAD_CAST "Type");
  xmlNewProp(row_node, BAD_CAST "cell_b", BAD_CAST info->variable_type);

  row_node = xmlNewChild(table_node, NULL, BAD_CAST "row", NULL);
  xmlNewProp(row_node, BAD_CAST "cell_a", BAD_CAST "Representation");
  xmlNewProp(row_node, BAD_CAST "cell_b", BAD_CAST info->representation);

  row_node = xmlNewChild(table_node, NULL, BAD_CAST "row", NULL);
  xmlNewProp(row_node, BAD_CAST "cell_a", BAD_CAST "Init value");
  xmlNewProp(row_node, BAD_CAST "cell_b", BAD_CAST info->init_value);
  row_node = xmlNewChild(table_node, NULL, BAD_CAST "row", NULL);
  xmlNewProp(row_node, BAD_CAST "cell_a", BAD_CAST "Group");
  xmlNewProp(row_node, BAD_CAST "cell_b", BAD_CAST info->variable_group);

  /*2018-12-25按Esterel不用以下项
  row_node = xmlNewChild(table_node, NULL, BAD_CAST "row", NULL);
  xmlNewProp(row_node, BAD_CAST "cell_a", BAD_CAST "Control");
  xmlNewProp(row_node, BAD_CAST "cell_b", BAD_CAST info->variable_control);
  */
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_object_specification_table(
  xmlNodePtr father_node,
  object_t *object)
{
  object_prop_t *prop;
  xmlNodePtr table_node;
  xmlNodePtr row_node;
  char temp[100];
  int count = 0;

  table_node = xmlNewChild(father_node, NULL, BAD_CAST "req_table", NULL);
  xmlNewProp(table_node, BAD_CAST "type", BAD_CAST convert_tag_to_str(object->object_tag));
  xmlNewProp(table_node, BAD_CAST "column_a", BAD_CAST "Property");
  xmlNewProp(table_node, BAD_CAST "column_b", BAD_CAST "Value");
  xmlNewProp(table_node, BAD_CAST "column_c", BAD_CAST "Plugs expression");

  prop = object->prop_head;
  while (prop)
  {
    count++;
    prop = prop->next;
  }
  _itoa(count, temp, 10);
  xmlNewProp(table_node, BAD_CAST "row_count", BAD_CAST temp);
  save_table_length_info(count);

  prop = object->prop_head;
  while (prop)
  {
    row_node = xmlNewChild(table_node, NULL, BAD_CAST "row", NULL);
    xmlNewProp(row_node, BAD_CAST "cell_a", BAD_CAST prop->name);

    if (prop->value_and_expr.value)
    {
      xmlNewProp(row_node, BAD_CAST "cell_b", BAD_CAST prop->value_and_expr.value);
    }
    else
    {
      xmlNewProp(row_node, BAD_CAST "cell_b", NULL);
    }

    if (prop->value_and_expr.expr)
    {
      xmlNewProp(row_node, BAD_CAST "cell_c", BAD_CAST prop->value_and_expr.expr);
    }
    else
    {
      xmlNewProp(row_node, BAD_CAST "cell_c", NULL);
    }
   
    prop = prop->next;
  }

  if (object->indexes_info)
  {
    indexes_info_t *indexes = object->indexes_info;
    int i;
    row_node = xmlNewChild(table_node, NULL, BAD_CAST "row", NULL);
    xmlNewProp(row_node, BAD_CAST "cell_a", BAD_CAST "Indexes");
    temp[0] = 0;
    for (i = 0; i < indexes->condition_index_count; i++)
    {
      sprintf(temp + strlen(temp), "%d\r\n", indexes->condition_index_list[i]);
    }
    sprintf(temp + strlen(temp), "(default)%d", indexes->default_index);
    xmlNewProp(row_node, BAD_CAST "cell_b", BAD_CAST temp);
    xmlNewProp(row_node, BAD_CAST "cell_c", NULL);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_object_ref_parameter_table( 
  xmlNodePtr father_node,
  object_t *object,
  int count,
  int first_ref_parameter_info_id)
{
  ref_parameter_info_t *info = &ref_parameter_info_list[first_ref_parameter_info_id];
  xmlNodePtr table_node;
  xmlNodePtr row_node;
  char temp[5];

  _itoa(count, temp, 10);

  table_node = xmlNewChild(father_node, NULL, BAD_CAST "req_table", NULL);
  xmlNewProp(table_node, BAD_CAST "type", BAD_CAST "interface_parameters");
  xmlNewProp(table_node, BAD_CAST "column_a", BAD_CAST "Parameter");
  xmlNewProp(table_node, BAD_CAST "column_b", BAD_CAST "Value");
  xmlNewProp(table_node, BAD_CAST "column_c", BAD_CAST "Plugs expression");
  xmlNewProp(table_node, BAD_CAST "row_count", BAD_CAST temp);

  save_table_length_info(count);
  while (count--)
  {
    row_node = xmlNewChild(table_node, NULL, BAD_CAST "row", NULL);
    xmlNewProp(row_node, BAD_CAST "cell_a", BAD_CAST info->parameter_name);
    xmlNewProp(row_node, BAD_CAST "cell_b", info->init_value ? BAD_CAST info->init_value : NULL);
    xmlNewProp(row_node, BAD_CAST "cell_c", info->real_para_name ? BAD_CAST info->real_para_name : NULL);
    info++;
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_mask_table(
  xmlNodePtr father_node,
  mask_info_t *mask_info)
{
  xmlNodePtr table_node;
  xmlNodePtr row_node;

  table_node = xmlNewChild(father_node, NULL, BAD_CAST "mask_table", NULL);
  xmlNewProp(table_node, BAD_CAST "column_a", BAD_CAST "Applicable Mask");
  xmlNewProp(table_node, BAD_CAST "column_b", BAD_CAST "State");

  while (mask_info)
  {
    row_node = xmlNewChild(table_node, NULL, BAD_CAST "row", NULL);
    xmlNewProp(row_node, BAD_CAST "cell_a", BAD_CAST mask_info->mask->full_name_with_level_number);
    xmlNewProp(row_node, BAD_CAST "cell_b", BAD_CAST mask_info->inactivity ? "Inactive" : "Active");
    mask_info = mask_info->next_mask_info;
  }
}
 