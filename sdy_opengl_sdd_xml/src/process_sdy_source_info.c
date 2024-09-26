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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdy_object.h"
#include "sdy_source.h"
#include "process_file_and_dir.h"

/* --------------------------- 宏定义 -------------------------------*/
#define TRACE_COMMENT_SIZE 200

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/

const char *object_class_str[] =
{
  "NO_SGL_CODE",
  "PRIMITIVE",
  "BI_PRIMITIVE",
  "CASE_PRIMITIVE",
  "COMPONENT",
};
static int **condition_indexes_list;
static unsigned char *object_lineal_matrix;
static char specific_draw_callee[OBJECT_NAME_LENGTH + 1];
static int including_ogfx_children = 1;
static char trace_comment_buffer[TRACE_COMMENT_SIZE];

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void enhance_indexes_info(void)
{
  int j;
  for (j = 1; j < object_count; j++)
  {
    object_t *o = object_list[j];
    if (o->object_tag == TAG_COND_CONTAINER)
    {
      indexes_info_t *indexes = o->indexes_info;
      int i = 0;
      /*compose_object_expected_condition_index*/
      object_t *s = o->son;
      do
      {
        s->expected_condition_index = indexes->condition_index_list[i++];
        s = s->next_sibling;
      } while (s);

      switch (indexes->index_init_type)
      {
      case CONDITION_INDEX_INIT_DEFAULT:
        indexes->index_init_value = indexes->default_index;
        break;
      case CONDITION_INDEX_INIT_OTHER:
        indexes->index_init_value = INDEXES_OTHER;
        break;
      }

      o->indexes_id = indexes_count++;
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
static void compose_condition_indexes_list(void)
{
  int j;
  int *condition_indexes_init_value;

  condition_indexes_init_value = (int *)get_cmem(indexes_count * sizeof(int));
  for (j = 1; j < object_count; j++)
  {
    object_t *o = object_list[j];

    if (o->object_tag == TAG_COND_CONTAINER)
    {
      condition_indexes_init_value[o->indexes_id] = o->indexes_info->index_init_value;
    }
  }

  condition_indexes_list = (int **)get_cmem(object_count * (sizeof(int *)));
  for (j = 0; j < object_count; j++)
  {
    object_t *o = object_list[j];
    object_t *f = o->father_list[0];
    int expected_condition_index = o->expected_condition_index;

    condition_indexes_list[j] = (int *)get_cmem(indexes_count * sizeof(int));
    memcpy(condition_indexes_list[j], condition_indexes_init_value, indexes_count * sizeof(int));
    while (f)
    {
      if (f->object_tag == TAG_COND_CONTAINER)
      {
        condition_indexes_list[j][f->indexes_id] = expected_condition_index;
      }
      expected_condition_index = f->expected_condition_index;
      f = f->father_list[0];
    }
  }
  free_mem(condition_indexes_init_value);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void convert_primitive_sibling_order(unsigned n, /*@out@*/char *temp)
{
  int j, l;
  _itoa(n, temp, 26);
  l = strlen(temp);
  for (j=0; j < l && temp[j]; j++)
  {
    char c = temp[j];
    if (c >= '0' && c <= '9')
    {
      if (j < l - 1)
        temp[j] = 'a' + c - '1';
      else
        temp[j] = 'a' + c - '0';
    }
    else
      temp[j] += 10;
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void compose_object_level_number(object_t *o)
{
  if (o->father_list[0]->object_tag != TAG_REFERENCE_CONTAINER &&
      o->father_list[0]->object_tag != TAG_NPLICATOR_CONTAINER)
  {
    o->non_primitive_sibling_order = o->prev_sibling ? o->prev_sibling->non_primitive_sibling_order : 0;
  }
  if (o->son)
  {
    o->non_primitive_sibling_order++;
    if (o->object_tag == TAG_REFERENCE_CONTAINER ||
        o->object_tag == TAG_NPLICATOR_CONTAINER)
    {
      //sprintf(o->level_number, "%s%d*", o->father_list[0]->level_number, o->non_primitive_sibling_order);
      sprintf(o->level_number, "%s%d-", o->father_list[0]->level_number, o->non_primitive_sibling_order);
    }
    else
    {
      if (o->father_list[0]->object_tag == TAG_REFERENCE_CONTAINER)
      {
        sprintf(o->level_number, "%d-", o->non_primitive_sibling_order);
      }
      else
      {
        sprintf(o->level_number, "%s%d-", o->father_list[0]->level_number, o->non_primitive_sibling_order);
      }
      compose_object_level_number(o->son);
    }
    sprintf(o->full_name_with_level_number, "%s%s", o->level_number, o->object_name);
  }
  //primitive just pass the non_primitive_sibling_order
  else
  {
    object_t *f;
    char temp[5];
    if (o->prev_sibling)
      o->primitive_sibling_order = o->prev_sibling->primitive_sibling_order + 1;
    convert_primitive_sibling_order(o->primitive_sibling_order, temp);
    sprintf(o->level_number, "%s%s", o->father_list[0]->level_number, temp);
    sprintf(o->full_name_with_level_number, "%s-%s", o->level_number, o->object_name);

    f = o->father_list[0];
    if (f)
      f->primitive_son_count++;
  }
  if (o->next_sibling)
  {
    compose_object_level_number(o->next_sibling);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
#if 0
static void compose_object_lineal_matrix(object_t *o, int father_id)
{
  *(object_lineal_matrix + father_id * object_count + o->id) = 1;
  if (o->to_handle)
  {
    o->to_handle = false;
    if (o->son)
    {
      object_t *s = o->son;
      do
      {
        int i;
        compose_object_lineal_matrix(s, o->id);
        for (i = 0; i < object_id; i++)
        {
          *(object_lineal_matrix + o->id * object_count + i) |= *(object_lineal_matrix + s->id * object_count + i);
        }
        s = s->next_sibling;
      } while (s);
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
static void compose_object_lineal_matrix_from_root(void)
{
  int i;
  size_t s = object_count * object_count;

  object_lineal_matrix = (unsigned char *)get_mem(s);
  memset(object_lineal_matrix, 0, s);
  for (i = 0; i < object_count; i++)
  {
    object_list[i]->to_handle = true;
  }
  memset(object_lineal_matrix + 1, 1, object_count-1);//set 1 for root
  compose_object_lineal_matrix(root->son, 0);
}
#endif
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void set_son_node_to_reachable(object_t *o, int focus_id)
{
  object_t *s = o->son;
  if (s)
  {
    *(object_lineal_matrix + o->id * object_count + s->id) = 1;
    *(object_lineal_matrix + focus_id * object_count + s->id) = 1;
    set_son_node_to_reachable(s, focus_id);
    while (s = s->next_sibling)
    {
      *(object_lineal_matrix + o->id * object_count + s->id) = 1;
      *(object_lineal_matrix + focus_id * object_count + s->id) = 1;
      set_son_node_to_reachable(s, focus_id);
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
static void set_father_node_to_reachable(object_t *o, int focus_id)
{
  int i;
  for (i = 0; i < o->father_count; i++)
  {
    object_t *f = o->father_list[i];
    if (f)
    {
      *(object_lineal_matrix + f->id * object_count + o->id) = 1;
      *(object_lineal_matrix + f->id * object_count + focus_id) = 1;
      set_father_node_to_reachable(f, focus_id);
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
static void compose_object_lineal_matrix_from_root(void)
{
  int focus_id;
  size_t s = object_count * object_count;
  layer_info_t *info = layer_info_list;

  object_lineal_matrix = (unsigned char *)get_mem(s);
  memset(object_lineal_matrix, 0, s);
  memset(object_lineal_matrix + 1, 1, object_count-1);//set 1 for root

  for (focus_id = 1; focus_id < object_count; focus_id++)
  {
    object_t *o = object_list[focus_id];
    set_father_node_to_reachable(o, focus_id);
    set_son_node_to_reachable(o, focus_id);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static int search_variable_dictionary_info_id(int scope_id, char *input_name)
{
  int i;
  variable_dictionary_info_t *info = variable_dictionary_info_list;
  if (input_name != NULL)
  {
    for (i = 0; i < variable_dictionary_info_count; i++, info++)
    {
      if (info->variable_dictionary_scope_id == scope_id && strcmp(input_name, info->variable_name) == 0)
      {
        return i;
      }
    }
  }
  return -1;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void compose_ref_paramerer_more_info(void)
{
  int i;
  ref_parameter_info_t *info = ref_parameter_info_list;
  for (i = 0; i < ref_parameter_info_count; i++, info++)
  {
    info->form_para_scope_id = info->ref_object->ref_ogfx_id + 1;
    info->form_para_info_id = search_variable_dictionary_info_id(info->form_para_scope_id, info->parameter_name);
    info->real_para_info_id = search_variable_dictionary_info_id(info->real_para_scope_id, info->real_para_name);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void enhance_layer_info(void)
{
  int i;
  layer_info_t *info = layer_info_list;

  for (i = 0; i < layer_info_count; i++, info++)
  {
    object_t *o = object_list[info->object_id];
    info->layer_name = o->object_name;
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void enhance_object_info(void)
{
  int i;
  ogfx_info_t *info = global_ogfx_info_list;

  compose_object_level_number(root->son);
  for (i = 0; i < global_ogfx_info_count; i++, info++)
  {
    object_t *o = info->ogfx_root;
    o->non_primitive_sibling_order = i+1;
    compose_object_level_number(o);
  }

  enhance_indexes_info();
  if (indexes_count > 0)
  {
    compose_condition_indexes_list();
  }
  compose_object_lineal_matrix_from_root();
  compose_ref_paramerer_more_info();
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void generate_aside_sdy_object_relation(void)
{
  char file_name[FILE_NAME_LENGTH];
  FILE *file_id;
  int i, j;

  sprintf(file_name, "%s\\Specific\\aside_sdy_object_relation.h", get_sdy_obj_dir());
  file_id = fopen(file_name, "w");
  if (NULL == file_id)
  {
    fatal_error("Can't create %s\n", file_name);
  }
  fprintf(file_id, "#ifndef __ASIDE_SDY_OBJECT_RELATION_H__\n");
  fprintf(file_id, "#define __ASIDE_SDY_OBJECT_RELATION_H__\n");
  fprintf(file_id, "#define OBJECT_COUNT %d\n", object_count);
  fprintf(file_id, "#define OBJECT_COUNT_FOR_IMAGE %d\n", including_ogfx_children ? object_count : object_count_for_main_source);
  fprintf(file_id, "#define INDEXES_COUNT %d\n", indexes_count == 0 ? 1 : indexes_count);
  fprintf(file_id, "#define LAYER_COUNT %d\n", layer_info_count);
  fprintf(file_id, "#endif\n");
  fclose(file_id);

  sprintf(file_name, "%s\\Specific\\aside_sdy_object_relation.c", get_sdy_obj_dir());
  file_id = fopen(file_name, "w");
  if (NULL == file_id)
  {
    fatal_error("Can't create %s\n", file_name);
  }

  fprintf(file_id, "#include \"aside_sdy_object_relation.h\"\n");

  fprintf(file_id, "char *image_name_list[OBJECT_COUNT] =\n{\n");
  fprintf(file_id, "  \"00000.jpeg\", /*    0*/\n");
  for (i = 1; i < object_count; i++)
  {
    fprintf(file_id, "  \"%s\", /*%5d*/\n", object_list[i]->image_name, i);
  }
  fprintf(file_id, "};\n");

  fprintf(file_id, "unsigned char object_class_list[OBJECT_COUNT] =\n{\n");
  for (i = 0; i < object_count; i++)
  {
    fprintf(file_id, "  %d, /*%5d*/\n", object_list[i]->object_class, i);
  }
  fprintf(file_id, "};\n");

  fprintf(file_id, "int object_scope_list[OBJECT_COUNT] =\n{\n");
  for (i = 0; i < object_count; i++)
  {
    fprintf(file_id, "  %d, /*%5d*/\n", object_list[i]->variable_dictionary_scope_id, i);
  }
  fprintf(file_id, "};\n");

  fprintf(file_id, "unsigned char object_lineal_matrix[OBJECT_COUNT][OBJECT_COUNT] =\n{\n");
  for (i = 0; i < object_count; i++)
  {
    fprintf(file_id, "  { /*%05d*/", i);
    for (j = 0; j < object_count; j++)
    {
      if ((j % 40) == 0)
      {
        fprintf(file_id, "\n  ");
      }
      fprintf(file_id, "%d%s", *(object_lineal_matrix + i * object_count + j), j < object_count - 1 ? "," : "},\n");
    }
  }
  fprintf(file_id, "};\n");

  if (indexes_count == 0)
  {
    fprintf(file_id, "int condition_indexes_list[OBJECT_COUNT][INDEXES_COUNT];/*dummy*/\n");
  }
  else
  {
    fprintf(file_id, "int condition_indexes_list[OBJECT_COUNT][INDEXES_COUNT] =\n{\n");
    for (i = 0; i < object_count; i++)
    {
      fprintf(file_id, "  /*%05d*/ {", i);
      for (j = 0; j < indexes_count; j++)
      {
        fprintf(file_id, "%d%s", condition_indexes_list[i][j], j < indexes_count-1 ? ", " : "},\n");
      }
      free_mem(condition_indexes_list[i]);
    }
    fprintf(file_id, "};\n");
  }

  free_mem(object_lineal_matrix);
  free_mem(condition_indexes_list);
  fclose(file_id);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void generate_aside_sdy_object_info(void)
{
  char file_name[FILE_NAME_LENGTH];
  FILE *file_id;
  int i;
  int image_count = 0;

  compose_aside_sdy_object_info_file_name(file_name);
  file_id = fopen(file_name, "w");
  if (NULL == file_id)
  {
    fatal_error("Can't create %s\n", file_name);
  }

  for (i = 1; i < object_count; i++)
  {
    object_t *o = object_list[i];
    if (o->image_name[0])
    {
      image_count++;
      fprintf(file_id, "%s|%s", o->image_name, o->full_name_with_level_number);
    }
    else
    {
      fprintf(file_id, "         |%s", o->full_name_with_level_number);
    }
    fprintf(file_id, "|%s|%s\n", convert_enum_to_str(o->object_tag, str_and_tag_list), object_class_str[o->object_class]);
  }
  fprintf(file_id, "image_count=%d\n", image_count);
  /* for debug
  {
    ogfx_info_t *o = global_ogfx_info_list;
    fprintf(file_id, "\n");
    for (i = 0; i < global_ogfx_info_count; i++, o++)
    {
      fprintf(file_id, "[%d]%s\n", i+1, o->file_name);
    }
  }*/
  fclose(file_id);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void set_object_class_and_image_name(int object_id)
{
  object_t *o = object_list[object_id];
  sdy_source_tag_t tag = o->object_tag;
  if (o->son)
    o->object_class = COMPONENT_OBJECT;
  else if (tag == TAG_BIFONT) // || tag == TAG_BITMAP)
    o->object_class = BI_PRIMITIVE_OBJECT;
  else if (o->father_list[0]->object_tag == TAG_COND_CONTAINER)
    o->object_class = CASE_PRIMITIVE_OBJECT;
  else
    o->object_class = PRIMITIVE_OBJECT;

  if (o->object_class > PRIMITIVE_OBJECT)
  {
    sprintf(o->image_name, "%05d.jpeg", o->id);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
object_t *search_object_by_oid(char *oid)
{
  int j;
  for (j = 1; j < object_count; j++)
  {
    object_t *o = object_list[j];
    if (strcmp(o->oid, oid) == 0)
    {
      return o;
    }
  }
  return NULL;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void set_specific_draw_callee(char *sgfx_ogfx_file_name)
{
  char *p = strrchr(sgfx_ogfx_file_name, '.');
  if (p == NULL)
  {
    fatal_error("specification file name has no suffix sgfx or ogfx\n");
  }
  else
  {
    if (strncmp(p + 1, "sgfx", 4) == 0)
    {
      specific_draw_callee[0] = 0;
    }
    else
    {
      p = strrchr(sgfx_ogfx_file_name, '\\');
      if (p == NULL)
      {
        p = sgfx_ogfx_file_name;
      }
      else
      {
        p++;
      }
      strcpy(specific_draw_callee, p);
      p = specific_draw_callee;
      while ((*p) != '.')
      {
        if ((*p) == ' ') (*p) = '_';
        p++;
      }
      (*p) = 0;
    }
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *  对于sgfx返回object_list[1]->object_name
 *  对于ogfx返回已生成的specific_draw_callee
 *
 **********************************************************************
*/
char *get_specific_draw_callee(void)
{
  return specific_draw_callee[0] ? specific_draw_callee : object_list[1]->object_name;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
char *convert_tag_to_str(sdy_source_tag_t tag)
{
  return convert_enum_to_str(tag, str_and_tag_list);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
variable_dictionary_info_t *search_input_info_by_oid(char *input_oid)
{
  int i;
  variable_dictionary_info_t *info = variable_dictionary_info_list;
  for (i = 0; i < variable_dictionary_info_count; i++, info++)
  {
    if (strcmp(input_oid, info->variable_oid) == 0)
    {
      return info;
    }
  }
  return NULL;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void set_including_ogfx_children(char *option)
{
  including_ogfx_children = (*option) == '0' ? 0 : 1;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
int get_including_ogfx_children(void)
{
  return including_ogfx_children;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_ogfx_next_variable_dictionary_info_id(void)
{
  int i;
  int count;
  ogfx_info_t *ogfx = global_ogfx_info_list;
  count = (get_including_ogfx_children() == 1) ? global_ogfx_info_count : 0;
  for (i = 0; i < count; i++, ogfx++)
  {
    if (i < count - 1)
    {
      ogfx->next_variable_dictionary_info_id = (ogfx + 1)->this_variable_dictionary_info_id;
    }
    else
    {
      ogfx->next_variable_dictionary_info_id = variable_dictionary_info_count;
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
void inherit_trace_comment(char *key_word)
{
  int i, j, k;
  for (i = 1; i < object_count; i++)
  {
    object_t *o = object_list[i];
    if (!o->comment)
    {
      k = 0;
      for (j = 0; j < o->father_count; j++)
      {
        object_t *f = o->father_list[j];
        if (f->comment)
        {
          char *p;
          char *q = f->comment;
          while ((*q) && (p = strstr(q, key_word)))
          {
            char c;
            do
            {
              c = *p++;
              trace_comment_buffer[k++] = c;
              if (k >= TRACE_COMMENT_SIZE)
              {
                fatal_error("TRACE_COMMENT_SIZE is not enough\n");
              }
            } while (c && c != ' ' && c != '\n' && c != ',');
            q = p;
          }
        }
      }
      if (k > 0)
      {
        trace_comment_buffer[k] = 0;
        create_string_item(&o->comment, trace_comment_buffer);
      }
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
static void spread_mask_info(object_t *mask_object, object_t *object)
{
  if (object->object_tag != TAG_STENCIL &&
      object->object_tag != TAG_CLIP_BOX &&
      object->object_tag != TAG_CLIP_PLANE)
  {
    object_t *son = object->son;
    if (!son)
    {
      mask_info_t *mask_info = (mask_info_t *)get_cmem(sizeof(mask_info_t));
      mask_info->mask = mask_object;
      if (object->mask_info)
      {
        mask_info->next_mask_info = object->mask_info;
      }
      object->mask_info = mask_info;
      if (object->inactive_mask_count)
      {
        int i;
        for (i = 0; i < object->inactive_mask_count; i++)
        {
          if (mask_object == object->inactive_mask_list[i])
          {
            object->mask_info->inactivity = 1;
            break;
          }
        }       
      }
    }

    if (object->object_tag != TAG_REFERENCE_CONTAINER && object->object_tag != TAG_NPLICATOR_CONTAINER)
    {
      while (son)
      {
        spread_mask_info(mask_object, son);
        son = son->next_sibling;
      }
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
void process_mask_info(void)
{
  int i;
  object_t *object;
  for (i = 0; i < object_count; i++)
  {
    object = object_list[i];
    if (object->object_tag == TAG_STENCIL ||
        object->object_tag == TAG_CLIP_BOX ||
        object->object_tag == TAG_CLIP_PLANE
        /*
        TAG_CLIP_ANGLE,
        TAG_CLIP_INSIDE,
        TAG_CLIP_START_POINT,
        */
      )
    {
      object_t *sibling = object->next_sibling;
      while (sibling)
      {
        if (sibling->object_tag != TAG_STENCIL &&
          sibling->object_tag != TAG_CLIP_BOX &&
          sibling->object_tag != TAG_CLIP_PLANE)
        {
          spread_mask_info(object, sibling);
        }
        sibling = sibling->next_sibling;
      }
    }
  }
}
