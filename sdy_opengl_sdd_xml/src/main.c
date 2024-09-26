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
#include "free_memory.h"
#include "process_file_and_dir.h"
#include "process_sdy_code.h"
#include "scade_display_dir.h"
#include "sdy_config.h"
#include "sdy_source.h"
#include "utilities.h"
#ifdef _DEBUG
#include "vld.h" 
#endif
void dump_table_length_info();

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
int main(int argc, char *argv[])
{
  int i;
  int gen_image = 1;
  char xml_file_name[FILE_NAME_LENGTH];

  if (argc < 4)
  {
    fatal_error("Usage: <输入目录名> <etp文件名> <输出目录名> [language=<>] [including_ogfx_children=<>] [no_image]\n");
  }

  for (i = 4; i < argc; i++)
  {
    char *p;
    if ((p = strstr(argv[i], "language=")))
    {
      set_language(strchr(p, '=') + 1);
    }
    else if ((p = strstr(argv[i], "including_ogfx_children=")))
    {
      set_including_ogfx_children(strchr(p, '=') + 1);
    }
    else if (strstr(argv[i], "no_image"))
    {
      gen_image = 0;
    }
    else
    {
      fatal_error("Invalid option: %s\n", argv[i]);
    }
  }
  parse_sdy_project(argv[1], argv[2]);
  process_dir(argv[0], argv[2], argv[3]);
  search_scade_display_dir();
  parse_sdy_source(get_sgfx_file_name(), NULL, 0);

  parse_color_table();
  parse_font_table(argv[1]);
  parse_line_width_table();
  parse_line_stipple_table();
  parse_texture_table();

  set_specific_draw_callee(get_sgfx_file_name());
  enhance_layer_info();
  enhance_object_info();
  process_mask_info();

  inherit_trace_comment("[HLR]");

  gen_sdy_code();

  generate_aside_sdy_object_relation();
  generate_aside_sdy_object_info();
  gen_ogfx_next_variable_dictionary_info_id();

  sprintf(xml_file_name, "%s_sdd_%s.xml", get_specification_name(), 
    language == CHINESE ? "chn" : "eng");
  gen_sdd_xml(argv[3], xml_file_name);
  dump_table_length_info();

  if (gen_image)
  {
    build_and_run_aside_sdy_opengl_exe();
  }
  free_memory();
#ifdef NDEBUG
  delete_sdy_obj_dir();
#endif
  return 0;
 }

