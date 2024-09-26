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

/* --------------------------- �������� -----------------------------*/
void init_sdd_file(TCHAR *xmlFileName);
void close_sdd_file();
void compose_figure_number();
void compose_figure_with_number(LPCTSTR figureFileName);
void compose_section_heading(LPCTSTR heading, int level, bool pageBreak);
void compose_requirement_with_list(CString reqStatement, vector<CString> &listItemVector);
void compose_requirement(CString reqStatement);
void compose_list_item(CString listItem);
void compose_variable_table(LPCTSTR reqStatement, vector<CString> &value);
void compose_requirement_with_known_table(
  template_table_id_t tabelId,
  LPCTSTR reqStatement,
  vector<CString> &value,
  vector<CString> &plug);
void compose_requirement_with_unknown_table(
  LPCTSTR reqStatement,
  LPCTSTR typeName,
  vector<CString> &property,
  vector<CString> &value,
  vector<CString> &plug);
void compose_requirement_with_interface_parameters_table(
  LPCTSTR reqStatement,
  vector<CString> &property,
  vector<CString> &value,
  vector<CString> &plug);
void compose_requirement_with_shape_table(
  LPCTSTR reqStatement,
  vector<CString> &property,
  vector<CString> &value,
  vector<CString> &plug);
