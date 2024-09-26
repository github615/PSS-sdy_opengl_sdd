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
#include "COMUTIL.H"

/* --------------------------- 宏定义 -------------------------------*/
#define wdPageBreak 7
#define TABLE_SHAPE_DEFAULT_VALUE_COUNT 20

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
static TCHAR xmlDirName[MAX_PATH * 2];
static CDocument0 SDDoc;
static CnlineShapes SDDinLineShapes;
static CnlineShape SDDinLineShape;
static CParagraphs SDDParagraphs;
static CParagraph SDDParagraph;
static CTables0 SDDTables;
static CTable0 SDDTable;
static CColumns0 SDDColumns;
static CRows SDDRows;
static CRange SDDRange;
static VARIANT vRange;

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void init_sdd_file(TCHAR *xmlFileName)
{
  _tcscpy(xmlDirName, xmlFileName);
  PathRemoveFileSpec(xmlDirName);

  open_word_file(SDDoc, sddFileName, FALSE);
  SDDParagraphs = SDDoc.get_Paragraphs();
  SDDTables = SDDoc.get_Tables();
  SDDinLineShapes = SDDoc.get_InlineShapes();
  vRange.vt = VT_DISPATCH;
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void close_sdd_file()
{
  SDDParagraph.ReleaseDispatch();
  SDDParagraphs.ReleaseDispatch();
  close_word_file(SDDoc, true);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void compose_paragraph(CRange &tmpRange, LPCTSTR newText)
{
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  SDDRange.InsertParagraphAfter();
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  tmpRange.Copy();
  VARIANT style = tmpRange.get_Style();
  SDDRange.Paste();
  SDDRange.put_Text(newText);
  SDDRange.put_Style(&style);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_figure_number()
{
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  SDDRange.InsertParagraphAfter();
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  tmpParagraphRange[TEMPLATE_FIGURE_TITLE].Copy();
  VARIANT style = tmpParagraphRange[TEMPLATE_FIGURE_TITLE].get_Style();
  SDDRange.Paste();
  SDDRange.put_Style(&style);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_figure_with_number(LPCTSTR figureFileName)
{
  //wordSelection.InsertBreak(COleVariant((short)wdPageBreak));

  wordSelection.EndKey(COleVariant((short)6),COleVariant(short(0)));  //定位到全文末尾
  compose_paragraph(tmpParagraphRange[TEMPLATE_FIGURE_PLACE], _T(""));

  TCHAR fullFigureFileName[MAX_PATH * 2];
  _stprintf(fullFigureFileName, _T("%s\\%s"), xmlDirName, figureFileName);
  vRange.pdispVal = (IDispatch *)wordSelection.get_Range();
  SDDinLineShape = SDDinLineShapes.AddPicture(fullFigureFileName, &vFalse, &vTrue, &vRange);

  compose_figure_number();
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_section_heading(LPCTSTR heading, int level, bool pageBreak)
{
  compose_paragraph(TmpHeadingRange[level], heading);
  if (pageBreak)
  {
    SDDRange.InsertBreak(COleVariant((short)wdPageBreak));
    SDDRange.put_Text(heading);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_requirement(CString reqStatement)
{
  compose_paragraph(tmpParagraphRange[TEMPLATE_REQ_STATEMENT], reqStatement);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_list_item(CString listItem)
{
  compose_paragraph(tmpParagraphRange[TEMPLATE_LIST_ITEM], listItem);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_requirement_with_list(CString reqStatement, vector<CString> &listItemVector)
{
  compose_paragraph(tmpParagraphRange[TEMPLATE_REQ_STATEMENT], reqStatement);

  vector<CString>::iterator it;
  int i;
  for (i = 1, it = listItemVector.begin(); it != listItemVector.end(); i++, it++)
  {
    CString txt;
    txt.Format(_T("(%d) %s"), i, *it);
    compose_paragraph(tmpParagraphRange[TEMPLATE_LIST_ITEM], txt);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void compose_table_title()
{
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  SDDRange.InsertParagraphAfter();
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  tmpParagraphRange[TEMPLATE_TABLE_TITLE].Copy();
  VARIANT style = tmpParagraphRange[TEMPLATE_TABLE_TITLE].get_Style();
  SDDRange.Paste();
  SDDRange.put_Style(&style);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void expand_requirement_table(int TemplateTableId, int rowNumberAdd)
{
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  SDDRange.InsertParagraphAfter();
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();

  TmpTableRange[TemplateTableId].Copy();
  VARIANT style = TmpTableRange[TemplateTableId].get_Style();
  SDDRange.Paste();

  long count = SDDTables.get_Count();
  SDDTable.AttachDispatch(SDDTables.Item(count));
  SDDRows.AttachDispatch(SDDTable.get_Rows());
  COleVariant covOp((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
  for (int i = 0; i < rowNumberAdd; i++)
  {
    SDDRows.Add(covOp);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void compose_requirement_table(int TemplateTableId)
{
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  SDDRange.InsertParagraphAfter();
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();

  TmpTableRange[TemplateTableId].Copy();
  VARIANT style = TmpTableRange[TemplateTableId].get_Style();
  SDDRange.Paste();

  long count = SDDTables.get_Count();
  SDDTable.AttachDispatch(SDDTables.Item(count));
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void fill_table_data(int columnId, vector<CString> &data)
{
  int i, j;
  int dataNumber = data.size(); 
  for (i = 0, j = 3; i < dataNumber; i++, j++)
  {
    if (data[i] != _T(""))
    {
      CCell c = SDDTable.Cell(j, columnId);
      SDDRange = c.get_Range();
      SDDRange.put_Text(data[i]);
    }
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_variable_table(LPCTSTR reqStatement, vector<CString> &value)
{
  compose_paragraph(tmpParagraphRange[TEMPLATE_REQ_STATEMENT], reqStatement);
  compose_table_title();
  compose_requirement_table(TEMPLATE_TABLE_VARIABLE);

  int i, j;
  int dataNumber = value.size(); 
  for (i = 0, j = 2; i < dataNumber; i++, j++)
  {
    CCell c = SDDTable.Cell(j, 2);
    SDDRange = c.get_Range();
    SDDRange.put_Text(value[i]);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_requirement_with_known_table(
  template_table_id_t tabelId,
  LPCTSTR reqStatement,
  vector<CString> &value,
  vector<CString> &plug)
{
  compose_paragraph(tmpParagraphRange[TEMPLATE_REQ_STATEMENT], reqStatement);
  compose_table_title();
  compose_requirement_table(tabelId);
  fill_table_data(2, value);
  fill_table_data(3, plug);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_requirement_with_unknown_table(
  LPCTSTR reqStatement,
  LPCTSTR typeName,
  vector<CString> &property,
  vector<CString> &value,
  vector<CString> &plug)
{
  compose_paragraph(tmpParagraphRange[TEMPLATE_TABLE_UNKNOWN_TYPE], reqStatement);
  compose_table_title();

  int valueCount = value.size();
  expand_requirement_table(TEMPLATE_TABLE_INTERFACE_PARAMETERS, value.size() - 1);
  fill_table_data(1, property);
  fill_table_data(2, value);
  fill_table_data(3, plug);

  CString cellText;
  cellText.Format(_T("Type: %s"), typeName);
  CCell c = SDDTable.Cell(1, 1);
  SDDRange = c.get_Range();
  SDDRange.put_Text(cellText);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_requirement_with_interface_parameters_table(
  LPCTSTR reqStatement,
  vector<CString> &property,
  vector<CString> &value,
  vector<CString> &plug)
{
  compose_paragraph(tmpParagraphRange[TEMPLATE_REQ_STATEMENT], reqStatement);
  compose_table_title();

  int valueCount = value.size();
  expand_requirement_table(TEMPLATE_TABLE_INTERFACE_PARAMETERS, value.size() - 1);
  fill_table_data(1, property);
  fill_table_data(2, value);
  fill_table_data(3, plug);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_requirement_with_shape_table(
  LPCTSTR reqStatement,
  vector<CString> &property,
  vector<CString> &value,
  vector<CString> &plug)
{
  compose_paragraph(tmpParagraphRange[TEMPLATE_REQ_STATEMENT], reqStatement);
  compose_table_title();

  int valueCount = value.size();
  if (valueCount < TABLE_SHAPE_DEFAULT_VALUE_COUNT)
  {
    dump_log_file(_T("internal error: valueCount(%d) < TABLE_SHAPE_DEFAULT_VALUE_COUNT\n"),
      valueCount);
  }
  else
  {
    expand_requirement_table(TEMPLATE_TABLE_SHAPE, value.size() - TABLE_SHAPE_DEFAULT_VALUE_COUNT);
    fill_table_data(1, property);
    fill_table_data(2, value);
    fill_table_data(3, plug);
  }
}
