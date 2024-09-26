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
#include "stdafx.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
CString sddFileName;
CString objTemplateFileName;
CSelection wordSelection;

static CApplication app;
static CDocuments docs;

COleVariant vTrue((short)TRUE);
CComVariant vFalse((short)FALSE);
COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void init_word_file(
  TCHAR *exeFileName,
  TCHAR *outputDirName,
  TCHAR *sddFileNameIn,
  TCHAR *sddTemplateFileNameIn,
  TCHAR *objTemplateFileNameIn)
{
  TCHAR distribDirName[MAX_PATH * 2];
  TCHAR *p;
  DWORD dwAttrib;

  _tcscpy(distribDirName, exeFileName);
  PathRemoveFileSpec(distribDirName);
  p =_tcsrchr(distribDirName, _T('\\'));
  if (_tcscmp(p, _T("\\bin")) == 0)
  {
    *p = 0;
  }
  else if (_tcscmp(p, _T("\\Debug")) == 0 || _tcscmp(p, _T("\\Release")) == 0)
  {
    _tcscpy(p+1, _T("distrib"));
  }
  else
  {
    fatal_error(_T("Cant find distrib directory\n"));
  }

  objTemplateFileName = CString(distribDirName) + _T("\\template\\") + objTemplateFileNameIn;
  dwAttrib = GetFileAttributes(objTemplateFileName);
  if (INVALID_FILE_ATTRIBUTES == dwAttrib)
  {
    fatal_error(_T("Cant open obj template file %s\n"), objTemplateFileName);
  }

  CString sddTemplateFileName = CString(distribDirName) + _T("\\template\\") + sddTemplateFileNameIn;

  dwAttrib = GetFileAttributes(sddTemplateFileName);
  if (INVALID_FILE_ATTRIBUTES == dwAttrib)
  {
    fatal_error(_T("Cant open sdd template file %s\n"), sddTemplateFileName);
  }

  int len = WideCharToMultiByte(CP_ACP,0,sddTemplateFileName,-1,NULL,0,NULL,NULL);  
  char *pSDDTemplate = new char[len +1];  
  WideCharToMultiByte(CP_ACP,0,sddTemplateFileName,-1,pSDDTemplate,len,NULL,NULL ); 

  sddFileName = CString(outputDirName) + _T("\\") + sddFileNameIn;
  len = WideCharToMultiByte(CP_ACP,0,sddFileName,-1,NULL,0,NULL,NULL);  
  char *pSDD = new char[len +1];  
  WideCharToMultiByte(CP_ACP,0,sddFileName,-1,pSDD,len,NULL,NULL );

  char cmd[MAX_PATH * 3];
  sprintf(cmd, "copy /Y \"%s\" \"%s\" >nul", pSDDTemplate, pSDD);
  system(cmd);

  delete[] pSDDTemplate;
  delete[] pSDD;
}
/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
void init_word_app()
{
  if (!app.CreateDispatch(_T("Word.Application"), NULL))
  {
    fatal_error(_T("Create Word service fail\n")); 
  }
#ifdef _DEBUG
  app.put_Visible(TRUE);
#else
  app.put_Visible(FALSE);
#endif
  docs = app.get_Documents();
  //useless at here!!! wordSelection = app.get_Selection();
}

/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
void open_word_file(CDocument0 &doc, CString file_name, bool readOnly)
{
  CComVariant Name(file_name);
  CComVariant Read(readOnly);
  CComVariant AddToR(FALSE);

  //0(wdOpenFormatAuto): The existing format
  //3(wdOpenFormatRTF): Rich text format(RTF) 此时template也必须是RTF
  CComVariant format(0);

  IDispatch *dispatch = docs.Open(&Name,&vFalse,&Read,&AddToR,  
    &vOpt,&vOpt,&vFalse,&vOpt,&vOpt,&format,&vOpt,  
    &vTrue,&vOpt,&vOpt,&vOpt,&vOpt);
  if (dispatch)  
  {
    doc.AttachDispatch(dispatch);
  }
  else  
  {
    fatal_error(_T("Can't open %s\n"), (LPCTSTR)file_name); 
  }
  wordSelection = app.get_Selection(); //must be at here!!!
}

/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
void close_word_file(CDocument0 &doc, bool saveIt)
{
  if (saveIt)
  {
    doc.Save();
  }
  doc.Close(&vFalse, &vOpt, &vOpt);
  doc.ReleaseDispatch();
}

/**
**********************************************************************
*
* @功能描述
*
**********************************************************************
*/
void release_word_app()
{
  app.Quit(&vOpt,&vOpt,&vOpt);
  app.ReleaseDispatch();
  docs.ReleaseDispatch();
}