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
#include <io.h>
#include <direct.h>
#include "Shlwapi.h"
#include "common_definitions.h"
#include "sdy_config.h"
#include "sdy_source.h"
#include "scade_display_dir.h"
#include "process_file_and_dir.h"

/* --------------------------- 宏定义 -------------------------------*/
#define MAKE_OPENGL_MODEL_BAT_FILE_NAME "make_opengl_model.bat"
#define HOLDER_MODEL_NAME "XXXXX"
#define HOLDER_LAYER_NAME "YYYYY"
#define SDY_DRAW_CODE_COUNT 500

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
/*2018-12-7 not used in gen_general_config_content
static char dimension_width_and_height[ATTR_LENGTH];
*/
static int target_periodicity = 33;

static char cmd[CMD_LENGTH];
static char sdy_draw_code_list[SDY_DRAW_CODE_COUNT][FILE_NAME_LENGTH];
static int sdy_draw_code_count;
extern resource_table_t resource_table;

/* --------------------------- 函数声明 -----------------------------*/
void compose_standalone(int target_periodicity);

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static int search_get_expr_stmt(char *line, char *expr, char *stmt)
{
  char *p, *p2;
  char *stmt_begin, *stmt_end;
  p = line;
  while (p = strstr(p, "aol_"))
  {
    stmt_begin = p;
    if (p = strstr(p, "_S_")) continue;
    p = strstr(stmt_begin, "_G_");
    if (!p) return 0;
    p += 3;
    strcpy(expr, p);
    p2 = strstr(p, "(pContext)");
    if (!p2) return 0;
    stmt_end = p2 + strlen("(pContext)");
    *(expr + (p2 - p)) = 0;
    strcpy(stmt, stmt_begin);
    *(stmt + (stmt_end - stmt_begin)) = 0;
    return 1;
  }
  return 0;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static char *search_expr_init_val(int obj_id, char *target_expr)
{
  object_t *o = object_list[obj_id];
  object_prop_t *prop = o->prop_head;
  int i;

  while (prop)
  {
    char *src_expr = prop->value_and_expr.expr;
    if (src_expr)
    {
      //skip "real " in expr: <expr>real pipUpTwoDisplayHundred</expr>
      char *p = strchr(src_expr, ' ');
      if (p) src_expr = p + 1;
      if (strcmp(src_expr, target_expr) == 0 &&
          prop->value_and_expr.value &&
          /* do not use below init value
          <parameter control="false" name="UNIT" oid="cd6a0f23-daa2-4e4e-8527-c8cc38b851df" representation="as_string" type="char^255">
          <init>[84, 82, 47, 77, 0]</init>
          </parameter>
          */
          prop->value_and_expr.value[0] != '['
        )
      {
        return prop->value_and_expr.value;
      }
    }
    prop =prop->next;
  }

  i = o->first_input_ref_parameter_info_id;
  if (i >= 0)
  {
    ref_parameter_info_t *info = &ref_parameter_info_list[i];
    for (i = 0; i < o->input_ref_parameter_count; i++, info++)
    {
      if (info->real_para_name &&
          strcmp(info->real_para_name, target_expr) == 0 &&
          info->init_value[0] != '[')
      {
        return info->init_value;
      }
    }
  }
  return NULL;
}
/**
 **********************************************************************
 *
 * @功能描述
 * @设计注记
 *  在R16中，第一层ogfx文件生成的c代码中第5行注释的Specification File是
 *  根sgfx文件名，而第二层ogfx文件生成的c代码中第5行注释的Specification File是
 *  本身的ogfx文件名，因此不能用以前R15的方法设计本函数。
 *
 **********************************************************************
*/
int search_first_object_id(char *file_in_name, FILE *file_in, int *input_scope_id)
{
  char line[LINE_LENGTH];
  char spec_file_name[FILE_NAME_LENGTH];
  int id = -1;
  int i;
  char *p;

  *input_scope_id = 0;

  while (fgets(line, LINE_LENGTH, file_in))
  {
    if (p = strstr(line, "Specification File:  "))
    {
      strcpy(spec_file_name, p+21);
      spec_file_name[strlen(spec_file_name)-1] = 0;
      break;
    }
  }

  if (strstr(spec_file_name, ".sgfx"))
  {
    if (layer_info_count == 1 || strstr(file_in_name, "symbology_layer"))
    {
      id = 1;
    }
    else
    {
      layer_info_t *info = layer_info_list;
	    //SDY1_TopLayer_TopLayer_draw.c掐头（SDY1_TopLayer）去尾（_draw.c）变为TopLayer
      char temp[FILE_NAME_LENGTH];
	    char *q;
      
      strcpy(temp, file_in_name);
      q = strstr(temp, get_model_name());
	    q += strlen(get_model_name()) + 1;
	    p = strstr(q, "_draw.c");
	    *p = '\0';
	    p = q;
      
      for (i = 0; i < layer_info_count; i++, info++)
      {
        if (strstr(p, info->layer_name))
        {
          id = info->object_id;
          break;
        }
      }
    }
  }
  else
  {
    ogfx_info_t *info = global_ogfx_info_list;
    for (i = 0; i < global_ogfx_info_count; i++, info++)
    {
      if (strcmp(spec_file_name, info->file_name) == 0)
      {
        id = info->ogfx_root->id;
        *input_scope_id = i + 1;
        break;
      }
    }
  }
  if (id == -1)
    fatal_error("search_first_object_id: can't find %s\n", spec_file_name);
  return id;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void modify_sdy_draw_code(char *file_name)
{
  char file_in_name[FILE_NAME_LENGTH];
  char file_out_name[FILE_NAME_LENGTH];
  FILE *file_in, *file_out;
  char line[LINE_LENGTH];
  int first_object_id;
  int input_scope_id;
  int current_object_id;

  sprintf(file_in_name, "%s\\Code\\%s", get_sdy_obj_dir(), file_name);
  file_in = fopen(file_in_name, "r");
  first_object_id = search_first_object_id(file_in_name, file_in, &input_scope_id);

  sprintf(file_out_name, "%s\\temp.c", get_sdy_obj_dir());
  file_out = fopen(file_out_name, "w");
  fseek(file_in, 0, SEEK_SET);
  while (fgets(line, LINE_LENGTH, file_in))
  {
    char *p;
    char expr[LINE_LENGTH];
    char stmt[LINE_LENGTH];

    if (strstr(line, " switch (") && !strstr(line, "switch (pPriority)"))
    {
      fprintf(file_out, "/*%s*/", line);
      fprintf(file_out, "switch (aside_sdy_condition_expr(%d)) {\n", 
        object_list[current_object_id]->indexes_id);
    }
    else if (strstr(line, "if ("))  /* 修改因visibility是expr而生成的条件语句 */
    {
      fprintf(file_out, "/*%s*/", line);
      fprintf(file_out, "if (1) {\n");
    }
    else if (search_get_expr_stmt(line, expr, stmt)) /* 把取变量改为取初值 */
    {
      char *init_val = search_expr_init_val(current_object_id, expr);
      if (init_val)
      {
        char new_line[LINE_LENGTH];
        strrpl(new_line, line, stmt, init_val);
        fprintf(file_out, "if (aside_sdy_in_same_scope(%d))\n", current_object_id);
        fputs(new_line, file_out);
        fputs("else\n", file_out);
        fputs(line, file_out);
      }
      else
      {
        fprintf(file_out, "/*??? no init val found*/\n");
        fputs(line, file_out);
      }
    }
    else if (strstr(line, "#include \"sgl_simu.h\""))
    {
      fputs(line, file_out);
      fputs("#include \"aside_decision.h\"\n", file_out);
      /*2018-12-1
      在R15的process_sdy_code.c中把以下宏插入到SCADE生成的draw文件中,
      但不记得其作用, 这些可能是条件语句中会出现的宏.
      不能把以下宏移到aside_decision.h中
      */
      fputs("#define true SGL_TRUE\n", file_out);
      fputs("#define false SGL_FALSE\n", file_out);

      fputs("#define left 0\n", file_out);
      fputs("#define center 1\n", file_out);
      fputs("#define right 2\n", file_out);
      fputs("#define baseline 0\n", file_out);
      fputs("#define middle 1\n", file_out);
      fputs("#define top 2\n", file_out);
    }
    else
    {
      fputs(line, file_out);
      if (p = strstr(line, "/* Object"))
      {
        int i;
        p += 10;
        sscanf(p, "%d", &i);
        current_object_id = first_object_id + i;
        sprintf(line, "if (aside_sdy_is_enable(%d))\n", current_object_id);
        set_object_class_and_image_name(current_object_id);
        fputs(line, file_out);
      }
    }
  }
  fclose(file_in);
  fclose(file_out);
  sprintf(cmd, "copy /Y \"%s\" \"%s\" >nul", file_out_name, file_in_name);
  system(cmd);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void modify_last_sdy_draw_code()
{
  char file_in_name[FILE_NAME_LENGTH];
  char file_out_name[FILE_NAME_LENGTH];
  FILE *file_in, *file_out;
  char line[LINE_LENGTH];

  sprintf(file_in_name, "%s\\Code\\%s%s_draw.c", 
    get_sdy_obj_dir(), get_prefix_str(), layer_info_list[layer_info_count-1].layer_name);
  file_in = fopen(file_in_name, "r");

  sprintf(file_out_name, "%s\\temp.c", get_sdy_obj_dir());
  file_out = fopen(file_out_name, "w");
  fseek(file_in, 0, SEEK_SET);

  while (fgets(line, LINE_LENGTH, file_in))
  {
    fputs(line, file_out);
    if (strstr(line, "#include \"sgl_simu.h\""))
    {
      fputs("#include \"asmt_display_server.h\"\n", file_out);
      break;
    }
  }

  while (fgets(line, LINE_LENGTH, file_in))
  {
    if (line[0] == '}')
    {
      fputs("aside_opengl_gen_image();\n", file_out);
    }
    fputs(line, file_out);
  }

  fclose(file_in);
  fclose(file_out);
  sprintf(cmd, "copy /Y \"%s\" \"%s\" >nul", file_out_name, file_in_name);
  system(cmd);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void modify_all_sdy_draw_code(void)
{
  int i;
  for (i = 0; i < sdy_draw_code_count; i++)
  {
    modify_sdy_draw_code(sdy_draw_code_list[i]);
  }

  modify_last_sdy_draw_code();
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void compose_target_configuration_c_file(void)
{
  char file_out_name[FILE_NAME_LENGTH];
  FILE *file_out;

  sprintf(file_out_name, "%s\\Specific\\target_configuration.c", get_sdy_obj_dir());
  file_out = fopen(file_out_name, "w");

  fprintf(file_out, "#include \"target_configuration.h\"\n");
  fprintf(file_out, "unsigned long target_screen_width = %s;\n", sgfx_dimension_width);
  fprintf(file_out, "unsigned long target_screen_height = %s;\n", sgfx_dimension_height);
  fprintf(file_out, "const double ratio_x = %s;\n", sgfx_ratio_horizontal);
  fprintf(file_out, "const double ratio_y = %s;\n", sgfx_ratio_vertical);
  fprintf(file_out, "const unsigned char target_rotation_mode = 0;\n");
  fprintf(file_out, "const long target_periodicity = %d;\n", target_periodicity);
  fprintf(file_out, "const char *specification_name = \"%s\";\n", get_specification_name());
  fclose(file_out);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void search_sdy_draw_code(char *code_dir)
{  
  struct _finddata_t FileInfo;
  intptr_t Handle;
  char target[FILE_NAME_LENGTH];

  sprintf(target, "%s\\*_draw.c", code_dir);
  if((Handle =_findfirst(target, &FileInfo)) == -1L) //查找目录中符合条件的文件
  {
    fatal_error("no file is found for %s\n", target);
  }
  else
  {
    do
    {
      if(!(FileInfo.attrib & _A_SUBDIR))
      {
        strcpy(sdy_draw_code_list[sdy_draw_code_count++], FileInfo.name);
        check_buffer_overflow(sdy_draw_code_count, SDY_DRAW_CODE_COUNT, "SDY_DRAW_CODE_COUNT");
      }
    } while(!_findnext(Handle, &FileInfo)); 
    _findclose(Handle);    
  } 
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void build_new_file(char *template_name, char *dst_file_name, char *sub_dir_name)
{
  FILE *src_file;
  FILE *tmp_file;
  char line[LINE_LENGTH];
  char new_line[LINE_LENGTH];
  char src_file_name[FILE_NAME_LENGTH];
  char tmp_file_name[FILE_NAME_LENGTH];
  char *layer_name = object_list[1]->object_name;

  sprintf(src_file_name, "%s\\template\\%s", get_distrib_dir(), template_name);
  sprintf(tmp_file_name, "%s\\tmp.txt", get_sdy_obj_dir());
  src_file = fopen(src_file_name, "r");
  tmp_file = fopen(tmp_file_name, "w");

  while(fgets(line, LINE_LENGTH, src_file))
  {
    if (strstr(line, HOLDER_MODEL_NAME))
    {
      strrpl(new_line, line, HOLDER_MODEL_NAME, get_model_name());
      strcpy(line, new_line);
    }
    if (strstr(line, HOLDER_LAYER_NAME))
    {
      strrpl(new_line, line, HOLDER_LAYER_NAME, layer_name);
      strcpy(line, new_line);
    }
    fputs(line, tmp_file);
  }
  fclose(src_file);
  fclose(tmp_file);
  if (sub_dir_name)
  {
    sprintf(cmd, "copy /Y \"%s\" \"%s\\%s\\%s\" >nul", tmp_file_name, get_sdy_obj_dir(), sub_dir_name, dst_file_name);
  }
  else
  {
    sprintf(cmd, "copy /Y \"%s\" \"%s\\%s\" >nul", tmp_file_name, get_sdy_obj_dir(), dst_file_name);
  }
  system(cmd);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void gen_sdy_code(void)
{
  char template_dir[FILE_NAME_LENGTH];
  char code_dir[FILE_NAME_LENGTH];
  char specific_dir[FILE_NAME_LENGTH];

  sprintf(template_dir, "%s\\template", get_distrib_dir());
  sprintf(code_dir, "%s\\Code", get_sdy_obj_dir());
  sprintf(specific_dir, "%s\\Specific", get_sdy_obj_dir());

  sprintf(cmd, "xcopy /Y /E \"%s\\sdy\" \"%s\" >nul", template_dir, get_sdy_obj_dir());
  system(cmd);

  sprintf(code_dir, "%s\\Code", get_sdy_obj_dir());
  if (_access(code_dir, 0))
  {
    sprintf(cmd, "mkdir \"%s\"", code_dir);
    system(cmd);
  }

  sprintf(cmd, "call \"%s\" -outdir \"%s\\Code\" -prefix %s \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
    get_ScadeDisplayCGSimu_exe(),
    get_sdy_obj_dir(),
    get_prefix_str(),
    get_sgfx_file_name(),
    resource_table.texture_table_file,
    resource_table.font_table_file,
    resource_table.color_table_file,
    resource_table.line_width_table_file,
    resource_table.line_stipple_table_file
    );
  system(cmd);

  search_sdy_draw_code(code_dir);
  modify_all_sdy_draw_code();
  compose_target_configuration_c_file();
  compose_standalone(target_periodicity);
  build_new_file("interactivity_mapping_c.template", "interactivity_mapping.c", "Code");
  sprintf(cmd, "copy /Y \"%s\\aside_decision.*\" \"%s\" >nul", template_dir, specific_dir);
  system(cmd);
  sprintf(cmd, "copy /Y \"%s\\aside_opengl_gen_image.*\" \"%s\" >nul", template_dir, specific_dir);
  system(cmd);
  sprintf(cmd, "copy /Y \"%s\\FreeImage.h\" \"%s\" >nul", template_dir, specific_dir);
  system(cmd);
  sprintf(cmd, "copy /Y \"%s\\FreeImage.lib\" \"%s\" >nul", template_dir, specific_dir);
  system(cmd);
  sprintf(cmd, "copy /Y \"%s\\FreeImage.dll\" \"%s\" >nul", template_dir, get_sdy_obj_dir());
  system(cmd);
  sprintf(cmd, "copy /Y \"%s\\glut32.dll\" \"%s\" >nul", template_dir, get_sdy_obj_dir());
  system(cmd);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void compose_makefile(void)
{
  static const char *stmt_1 = 
    "GCC32=/mingw_w64/bin/gcc -m32\n"
    "LS=/bin/ls\n"
    "RM=/bin/rm\n"
    "SED=/bin/sed\n"
    ;
  static const char *stmt_2 = 
    "libs = -lopengl32 -lws2_32 -lkernel32 -luser32 -lgdi32 ./Specific/FreeImage.lib $(OGLX_EXTRAS)/glut32/glut32.lib\n"
    "cg_sources = $(wildcard ./Code/*.c)\n"
    "specific_sources = $(wildcard ./Specific/*.c)\n"
    "dllsources = $(cg_sources) $(specific_sources)\n\n"
    "static_libs =  $(wildcard ./Lib/*.o) $(wildcard ./Lib/*.a) $(wildcard ./Lib/*.lib)\n\n"
    "all : clean exe\n\n"
    ;
  static const char *stmt_3 = 
    "sources.lst::\n"
    "\t$(LS) -Q $(dllsources)  | $(SED) -e 's!\"/\\([A-Za-z]\\)/!\"\\1:/!' >sources.lst\n"
    "\nclean::\n"
    "\t-$(RM) -f *.o\n"
    ;
  char new_scade_suite_dir[FILE_NAME_LENGTH];
  char new_oglx_extras_dir[FILE_NAME_LENGTH];
  char file_out_name[FILE_NAME_LENGTH];
  FILE *file_out;
  char *model_name = get_model_name();

  convert_dir_format_for_mingw(get_scade_suite_dir(), new_scade_suite_dir);
  convert_dir_format_for_mingw(get_scade_display_oglx_extras_dir(), new_oglx_extras_dir);

  sprintf(file_out_name, "%s\\Makefile", get_sdy_obj_dir());
  file_out = fopen(file_out_name, "w");
  fputs(stmt_1, file_out);
  fprintf(file_out, "OGLX_EXTRAS = %s\n", new_oglx_extras_dir);
  fprintf(file_out, "includes = -Ui386 -I./Include -I./Include/oglx -I./Code -I./Specific -I$(OGLX_EXTRAS)/freeglut32 -I%s\n",
    new_scade_suite_dir);

  fputs(stmt_2, file_out);

  fputs("dll:: sources.lst\n", file_out);
  fprintf(file_out, "\t$(GCC32) $(includes) -DDLL_EXPORTS  @sources.lst -mwindows -mwin32 -shared -o %s.dll $(static_libs) $(libs)\n\n",
    model_name);

  fputs("dlldef:: sources.lst\n", file_out);
  fprintf(file_out, "\t$(GCC32) $(includes) -DDLL_EXPORTS  @sources.lst -mwindows -mwin32 -shared -Wl,--output-def,%s.def,--out-implib,%s.a -o %s.dll $(static_libs) $(libs)\n\n",
    model_name, model_name, model_name);

  fputs("exe:: sources.lst\n", file_out);
  fprintf(file_out, "\t$(GCC32) $(includes) -DDLL_EXPORTS  @sources.lst -mwindows -mwin32 -o %s.exe $(static_libs) $(libs)\n\n",
    model_name);

  fputs(stmt_3, file_out);
  fclose(file_out);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void compose_make_opengl_model_bat_file(void)
{
  static const char *stmt_1 = 
    "setlocal\n";
  static const char *stmt_2 = 
    "make --no-builtin-rules\n"
	  "endlocal\n";

  char new_scade_display_dir[FILE_NAME_LENGTH];
  char file_out_name[FILE_NAME_LENGTH];
  FILE *file_out;

  strrpl(new_scade_display_dir, get_scade_display_dir(), "\\", "/");

  sprintf(file_out_name, "%s\\%s", get_sdy_obj_dir(), MAKE_OPENGL_MODEL_BAT_FILE_NAME);
  file_out = fopen(file_out_name, "w");
  fputs(stmt_1, file_out);
  fprintf(file_out, "path %s/../contrib/Msys/bin;%s/bin;%%PATH%%\n", new_scade_display_dir, new_scade_display_dir);
  fputs(stmt_2, file_out);
  fclose(file_out);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void build_and_run_aside_sdy_opengl_exe(void)
{
  compose_makefile();
  compose_make_opengl_model_bat_file();
  _chdir(get_sdy_obj_dir());

  system(MAKE_OPENGL_MODEL_BAT_FILE_NAME);

  sprintf(cmd, "call %s.exe ..", get_model_name()); // ..是obj_dir的父目录
  system(cmd);
}