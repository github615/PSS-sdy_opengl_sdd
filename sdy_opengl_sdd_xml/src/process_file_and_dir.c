/* --------------------------- ���� ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2016, 2018 �Ϻ���ΤѶ��Ϣ�������޹�˾��
 *
 * @��������
 *
 **********************************************************************
*/

/* --------------------------- �ļ����� -----------------------------*/
#include <io.h>
#include <direct.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Shlwapi.h"

#include "common_definitions.h"
#include "sdy_config.h"
#include "scade_display_dir.h"

/* --------------------------- �궨�� -------------------------------*/
#define SDY_MODEL_CODE_DIR_NAME "ASIDE_SDY"

/* --------------------------- ���Ͷ��� -----------------------------*/

/* --------------------------- �������� -----------------------------*/
static char cmd[CMD_LENGTH];
static char sgfx_file_name[FILE_NAME_LENGTH];
static char distrib_dir[FILE_NAME_LENGTH];
static char sdy_source_dir[FILE_NAME_LENGTH];
static char output_dir[FILE_NAME_LENGTH];
static char sdy_obj_dir[FILE_NAME_LENGTH];

static char specification_name[FILE_NAME_LENGTH];
static char model_name[OBJECT_NAME_LENGTH];
static char prefix_str[OBJECT_NAME_LENGTH];

extern sdy_config_t sdy_config;

/* --------------------------- �������� -----------------------------*/
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
void delete_sdy_obj_dir(void)
{
  if (!_access(sdy_obj_dir, 0))
  {
    _chdir(sdy_obj_dir);
    _chdir("..");
    sprintf(cmd, "rmdir /S /Q \"%s\"", sdy_obj_dir);
    system(cmd);
  }
}
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
void process_dir(char *aside_sdy_engine_exe_name, char *etp_file_name_in, char *output_dir_in)
{
  char *p;
  char error_log_file_name[FILE_NAME_LENGTH];

  strcpy(distrib_dir, aside_sdy_engine_exe_name);
  convert_dir_format_for_windows(distrib_dir);
  PathRemoveFileSpecA(distrib_dir);
  p =strrchr(distrib_dir, '\\');
  if (strcmp(p, "\\bin") == 0)
  {
    *p = 0;
  }
  else if (strcmp(p, "\\Debug") == 0)
  {
    strcpy(p+1, "distrib");
  }
  else
  {
    fatal_error("Cant find distrib directory\n");
  }

  strcpy(sgfx_file_name, sdy_config.soure_file);

  strcpy(sdy_source_dir, sdy_config.soure_file);
  PathRemoveFileSpecA(sdy_source_dir);

  strcpy(specification_name, etp_file_name_in);
  p = strrchr(specification_name, '.');
  if (p)
  {
    *p = 0;
  }
  sprintf(model_name, "SDY1_%s", specification_name);
  sprintf(prefix_str, "%s_", model_name);

  if (_access(output_dir_in, 0))
  {
    fatal_error("%s does not exist\n", output_dir_in);
  }
  strcpy(output_dir, output_dir_in);
  convert_dir_format_for_windows(output_dir);
  PathRemoveBackslashA(output_dir);
  sprintf(sdy_obj_dir, "%s\\%s", output_dir, SDY_MODEL_CODE_DIR_NAME);
  delete_sdy_obj_dir();
  sprintf(cmd, "mkdir \"%s\"", sdy_obj_dir);
  system(cmd);

  sprintf(error_log_file_name, "%s\\aside_sdy_error.txt", output_dir);
  set_error_log_file(error_log_file_name);
}
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
void compose_sdy_source_path(char *sdy_source_file_name, char *sdy_source_path_out)
{
  char temp[FILE_NAME_LENGTH];
  strcpy(temp, sdy_source_file_name);
  PathRemoveFileSpecA(temp);
  PathCanonicalizeA(sdy_source_path_out, temp);
}
/**
 **********************************************************************
 *
 * @��������
 * ��Ȼ�ú���ò����compose_sdy_source_path���ƣ������ܸ�Ϊ��sdy_source_path
 * ���򻯴���
 *
 **********************************************************************
*/
void compose_ogfx_file_name(char *father_file_name, char *file_name_in, char *ogfx_file_name)
{
  char temp[FILE_NAME_LENGTH];
  if (file_name_in[1] == ':'/*���Ǿ���·��*/) 
  {
    strcpy(temp, file_name_in);
  }
  else
  {
    strcpy(temp, father_file_name);
    PathRemoveFileSpecA(temp);
    sprintf(temp + strlen(temp), "\\%s", file_name_in);
  }
  PathCanonicalizeA(ogfx_file_name, temp);
}
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
void compose_aside_sdy_object_info_file_name(char *aside_sdy_object_info_file_name)
{
  sprintf(aside_sdy_object_info_file_name, "%s\\aside_sdy_object_info.txt", output_dir);
}
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
char *get_distrib_dir(void)
{
  return distrib_dir;
}
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
char *get_sdy_source_dir(void)
{
  return sdy_source_dir;
}
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
char *get_sgfx_file_name(void)
{
  return sgfx_file_name;
}
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
char *get_specification_name(void)
{
  return specification_name;
}
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
char *get_model_name(void)
{
  return model_name;
}
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
char *get_prefix_str(void)
{
  return prefix_str;
}
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
char *get_output_dir(void)
{
  return output_dir;
}
/**
 **********************************************************************
 *
 * @��������
 *
 **********************************************************************
*/
char *get_sdy_obj_dir(void)
{
  return sdy_obj_dir;
}
