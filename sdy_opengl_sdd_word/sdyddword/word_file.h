#pragma once
/* --------------------------- ���� ---------------------------------*/

/**
**********************************************************************
*
* (C) 2018 �Ϻ���ΤѶ��Ϣ�������޹�˾��
*
* @��������
*
**********************************************************************
*/

/* --------------------------- �ļ����� -----------------------------*/

/* --------------------------- �궨�� -------------------------------*/

/* --------------------------- ���Ͷ��� -----------------------------*/

/* --------------------------- �������� -----------------------------*/
extern CString sddFileName;
extern CString objTemplateFileName;
extern CSelection wordSelection;
extern COleVariant vTrue;
extern CComVariant vFalse;
extern COleVariant vOpt;

/* --------------------------- �������� -----------------------------*/
void init_word_file(
  TCHAR *exeFileName,
  TCHAR *outputDirName,
  TCHAR *sddFileNameIn,
  TCHAR *sddTemplateFileNameIn,
  TCHAR *objTemplateFileNameIn);
void init_word_app();
void open_word_file(CDocument0 &doc, CString file_name, bool readOnly);
void close_word_file(CDocument0 &doc, bool saveIt);
void release_word_app();
