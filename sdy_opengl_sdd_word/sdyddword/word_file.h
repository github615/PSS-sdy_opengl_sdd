#pragma once
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

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量声明 -----------------------------*/
extern CString sddFileName;
extern CString objTemplateFileName;
extern CSelection wordSelection;
extern COleVariant vTrue;
extern CComVariant vFalse;
extern COleVariant vOpt;

/* --------------------------- 函数声明 -----------------------------*/
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
