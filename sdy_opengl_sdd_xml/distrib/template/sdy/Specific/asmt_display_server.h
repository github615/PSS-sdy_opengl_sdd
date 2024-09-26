#ifndef __ASMT_DISPLAY_SERVER_H
#define __ASMT_DISPLAY_SERVER_H
/* --------------------------- ���� ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2014 �Ϻ���ΤѶ��Ϣ�������޹�˾��
 *
 * @��������
 *
 **********************************************************************
*/

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API extern
#endif

DLL_API void asmt_display_server_init(char *shm_name);
DLL_API void asmt_display_server_process(void);
DLL_API void asmt_set_width_height(unsigned long width_in, unsigned long height_in);

#undef DLL_API

#endif
