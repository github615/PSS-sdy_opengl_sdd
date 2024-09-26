#ifndef __SDY_CONFIG_H__
#define __SDY_CONFIG_H__

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
#include "common_definitions.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/
typedef struct  
{
  char name[ATTR_LENGTH];
  char rgb[ATTR_LENGTH];
  char ensemble[ATTR_LENGTH];
} color_def_t;

typedef struct  
{
  char visible[ATTR_LENGTH];
  int value;
} line_stipple_length_attr_t;

typedef struct  
{
  char stipple_type[ATTR_LENGTH];
  line_stipple_length_attr_t *line_stipple_length_attr_table;
  int line_stipple_length_attr_count;
  int line_stipple_length_attr_size;
} line_stipple_def_t;

typedef struct  
{
  char width[ATTR_LENGTH];
  char color_width[ATTR_LENGTH];
  char halo_width[ATTR_LENGTH];
  char value[ATTR_LENGTH];
  char name[ATTR_LENGTH];
  char ensemble[ATTR_LENGTH];
} line_width_t;

typedef struct  
{
  char height[ATTR_LENGTH];
  char name[ATTR_LENGTH*5];
  char repeatable[ATTR_LENGTH];
  char width[ATTR_LENGTH];
} bit_map_t;

typedef struct  
{
  char color_table_value[ATTR_LENGTH];
  char color_table_file[FILE_NAME_LENGTH];
  char line_width_table_value[ATTR_LENGTH];
  char line_width_table_file[FILE_NAME_LENGTH];
  char line_stipple_table_value[ATTR_LENGTH];
  char line_stipple_table_file[FILE_NAME_LENGTH];
  char texture_table_value[ATTR_LENGTH];
  char texture_table_file[FILE_NAME_LENGTH];
  char font_table_value[ATTR_LENGTH];
  char font_table_file[FILE_NAME_LENGTH];
} resource_table_t;

typedef struct
{
  unsigned config_id;
  char config_name[ATTR_LENGTH];
  //char source_value[ATTR_LENGTH];
  char soure_file[FILE_NAME_LENGTH];
} sdy_config_t;

/* --------------------------- 变量声明 -----------------------------*/
extern color_def_t *color_table;
extern int max_color_index;
extern char **font_table;
extern int font_count;
extern line_stipple_def_t *line_stipple_table;
extern int max_line_stipple_index;
extern line_width_t *line_width_table;
extern int max_line_width_index;
extern bit_map_t *texture_table;
extern int max_texture_index;

/* --------------------------- 函数声明 -----------------------------*/
void parse_sdy_project(char *sdy_source_dir_in, char *etp_file_name_in);
void save_environment_id(char *environment_id_in);
void save_color_mode(char *color_mode_in);
void parse_color_table(void);
void free_color_table(void);
void parse_font_table(char *sdy_source_dir_in);
void free_font_table(void);
void parse_line_width_table(void);
void free_line_width_table(void);
void parse_line_stipple_table(void);
void free_line_stipple_table(void);
void parse_texture_table(void);
void free_texture_table(void);
void gen_general_config_content(char *content_buffer);
void gen_color_table_content(char *content_buffer);
void gen_font_table_content(char *content_buffer);
void gen_line_stipple_table_content(char *content_buffer);
void gen_line_width_table_content(char *content_buffer);
void gen_texture_table_content(char *content_buffer);

#endif    /*  __SDY_CONFIG_H__  */
