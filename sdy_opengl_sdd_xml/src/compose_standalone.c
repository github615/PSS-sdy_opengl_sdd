/* --------------------------- 序言 ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2018 上海爱韦讯信息技术有限公司。
 *
 * @功能描述
 *
 **********************************************************************
*/

/* --------------------------- 文件包含 -----------------------------*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common_definitions.h"
#include "process_file_and_dir.h"
#include "sdy_source.h"
#include "utilities.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 * @设计注记
 *  在standalone中插入Application_RefreshCallback(pThis, aside_opengl_gen_image);
 *  不能画图。
 *
 **********************************************************************
*/
void compose_standalone(int target_periodicity)
{
  static const char *stmt_1 = 
    "#include \"sgl.h\"\n"
    "#include \"sdy_events.h\"\n"
    "#include \"sdy_application.h\"\n"
    "#include \"aside_opengl_gen_image.h\"\n"
    ;
  static const char *stmt_2 = 
    "static sdy_device_data_t g_pointers_data[] = {\n"
    "{\"_ptr0\", 0},\n"
    "};\n"
    "static sdy_device_data_t g_keyboards_data[] = {\n"
    "{\"_kbd0\", 0},\n"
    "};\n"
    ;
  static const char *stmt_3 = 
    "SGL_FALSE,\n"
    "SGL_FALSE,\n"
    "SGL_TRUE,\n"
    ;
  static const char *stmt_4 = 
    "SGL_NULL, SGL_NULL, &BHVR_SetAsynchronousACK, SGL_NULL, SGL_NULL, SGL_NULL,\n"
    ;
  static const char *stmt_5 = 
    "1, g_pointers_data,\n"
    "1, g_keyboards_data,\n};\n"
    ;
  static const char *stmt_6 =
    "#include <time.h>\n"
    "#include <stdio.h>\n"
    "#include <windows.h>\n"
    "\n"
    "int main(int argc, char** argv)\n"
    "{\n"
    "    sdy_application_t *pThis = NULL;\n"
    "\n"
    "    clock_t start, finish;\n"
    "    long loc_cycle_period = g_application_data.target_periodicity;\n"
    "    long lDuration, loc_time_sleep;\n"
    "    int i;\n"
    "\n"
    "    init_aside_opengl_gen_image(argv[argc-1]);\n"
    "    argc--;\n\n"
    "    for (i = 1; i < argc; i++) {\n"
    "      if ((strcmp(argv[i], \"-period\") == 0) && ((i + 1) < argc)) {\n"
    "        i += 1;\n"
    "        loc_cycle_period = (long) atoi(argv[i]);\n"
    "      }\n"
    "      else if (i+1 == argc) {\n"
    "        loc_cycle_period = (long) atoi(argv[i]);\n"
    "      }\n"
    "    }\n"
    "\n"
    "    pThis = Application_Allocate(&g_application_data, GetModuleHandle(NULL));\n"
    "    Application_UseSystemEvents(pThis, SGL_TRUE);\n"
    "    Application_CreateWindow(pThis, NULL);\n"
    "\n"
    "    while (!Application_Cancelled(pThis)){\n"
    "      start = clock();\n"
    "      Application_Draw(pThis);\n"
    "      finish = clock();\n"
    "      lDuration = (long)((double)(finish - start) / CLOCKS_PER_SEC * 1000.0);\n"
    "      loc_time_sleep = loc_cycle_period - lDuration;\n"
    "\n"
    "      if ( loc_time_sleep > 0 ){\n"
    "        SleepEx(loc_time_sleep, FALSE);\n"
    "      }\n"
    "    }\n"
    "\n"
    "    Application_Free(pThis);\n"
    "\n"
    "    return 0;\n"
    "}\n"
    ;

  char file_out_name[FILE_NAME_LENGTH];
  FILE *fout;
  int i;
  layer_info_t *info;
  char *prefix;
  char prefix_upper[ATTR_LENGTH];

  prefix = get_prefix_str();
  for (i = 0; i < (int)strlen(prefix); i++)
  {
    prefix_upper[i] = toupper(prefix[i]);
  }
  prefix_upper[i] = 0;

  sprintf(file_out_name, "%s\\Specific\\standalone.c", get_sdy_obj_dir());
  fout = fopen(file_out_name, "w");
  fputs(stmt_1, fout);

  fprintf(fout, "\n#include \"%scolor_table.h\"\n", prefix);
  fprintf(fout, "#include \"%sline_width_table.h\"\n", prefix);
  fprintf(fout, "#include \"%sline_stipple_table.h\"\n", prefix);
  fprintf(fout, "#include \"%stexture_table.h\"\n", prefix);
  fprintf(fout, "#include \"%sfont_table.h\"\n", prefix);

  for (i = 0, info = layer_info_list; i < layer_info_count; i++, info++)
  {
    fprintf(fout, "#include \"%s%s.h\"\n", prefix, info->layer_name);
  }

  fprintf(fout, "\nstatic sdy_layer_data_t g_layers_data[] = {\n");
  for (i = 0, info = layer_info_list; i < layer_info_count; i++, info++)
  {
    fprintf(fout, "{\n\"%s\", %sF, %sF,\n", 
      info->layer_name, info->ratio_horizontal, info->ratio_vertical);//2018-12-7
    fprintf(fout, "(T_RETURN_PTR_FUNCTION)&%s%s_alloc,\n", prefix, info->layer_name);
    fprintf(fout, "(T_VOIDPTR_FUNCTION)&%s%s_free,\n", prefix, info->layer_name);
    fprintf(fout, "(T_VOIDPTR_FUNCTION)&%s%s_init,\n", prefix, info->layer_name);
    fprintf(fout, "(T_VOIDPTR_FUNCTION)&%s%s_predraw,\n", prefix, info->layer_name);
    fprintf(fout, "(T_VOIDPTR_INT_FUNCTION)&%s%s_draw,\n", prefix, info->layer_name);
    fprintf(fout, "0, SGL_NULL, 0, SGL_NULL,\n},\n");
  }
  fprintf(fout, "};\n");
  fputs(stmt_2, fout);

  fprintf(fout, "\nsdy_application_data_t g_application_data = {\n\"%s\",\n", get_specification_name());
  fputs(stmt_3, fout);
  fprintf(fout, "%s, %s,\n%d,\n", sgfx_dimension_width, sgfx_dimension_height, target_periodicity);
  fputs(stmt_4, fout);

  fprintf(fout, "%scolor_table, &%scolor_table_size, &%scolor_table_background_index,\n",
    prefix, prefix, prefix);
  fprintf(fout, "%sline_width_table, &%sline_width_table_size,\n",
    prefix, prefix);
  fprintf(fout, "%sline_stipple_table, &%sline_stipple_table_size,\n",
    prefix, prefix);
  fprintf(fout, "&%sfont_table,\n", prefix);
  fprintf(fout, "&%stexture_table, %sTEXTURE_TABLE_SIZE,\n",
    prefix, prefix_upper);
  fprintf(fout, "%d, g_layers_data,\n", layer_info_count);

  fputs(stmt_5, fout);
  fputs(stmt_6, fout);
  fclose(fout);
}
