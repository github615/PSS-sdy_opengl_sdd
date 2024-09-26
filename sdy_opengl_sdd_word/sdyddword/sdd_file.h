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

/* --------------------------- 函数声明 -----------------------------*/
void init_sdd_file(TCHAR *xmlFileName);
void close_sdd_file();
void compose_figure_number();
void compose_figure_with_number(LPCTSTR figureFileName);
void compose_section_heading(LPCTSTR heading, int level, bool pageBreak);
void compose_item_list(CXMLDOMElement &previousElement);
void compose_requirement_paragraph(LPCTSTR reqStatement);
void compose_comment_paragraph(LPCTSTR commentText);
void compose_hierarchy_figure(CString listItem);
void compose_variable_table(LPCTSTR reqStatement, CXMLDOMElement &rowElement);
void compose_object_requirement_table(
  LPCTSTR typeName,
  LPCTSTR column1Name,
  int rowCount,
  CXMLDOMElement &rowElement);
void compose_mask_table(CXMLDOMElement rowElement);
