/* --------------------------- 序言 ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2013, 2018 上海爱韦讯信息技术有限公司。
 *
 * @功能描述
 *
 **********************************************************************
*/

/* --------------------------- 文件包含 -----------------------------*/
#include "aside_decision.h"

/* --------------------------- 宏定义 -------------------------------*/
#define PRIMITIVE_OBJECT 1
#define FILE_NAME_LENGTH 500

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
extern char object_class_list[OBJECT_COUNT];
extern int object_scope_list[OBJECT_COUNT];
extern char object_lineal_matrix[OBJECT_COUNT][OBJECT_COUNT];
extern int condition_indexes_list[OBJECT_COUNT][INDEXES_COUNT];

FILE *log_file;
int active_object_id;
int active_object_is_reached;
int ancestry_object_count;
int posterity_object_count;
int image_count;

/* --------------------------- 函数声明 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void open_log_file(char *image_file_dir_name)
{
  char log_file_name[FILE_NAME_LENGTH];
  sprintf(log_file_name, "%s\\aside_sdy_log.txt", image_file_dir_name);
  log_file = fopen(log_file_name, "w");
  
  image_count = 0;
  active_object_id = 0;
  active_object_is_reached = 1; //for dump 00000.jpeg
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void close_log_file(void)
{
  fclose(log_file);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
int get_active_object_id(void)
{
  return active_object_id;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
int increase_active_object_id(void)
{
  do
  {
    active_object_id++;
  } while (object_class_list[active_object_id] <= PRIMITIVE_OBJECT);
  return active_object_id;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
int aside_sdy_condition_expr(int indexes_id)
{
  return condition_indexes_list[active_object_id][indexes_id];
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
int aside_sdy_is_enable(int object_id)
{
  int is_enable;

  if (object_id == active_object_id)
  {
    is_enable = 1;
    active_object_is_reached = 1;
  }
  else if (object_lineal_matrix[object_id][active_object_id] == 1)
  {
    is_enable = 1;
    ancestry_object_count++;
    if (ancestry_object_count == 1)
    {
      fprintf(log_file, "ancestry_object:\n");
    }
    fprintf(log_file, "%d ", object_id);
    if ((ancestry_object_count % 20) == 0)
    {
      fprintf(log_file, "//[%d]\n", ancestry_object_count);
    }
  }
  else if (object_lineal_matrix[active_object_id][object_id] == 1)
  {
    is_enable = 1;
    posterity_object_count++;
    if (posterity_object_count == 1)
    {
      fprintf(log_file, "\nposterity_object:\n");
    }
    fprintf(log_file, "%d ", object_id);
    if ((posterity_object_count % 20) == 0)
    {
      fprintf(log_file, "//[%d]\n", posterity_object_count);
    }
  }
  else
  {
    is_enable = 0;
  }
  return is_enable;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
int aside_sdy_in_same_scope(int object_id)
{
	return object_scope_list[object_id] == object_scope_list[active_object_id];
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void reset_parameters(void)
{
  ancestry_object_count = 0;
  posterity_object_count = 0;
  active_object_is_reached = 0;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
int get_active_object_is_reached(void)
{
  return active_object_is_reached;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void log_object_is_not_reached(int object_id)
{
  fprintf(log_file, "object %d is not reached\n", object_id);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void log_object_summary(int active_object_id)
{
  fprintf(log_file, "\nobject_id=%d ancestry_object_count=%d posterity_object_count=%d image_count=%d\n*****\n",
    active_object_id, ancestry_object_count, posterity_object_count, ++image_count);
}
