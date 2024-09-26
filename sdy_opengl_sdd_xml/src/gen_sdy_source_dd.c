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
#include "sdy_config.h"
#include "sdy_object.h"
#include "sdy_source.h"
#include "convert_xml_code.h"
#include "process_file_and_dir.h"

/* --------------------------- 宏定义 -------------------------------*/
#define WORDS_LENGTH 50
#define MAX_LINE_LENGTH 200
#define MAX_HIERARCHY_FIGURE_BUFFER_SIZE 500

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
language_t language = ENGLISH;

static xmlDocPtr doc;
static char node_title_buffer[MAX_TEXT_BUFFER];
static char node_content_buffer[MAX_TEXT_BUFFER];
static int sdd_image_number;
static int sdd_table_number;
static char hierarchy_figure_buffer[MAX_HIERARCHY_FIGURE_BUFFER_SIZE][MAX_LINE_LENGTH];
static int hierarchy_figure_buffer_count;

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void set_language(char *language_seletion)
{
  if (_stricmp(language_seletion, "chinese") == 0)
  {
    language = CHINESE;
  }
  else
  {
    language = ENGLISH;
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_object_image(
  xmlNodePtr father_node,
  object_t *object)
{
  if (object->image_name[0])
  {
    xmlNodePtr image_node;
    if (language == CHINESE)
    {
      char temp[100];
      sprintf(temp, "图%d是%s的参考图像。", 
        ++sdd_image_number, object->full_name_with_level_number);
      strcpy(node_content_buffer, g2u(temp));
    }
    else
    {
      sprintf(node_content_buffer, "The Figure %d is the reference image for %s.", 
        ++sdd_image_number, object->full_name_with_level_number);
    }
    image_node = xmlNewChild(father_node, NULL, BAD_CAST "req_image", BAD_CAST node_content_buffer);
    xmlNewProp(image_node, BAD_CAST "file", BAD_CAST object->image_name);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_file_list(xmlNodePtr father_node)
{
  int i;
  xmlNodePtr file_list_node;
  xmlNodePtr req_list_node;

  if (language == CHINESE)
  {
    strcpy(node_title_buffer, g2u("文件列表"));
    strcpy(node_content_buffer, g2u("本系统应由以下SCADE Display的图形规格说明文件组成："));
  }
  else
  {
    strcpy(node_title_buffer, "File List");
    strcpy(node_content_buffer, "This system shall consist of below graphical specification files used in SCADE Display:");
  }

  file_list_node = xmlNewChild(father_node, NULL, BAD_CAST "section_file_list", NULL);
  xmlNewProp(file_list_node, BAD_CAST "heading", BAD_CAST node_title_buffer);
  xmlNewChild(file_list_node, NULL, BAD_CAST "req_statement", BAD_CAST node_content_buffer);

  req_list_node = xmlNewChild(file_list_node, NULL, BAD_CAST "req_list", NULL);
  xmlNewChild(req_list_node, NULL, BAD_CAST "list_item", BAD_CAST get_sgfx_file_name());
  for (i = 0; i < global_ogfx_info_count; i++)
  {
    ogfx_info_t *ogfx = &global_ogfx_info_list[i];
    xmlNewChild(req_list_node, NULL, BAD_CAST "list_item", BAD_CAST ogfx->file_name);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_layer_list(xmlNodePtr father_node)
{
  int i;
  xmlNodePtr layer_list_node;
  xmlNodePtr req_list_node;

  if (language == CHINESE)
  {
    strcpy(node_title_buffer, g2u("图层列表"));
  }
  else
  {
    strcpy(node_title_buffer, "Layer List");
  }
  layer_list_node = xmlNewChild(father_node, NULL, BAD_CAST "section_layer_list", NULL);
  xmlNewProp(layer_list_node, BAD_CAST "heading", BAD_CAST node_title_buffer);

  if (language == CHINESE)
  {
    char temp[500];
    sprintf(temp, "文件%s应包含以下图层：", get_sgfx_file_name());
    strcpy(node_content_buffer, g2u(temp));
  }
  else
  {
    sprintf(node_content_buffer, "The file %s shall contain below layers:", get_sgfx_file_name());
  }
  xmlNewChild(layer_list_node, NULL, BAD_CAST "req_statement", BAD_CAST node_content_buffer);

  req_list_node = xmlNewChild(layer_list_node, NULL, BAD_CAST "req_list", NULL);
  for (i = 0; i < layer_info_count; i++)
  {
    object_t *layer = object_list[layer_info_list[i].object_id];
    xmlNewChild(req_list_node, NULL, BAD_CAST "list_item", BAD_CAST layer->full_name_with_level_number);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void compose_hierarchy_figure(object_t *father, int level, char *line)
{
  size_t l = strlen(node_content_buffer);
  int pos = level * 3;
  object_t *son = father->son;
  object_t *next_sibling = father->next_sibling;

  if (level > 0)
  {
    line[pos-1] = ' ';
    line[pos-2] = '-';
    line[pos] = 0;
  }

  if (son)
  {
    sprintf(hierarchy_figure_buffer[hierarchy_figure_buffer_count++], "%s%s\n",
      line, father->full_name_with_level_number);
  }

  if (father->object_tag == TAG_REFERENCE_CONTAINER)
    return;
  if (level > 0)
  {
    line[pos-2] = ' ';
  }

  if (!next_sibling && level > 0)
  {
    line[pos-3] = ' ';
  }

  if (son)
  {
    line[pos] = '|';
    compose_hierarchy_figure(son, level + 1, line);
  }
  if (next_sibling && next_sibling->object_tag != TAG_LAYER)
  {
    compose_hierarchy_figure(next_sibling, level, line);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void clean_hierarchy_figure()
{
  int i, j;
  char *p1, *p2, *q;
  p1 = hierarchy_figure_buffer[hierarchy_figure_buffer_count-1];
  p2 = hierarchy_figure_buffer[hierarchy_figure_buffer_count];
  memset(p2, ' ', MAX_LINE_LENGTH);
  for (j = 0; j < MAX_LINE_LENGTH && p1[j + 1] != '-'; j++)
  {
    if (p1[j] == '|' && p1[j + 1] == ' ' &&
        p2[j] == ' ' && p2[j + 1] == ' ')
    {
      q = &hierarchy_figure_buffer[hierarchy_figure_buffer_count-1][j];
      for (i = hierarchy_figure_buffer_count-1; i > 0; i--)
      {
        if (q[1] == '-') break;
        if (q[0] == '|') q[0] = ' ';
        q -= MAX_LINE_LENGTH;
      }
    }
  }

  for (i = 0; i < hierarchy_figure_buffer_count; i++)
  {
    strcat(node_content_buffer, hierarchy_figure_buffer[i]);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_hierarchical_structure(xmlNodePtr father_node)
{
  int i;
  char line[MAX_LINE_LENGTH];
  xmlNodePtr hierarchical_structure_node;

  memset(line, 0, MAX_LINE_LENGTH);
  if (language == CHINESE)
  {
    strcpy(node_title_buffer, g2u("层次结构"));
  }
  else
  {
    strcpy(node_title_buffer, "Hierarchical Structure");
  }
  hierarchical_structure_node = xmlNewChild(father_node, NULL, BAD_CAST "section_hierarchical_structure", NULL);
  xmlNewProp(hierarchical_structure_node, BAD_CAST "heading", BAD_CAST node_title_buffer);

  for (i = 0; i < layer_info_count; i++)
  {
    object_t *object = object_list[layer_info_list[i].object_id];
    node_content_buffer[0] = 0;
    if (language == CHINESE)
    {
      sprintf(node_content_buffer, g2u("图层%s应具有图%d所示的层次结构："),
        object->full_name_with_level_number, ++sdd_image_number);
    }
    else
    {
      sprintf(node_content_buffer, "The hierarchical structure of the layer %s shall be designed as the Figure %d:",
        object->full_name_with_level_number, ++sdd_image_number);
    }
    xmlNewChild(hierarchical_structure_node, NULL, BAD_CAST "req_statement", BAD_CAST node_content_buffer);

    node_content_buffer[0] = 0;
    line[0] = 0;
    hierarchy_figure_buffer_count = 0;
    compose_hierarchy_figure(object, 0, line);
    clean_hierarchy_figure();
    xmlNewChild(hierarchical_structure_node, NULL, BAD_CAST "hierarchy_figure", BAD_CAST node_content_buffer);
  }

  for (i = 0; i < global_ogfx_info_count; i++)
  {
    ogfx_info_t *ogfx_info = &global_ogfx_info_list[i];
    object_t *object = object_list[ogfx_info->ogfx_root->id];

    strcpy(ogfx_info->full_name_with_level_number, object->full_name_with_level_number);
    node_content_buffer[0] = 0;
    if (language == CHINESE)
    {
      sprintf(node_content_buffer, g2u("OGFX %s应具有图%d所示的层次结构："), 
        ogfx_info->full_name_with_level_number, ++sdd_image_number);
    }
    else
    {
      sprintf(node_content_buffer, "The hierarchical structure of the OGFX %s shall be designed as the Figure %d:",
        ogfx_info->full_name_with_level_number, ++sdd_image_number);
    }
    xmlNewChild(hierarchical_structure_node, NULL, BAD_CAST "req_statement", BAD_CAST node_content_buffer);

    node_content_buffer[0] = 0;
    line[0] = 0;
    hierarchy_figure_buffer_count = 0;
    compose_hierarchy_figure(object, 0, line);
    clean_hierarchy_figure();
    xmlNewChild(hierarchical_structure_node, NULL, BAD_CAST "hierarchy_figure", BAD_CAST node_content_buffer);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_architecture_design(xmlNodePtr arch_design_node)
{
  gen_file_list(arch_design_node);
  gen_layer_list(arch_design_node);
  gen_hierarchical_structure(arch_design_node);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static int get_variable_dictionary_count(
  int this_var_dict_info_id,
  int next_var_dict_info_id,
  init_and_expr_scope_t init_and_expr_scope)
{
  int count = 0;
  int j;
  for (j = this_var_dict_info_id; j < next_var_dict_info_id; j++)
  {
    variable_dictionary_info_t *var_dict_info = &variable_dictionary_info_list[j];
    if (var_dict_info->init_and_expr_scope == init_and_expr_scope)
    {
      count++;
    }
  }
  return count;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_variable_specification(
  xmlNodePtr father_node,
  int start_info_id,
  int last_info_id,
  init_and_expr_scope_t init_and_expr_scope)
{
  int j;
  for (j = start_info_id; j < last_info_id; j++)
  {
    variable_dictionary_info_t *info = &variable_dictionary_info_list[j];
    if (info->init_and_expr_scope == init_and_expr_scope)
    {
      if (language == CHINESE)
      {
        char temp[100];
        sprintf(temp, "变量%s应具有表%d定义的特性。", info->variable_name, ++sdd_table_number);
        strcpy(node_content_buffer, g2u(temp));
      }
      else
      {
        sprintf(node_content_buffer, "The variable %s shall be designed with the specification defined in the Table %d.", 
          info->variable_name, ++sdd_table_number);
      }
      xmlNewChild(father_node, NULL, BAD_CAST "req_statement", BAD_CAST node_content_buffer);
      gen_variable_specification_table(father_node, info); 
    }
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_variable_list(
  xmlNodePtr father_node,
  int start_info_id,
  int last_info_id,
  init_and_expr_scope_t init_and_expr_scope)
{
  xmlNodePtr req_list_node;
  int j;

  req_list_node = xmlNewChild(father_node, NULL, BAD_CAST "req_list", NULL);
  for (j = start_info_id; j < last_info_id; j++)
  {
    variable_dictionary_info_t *info = &variable_dictionary_info_list[j];
    if (info->init_and_expr_scope == init_and_expr_scope)
    {
      xmlNewChild(req_list_node, NULL, BAD_CAST "list_item", BAD_CAST info->variable_name);
    }
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_input_variables(
  xmlNodePtr father_node,
  char *father_name,
  int this_var_dict_info_id,
  int next_var_dict_info_id
  )
{
  if (language == CHINESE)
  {
    sprintf(node_content_buffer, "%s%s", father_name, g2u("的输入接口应包含以下变量:"));
  }
  else
  {
    sprintf(node_content_buffer, "The input interface of %s shall consist of below variables:", 
      father_name);
  }
  xmlNewChild(father_node, NULL, BAD_CAST "req_statement", BAD_CAST node_content_buffer);
  gen_variable_list(father_node, 
      this_var_dict_info_id, 
      next_var_dict_info_id,
      SCOPE_INPUT);
  gen_variable_specification(father_node,
    this_var_dict_info_id, 
    next_var_dict_info_id,
    SCOPE_INPUT);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_output_variables(
  xmlNodePtr father_node,
  char *father_name,
  int this_var_dict_info_id,
  int next_var_dict_info_id
  )
{
  if (language == CHINESE)
  {
    sprintf(node_content_buffer, "%s%s", father_name, g2u("的输出接口应包含以下变量:"));
  }
  else
  {
    sprintf(node_content_buffer, "The output interface of %s shall consist of below variables:", 
      father_name);
  }
  xmlNewChild(father_node, NULL, BAD_CAST "req_statement", BAD_CAST node_content_buffer);
  gen_variable_list(father_node, 
      this_var_dict_info_id, 
      next_var_dict_info_id,
      SCOPE_OUTPUT);
  gen_variable_specification(father_node,
    this_var_dict_info_id, 
    next_var_dict_info_id,
    SCOPE_OUTPUT);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_input_output_node(
  xmlNodePtr father_node, 
  char *father_name,
  int this_var_dict_info_id,
  int next_var_dict_info_id,
  int input_count,
  int output_count
  //int constant_count
  )
{
  xmlNodePtr input_variables_node;
  xmlNodePtr output_variables_node;

  if (input_count > 0)
  {
    if (language == CHINESE)
    {
      strcpy(node_title_buffer, g2u("输入变量"));
    }
    else
    {
      strcpy(node_title_buffer, "Input Variables");
    }
    input_variables_node = xmlNewChild(father_node, NULL, BAD_CAST "section_input_variables", NULL);
    xmlNewProp(input_variables_node, BAD_CAST "heading", BAD_CAST node_title_buffer);
    gen_input_variables(input_variables_node,
      father_name, 
      this_var_dict_info_id,
      next_var_dict_info_id);
  }

  if (output_count > 0)
  {
    if (language == CHINESE)
    {
      strcpy(node_title_buffer, g2u("输出变量"));
    }
    else
    {
      strcpy(node_title_buffer, "Output Variables");
    }
    output_variables_node = xmlNewChild(father_node, NULL, BAD_CAST "section_output_variables", NULL);
    xmlNewProp(output_variables_node, BAD_CAST "heading", BAD_CAST node_title_buffer);
    gen_output_variables(output_variables_node,
      father_name, 
      this_var_dict_info_id,
      next_var_dict_info_id);
  }

  /*
  if (constant_count > 0)
  {
    gen_var_dict_constant_contents(object->object_name, 
      words_layer_for_content, 
      this_var_dict_info_id,
      next_var_dict_info_id);
    gen_variable_dictionary_node(father_node, object->full_name_with_level_number, "Constant Dictionary", "常量字典");
  }*/
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_interface_design(xmlNodePtr father_node)
{
  int i;
  xmlNodePtr sgfx_interface_node;
  xmlNodePtr all_ogfx_interface_node;

  if (language == CHINESE)
  {
    strcpy(node_title_buffer, g2u("SGFX接口"));
  }
  else
  {
    strcpy(node_title_buffer, "SGFX Interface");
  }
  sgfx_interface_node = xmlNewChild(father_node, NULL, BAD_CAST "section_SGFX_interface", NULL);
  xmlNewProp(sgfx_interface_node, BAD_CAST "heading", BAD_CAST node_title_buffer);

  for (i = 0; i < layer_info_count; i++)
  {
    object_t *object = object_list[layer_info_list[i].object_id];
    int this_var_dict_info_id = layer_info_list[i].this_variable_dictionary_info_id;
    int next_var_dict_info_id = layer_info_list[i].next_variable_dictionary_info_id; 
    int input_count = get_variable_dictionary_count(this_var_dict_info_id, next_var_dict_info_id, SCOPE_INPUT);
    int output_count = get_variable_dictionary_count(this_var_dict_info_id, next_var_dict_info_id, SCOPE_OUTPUT);
    //int constant_count = get_variable_dictionary_count(this_var_dict_info_id, next_var_dict_info_id, SCOPE_CONSTANT);
    xmlNodePtr layer_interface_node;

    if (language == CHINESE)
    {
      char temp[200];
      sprintf(temp, "图层%s的接口", object->full_name_with_level_number);
      strcpy(node_title_buffer, g2u(temp));
    }
    else
    {
      sprintf(node_title_buffer, "The Interface of the layer %s", object->full_name_with_level_number);
    }
    layer_interface_node = xmlNewChild(sgfx_interface_node, NULL, BAD_CAST "section_layer_interface", NULL);
    xmlNewProp(layer_interface_node, BAD_CAST "heading", BAD_CAST node_title_buffer);

    gen_input_output_node(layer_interface_node,
      object->full_name_with_level_number,
      this_var_dict_info_id,
      next_var_dict_info_id,
      input_count,
      output_count);
  }

  if (global_ogfx_info_count > 0)
  {
    if (language == CHINESE)
    {
      strcpy(node_title_buffer, g2u("OGFX接口"));
    }
    else
    {
      strcpy(node_title_buffer, "OGFX Interface");
    }
    all_ogfx_interface_node = xmlNewChild(father_node, NULL, BAD_CAST "section_all_ogfx_interface", NULL);
    xmlNewProp(all_ogfx_interface_node, BAD_CAST "heading", BAD_CAST node_title_buffer);

    for (i = 0; i < global_ogfx_info_count; i++)
    {
      ogfx_info_t *ogfx_info = &global_ogfx_info_list[i];
      int this_var_dict_info_id = ogfx_info->this_variable_dictionary_info_id;
      int next_var_dict_info_id = ogfx_info->next_variable_dictionary_info_id; 
      int input_count = get_variable_dictionary_count(this_var_dict_info_id, next_var_dict_info_id, SCOPE_INPUT);
      int output_count = get_variable_dictionary_count(this_var_dict_info_id, next_var_dict_info_id, SCOPE_OUTPUT);
      //int constant_count = get_variable_dictionary_count(this_var_dict_info_id, next_var_dict_info_id, SCOPE_CONSTANT);
      xmlNodePtr one_ogfx_interface_node;

      if (language == CHINESE)
      {
        char temp[200];
        sprintf(temp, "OGFX %s的接口", ogfx_info->full_name_with_level_number);
        strcpy(node_title_buffer, g2u(temp));
      }
      else
      {
        sprintf(node_title_buffer, "The Interface of the OGFX %s", ogfx_info->full_name_with_level_number);
      }
      one_ogfx_interface_node = xmlNewChild(all_ogfx_interface_node, NULL, BAD_CAST "section_OGFX_interface", NULL);
      xmlNewProp(one_ogfx_interface_node, BAD_CAST "heading", BAD_CAST node_title_buffer);

      gen_input_output_node(one_ogfx_interface_node,
        ogfx_info->full_name_with_level_number,
        this_var_dict_info_id,
        next_var_dict_info_id,
        input_count,
        output_count);
    }
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_obj_children_list(
  xmlNodePtr father_node,
  object_t *first_son)
{
  xmlNodePtr children_node;
  object_t *next_son;

  children_node = xmlNewChild(father_node, NULL, BAD_CAST "children_list", NULL);
  xmlNewChild(children_node, NULL, BAD_CAST "list_item", BAD_CAST first_son->full_name_with_level_number);
  next_son = first_son->next_sibling;
  while (next_son)
  {
    xmlNewChild(children_node, NULL, BAD_CAST "list_item", BAD_CAST next_son->full_name_with_level_number);
    next_son = next_son->next_sibling;
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_object_detail_design(xmlNodePtr father_node, object_t *object)
{
  xmlNodePtr object_detail_design_node;
  char temp[500];
  object_t *son = object->son;

  if (language == CHINESE)
  {
    sprintf(temp, "%s%s", son ? "图件":"图元", object->full_name_with_level_number);
    strcpy(node_title_buffer, g2u(temp));
  }
  else
  {
    sprintf(node_title_buffer, "%s%s", son ? "Object ":"Primitive ", object->full_name_with_level_number);
  }
  object_detail_design_node = xmlNewChild(father_node, NULL, BAD_CAST "section_object_detail_design", NULL);
  xmlNewProp(object_detail_design_node, BAD_CAST "heading", BAD_CAST node_title_buffer);

  if (language == CHINESE)
  {
    sprintf(temp, "应按表%d定义的规格设计%s%s。", ++sdd_table_number,
      son ? "图件" : "图元", 
      object->full_name_with_level_number);
    strcpy(node_content_buffer, g2u(temp));
  }
  else
  {
    sprintf(node_content_buffer, "The %s %s shall be designed with the specification defined in the Table %d.",
      son ? "object" : "primitive", 
      object->full_name_with_level_number, ++sdd_table_number);
  }
  xmlNewChild(object_detail_design_node, NULL, BAD_CAST "req_statement", BAD_CAST node_content_buffer);
  gen_object_specification_table(object_detail_design_node, object);

  if (object->mask_info)
  {
    if (language == CHINESE)
    {
      sprintf(temp, "应按表%d设置适用于%s的mask的状态：", 
        ++sdd_table_number, object->full_name_with_level_number);
      strcpy(node_content_buffer, g2u(temp));
    }
    else
    {
      sprintf(node_content_buffer, "The state of applicable mask for %s shall be set as the Table %d.",
        object->full_name_with_level_number, ++sdd_table_number);
    }
    xmlNewChild(object_detail_design_node, NULL, BAD_CAST "applicable_masks", BAD_CAST node_content_buffer);
    gen_mask_table(object_detail_design_node, object->mask_info);
  }

  if (object->input_ref_parameter_count > 0)
  {
    if (language == CHINESE)
    {
      sprintf(temp, "应按表%d设置%s的输入引用参数。", 
        ++sdd_table_number, object->full_name_with_level_number);
      strcpy(node_content_buffer, g2u(temp));
    }
    else
    {
      sprintf(node_content_buffer, "The input reference parameters for %s shall be set as the Table %d.", 
        object->full_name_with_level_number, ++sdd_table_number);
    }
    xmlNewChild(object_detail_design_node, NULL, BAD_CAST "req_statement", BAD_CAST node_content_buffer);
    gen_object_ref_parameter_table(object_detail_design_node, object, object->input_ref_parameter_count, object->first_input_ref_parameter_info_id);
  }

  if (object->output_ref_parameter_count > 0)
  {
    if (language == CHINESE)
    {
      sprintf(temp, "应按表%d设置%s的输出引用参数。", 
        ++sdd_table_number, object->full_name_with_level_number);
      strcpy(node_content_buffer, g2u(temp));
    }
    else
    {
      sprintf(node_content_buffer, "The output reference parameters for %s shall be set as the Table %d.", 
        object->full_name_with_level_number, ++sdd_table_number);
    }
    gen_object_ref_parameter_table(object_detail_design_node, object, object->output_ref_parameter_count, object->first_output_ref_parameter_info_id);
  }

  gen_object_image(object_detail_design_node, object);

  if (son)
  {
    if (language == CHINESE)
    {
      sprintf(temp, "%s%s包含下列子辈图件/图元:", 
        son ? "图件" : "图元", 
        object->full_name_with_level_number); 
      strcpy(node_content_buffer, g2u(temp));
    }
    else
    {
      sprintf(node_content_buffer, "The children of the %s %s are:",
        son ? "object" : "primitive", 
        object->full_name_with_level_number);
    }
    xmlNewChild(object_detail_design_node, NULL, BAD_CAST "children_comment", BAD_CAST node_content_buffer);
    gen_obj_children_list(object_detail_design_node, son);
  }

  if (language == CHINESE)
  {
    sprintf(temp, "%s%s的别名是: %s。", 
      son ? "图件" : "图元", 
      object->full_name_with_level_number,
      object->full_name_with_slash); 
    strcpy(node_content_buffer, g2u(temp));
  }
  else
  {
    sprintf(node_content_buffer, "The %s %s is aliased as %s.",
      son ? "object" : "primitive", 
      object->full_name_with_level_number,
      object->full_name_with_slash);
  }
  xmlNewChild(object_detail_design_node, NULL, BAD_CAST "aliased_comment", BAD_CAST node_content_buffer);

  if (object->object_tag != TAG_REFERENCE_CONTAINER && object->object_tag != TAG_NPLICATOR_CONTAINER)
  {
    while (son)
    {
      gen_object_detail_design(father_node, son);
      son = son->next_sibling;
    }
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_layer_detail_design(xmlNodePtr father_node, object_t *object)
{
  object_t *son = object->son;
  xmlNodePtr layer_object_design_node;
  if (language == CHINESE)
  {
    char temp[200];
    sprintf(temp, "图层%s", object->full_name_with_level_number);
    strcpy(node_title_buffer, g2u(temp));
  }
  else
  {
    sprintf(node_title_buffer, "The Layer %s", object->full_name_with_level_number);
  }
  layer_object_design_node = xmlNewChild(father_node, NULL, BAD_CAST "section_object_detail_design", NULL);
  xmlNewProp(layer_object_design_node, BAD_CAST "heading", BAD_CAST node_title_buffer);


  if (language == CHINESE)
  {
    char temp[100];
    sprintf(temp, "应按表%d定义的规格设计图层%s。", ++sdd_table_number, object->full_name_with_level_number);
    strcpy(node_content_buffer, g2u(temp));
  }
  else
  {
    sprintf(node_content_buffer, "The layer %s shall be designed with the specification defined in the Table %d.", 
      object->full_name_with_level_number, ++sdd_table_number);
  }
  xmlNewChild(layer_object_design_node, NULL, BAD_CAST "req_statement", BAD_CAST node_content_buffer);
  gen_object_specification_table(layer_object_design_node, object);
  gen_object_image(layer_object_design_node, object);

  while (son)
  {
    gen_object_detail_design(father_node, son);
    son = son->next_sibling;
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_ogfx_detail_design(xmlNodePtr father_node, ogfx_info_t *ogfx)
{
  object_t *son;
  son = ogfx->ogfx_root;
  while (son)
  {
    gen_object_detail_design(father_node, son);
    son = son->next_sibling;
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_detail_design(xmlNodePtr detail_design_node)
{
  int i;
  int count;
  ogfx_info_t *ogfx = global_ogfx_info_list;
  xmlNodePtr sgfx_detail_design_node;

  if (language == CHINESE)
  {
    strcpy(node_title_buffer, g2u("SGFX详细设计"));
  }
  else
  {
    strcpy(node_title_buffer, "SGFX Detail Design");
  }
  sgfx_detail_design_node = xmlNewChild(detail_design_node, NULL, BAD_CAST "section_sgfx_detail_design", NULL);
  xmlNewProp(sgfx_detail_design_node, BAD_CAST "heading", BAD_CAST node_title_buffer);

  for (i = 0; i < 1; i++)//for (i = 0; i < layer_info_count; i++)
  {
    object_t *object = object_list[layer_info_list[i].object_id];
    xmlNodePtr layer_detail_design_node;
    if (language == CHINESE)
    {
      char temp[200];
      sprintf(temp, "图层%s", object->full_name_with_level_number);
      strcpy(node_title_buffer, g2u(temp));
    }
    else
    {
      sprintf(node_title_buffer, "The Layer %s", object->full_name_with_level_number);
    }
    layer_detail_design_node = xmlNewChild(sgfx_detail_design_node, NULL, BAD_CAST "section_layer_detail_design", NULL);
    xmlNewProp(layer_detail_design_node, BAD_CAST "heading", BAD_CAST node_title_buffer);

    gen_layer_detail_design(layer_detail_design_node, object);
  }

  count = (get_including_ogfx_children() == 1) ? global_ogfx_info_count : 0;
  if (count > 0)
  {
    xmlNodePtr all_ogfx_detail_design_node;
    if (language == CHINESE)
    {
      strcpy(node_title_buffer, g2u("OGFX详细设计"));
    }
    else
    {
      strcpy(node_title_buffer, "OGFX Detail Design");
    }
    all_ogfx_detail_design_node = xmlNewChild(detail_design_node, NULL, BAD_CAST "section_all_OGFX_detail_design", NULL);
    xmlNewProp(all_ogfx_detail_design_node, BAD_CAST "heading", BAD_CAST node_title_buffer);
    for (i = 0; i < 1; i++, ogfx++) //for (i = 0; i < count; i++, ogfx++)
    {
      xmlNodePtr ogfx_detail_design_node;
      if (language == CHINESE)
      {
        char temp[200];
        sprintf(temp, "OGFX %s", ogfx->full_name_with_level_number);
        strcpy(node_title_buffer, g2u(temp));
      }
      else
      {
        sprintf(node_title_buffer, "The OGFX %s", ogfx->full_name_with_level_number);
      }
      ogfx_detail_design_node = xmlNewChild(all_ogfx_detail_design_node, NULL, BAD_CAST "section_OGFX_detail_design", NULL);
      xmlNewProp(ogfx_detail_design_node, BAD_CAST "heading", BAD_CAST node_title_buffer);

      gen_ogfx_detail_design(ogfx_detail_design_node, ogfx);
    }
  }
}
#if 0
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void gen_environment_parameter(xmlNodePtr environment_parameter_node)
{
  if (language == ENGLISH)
  {
    strcpy(node_title_buffer, "General Configuration");
    sprintf(node_content_buffer, "The general configuration for this application shall conform to the below specification of properties:");
  }
  else
  {
    strcpy(node_title_buffer, g2u("总体配置"));
    sprintf(node_content_buffer, "%s:", g2u("本项应用的总体配置应符合以下属性规格"));
  }
  gen_general_config_content(node_content_buffer+strlen(node_content_buffer));
  gen_general_node(environment_parameter_node, "oid-general-config");

  if (language == ENGLISH)
  {
    strcpy(node_title_buffer, "Color Table");
    sprintf(node_content_buffer, "The below color table shall be used:");
  }
  else
  {
    strcpy(node_title_buffer, g2u("颜色表"));
    sprintf(node_content_buffer, "%s:", g2u("应使用以下颜色表"));
  }
  gen_color_table_content(node_content_buffer+strlen(node_content_buffer));
  gen_general_node(environment_parameter_node, "oid-color-table");

  if (language == ENGLISH)
  {
    strcpy(node_title_buffer, "Texture Table");
  }
  else
  {
    strcpy(node_title_buffer, g2u("纹理表"));
  }
  if (max_texture_index == 0)
  {
    if (language == ENGLISH)
    {
      strcpy(node_content_buffer, "None.");
    }
    else
    {
      strcpy(node_content_buffer, g2u("无。"));
    }
    gen_general_node(environment_parameter_node, "oid-texture-table");
  }
  else
  {
    if (language == ENGLISH)
    {
      sprintf(node_content_buffer, "The below texture table shall be used:");
    }
    else
    {
      sprintf(node_content_buffer, "%s:", g2u("应使用以下纹理表"));
    }
    gen_texture_table_content(node_content_buffer+strlen(node_content_buffer));
    gen_general_node(environment_parameter_node, "oid-texture-table");
  }

  if (language == ENGLISH)
  {
    strcpy(node_title_buffer, "Line Width Table");
    sprintf(node_content_buffer, "The below line width table shall be used:");
  }
  else
  {
    strcpy(node_title_buffer,  g2u("线宽表"));
    sprintf(node_content_buffer, "%s:", g2u("应使用以下线宽表"));
  }
  gen_line_width_table_content(node_content_buffer+strlen(node_content_buffer));
  gen_general_node(environment_parameter_node, "oid-line_width-table");

  if (language == ENGLISH)
  {
    strcpy(node_title_buffer, "Line Stipple Table");
    sprintf(node_content_buffer, "The below line stipple table shall be used:");
  }
  else
  {
    strcpy(node_title_buffer,  g2u("点画线表"));
    sprintf(node_content_buffer, "%s:", g2u("应使用以下点画线表"));
  }
  gen_line_stipple_table_content(node_content_buffer+strlen(node_content_buffer));
  gen_general_node(environment_parameter_node, "oid-line_stipple-table");

  if (language == ENGLISH)
  {
    strcpy(node_title_buffer, "Font Table");
    sprintf(node_content_buffer, "The below font table shall be used:");
  }
  else
  {
    strcpy(node_title_buffer,  g2u("字体表"));
    sprintf(node_content_buffer, "%s:", g2u("应使用以下字体表"));
  }
  gen_font_table_content(node_content_buffer+strlen(node_content_buffer));
  gen_general_node(environment_parameter_node, "oid-font-table");
}
#endif
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_sdd_xml(char *output_path, char *dd_file_name)
{
  char full_dd_file_name[FILE_NAME_LENGTH];
  xmlNodePtr root_node;
  xmlNodePtr arch_design_node;
  xmlNodePtr interface_design_node;
  xmlNodePtr detail_design_node;
  /*不用xmlNodePtr environment_parameter_node;*/

  sprintf(full_dd_file_name, "%s\\%s", output_path, dd_file_name);

  doc = xmlNewDoc(BAD_CAST "1.0");
  root_node = xmlNewNode(NULL, BAD_CAST "sdy_opengl_sdd");
  xmlDocSetRootElement(doc, root_node);

  arch_design_node = xmlNewChild(root_node, NULL, BAD_CAST "chapter_architecture", NULL);
  xmlNewProp(arch_design_node, BAD_CAST "heading", 
    language == ENGLISH ? "Architecture Design" : BAD_CAST g2u("架构设计"));
  interface_design_node = xmlNewChild(root_node, NULL, BAD_CAST "chapter_interface", NULL);
  xmlNewProp(interface_design_node, BAD_CAST "heading", 
    language == ENGLISH ? "Interface Design" : BAD_CAST g2u("接口设计"));
  detail_design_node = xmlNewChild(root_node, NULL, BAD_CAST "chapter_detail", NULL);
  xmlNewProp(detail_design_node, BAD_CAST "heading", 
    language == ENGLISH ? "Detail Design" : BAD_CAST g2u("详细设计"));
  /*不用
  environment_parameter_node = xmlNewChild(root_node, NULL, BAD_CAST "level_1_section", NULL);
  xmlNewProp(environment_parameter_node, BAD_CAST "heading", BAD_CAST g2u("环境参数"));*/

  gen_architecture_design(arch_design_node);
  gen_interface_design(interface_design_node);
  //gen_environment_parameter(root_node);
  gen_detail_design(detail_design_node);
 
  xmlSaveFormatFileEnc(full_dd_file_name, doc, "UTF-8", 1);

  xmlFreeDoc(doc);
  xmlCleanupParser();
  xmlMemoryDump();//debug memory for regression tests
}
