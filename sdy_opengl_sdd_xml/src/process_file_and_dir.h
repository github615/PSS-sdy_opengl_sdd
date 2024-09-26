#ifndef __PROCESS_FILE_AND_DIR_H__
#define __PROCESS_FILE_AND_DIR_H__

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

/* --------------------------- �궨�� -------------------------------*/

/* --------------------------- ���Ͷ��� -----------------------------*/

/* --------------------------- �������� -----------------------------*/
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
