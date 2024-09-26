#ifndef __PROCESS_FILE_AND_DIR_H__
#define __PROCESS_FILE_AND_DIR_H__

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

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 函数声明 -----------------------------*/
void delete_sdy_obj_dir(void);
void process_dir(char *aside_sdy_engine_exe_name, char *etp_file_name_in, char *output_dir_in);
void compose_sdy_source_path(char *sdy_source_file_name, char *sdy_source_path_out);
void compose_ogfx_file_name(char *father_file_name, char *file_name_in, char *ogfx_file_name);
void compose_aside_sdy_object_info_file_name(char *aside_sdy_object_info_file_name);
char *get_distrib_dir(void);
char *get_sdy_source_dir(void);
char *get_sgfx_file_name(void);
char *get_specification_name(void);
char *get_model_name(void);
char *get_prefix_str(void);
char *get_output_dir(void);
char *get_sdy_obj_dir(void);

#endif    /*  __PROCESS_FILE_AND_DIR_H__  */
