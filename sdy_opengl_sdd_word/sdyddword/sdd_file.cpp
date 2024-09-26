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
/*
名称	值	说明
pbLineSpacing1pt5	1	将段落行距设置为 1.5 倍行距。
pbLineSpacingDouble	2	将段落行距设置为 2 倍行距。
pbLineSpacingExactly	4	将段落行距设置为完全，则LineSpacing属性值，即使在段落中使用较大字体。
pbLineSpacingMixed	-9999999	行距值不固定的段落的返回值。
pbLineSpacingMultiple	5	必须指定一个LineSpacing属性值中的行数。
pbLineSpacingSingle	0	将段落行距设置为单倍行距。
*/
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
static CHyperlinks SDDCHyperlinks;
static CHyperlink SDDCHyperlink;
static VARIANT vRange;
static int sddNumber;
static COleVariant covOp;

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
  SDDCHyperlinks = SDDoc.get_Hyperlinks();
  vRange.vt = VT_DISPATCH;

  covOp = new COleVariant((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
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
static void compose_common_paragraph(
  CRange &tmpRange,
  VARIANT &tmpStyle,
  LPCTSTR text)
{
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  SDDRange.InsertParagraphAfter();
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  tmpRange.Copy();
  SDDRange.Paste();
  SDDRange.put_Text(text);
  SDDRange.put_Style(&tmpStyle);//必需
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_requirement_paragraph(LPCTSTR reqStatement)
{
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  SDDRange.InsertParagraphAfter();
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  tmpParagraphRange_REQ_STATEMENT.Copy();
  SDDRange.Paste();

  CString newReq;
  newReq.Format(reqNumerFormat, ++sddNumber);
  newReq.Append(reqStatement);
  SDDRange.put_Text(newReq);
  SDDRange.put_Style(&tmpParagraphStyle_REQ_STATEMENT);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_comment_paragraph(LPCTSTR commentText)
{
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  SDDRange.InsertParagraphAfter();
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();
  tmpParagraphRange_COMMENT.Copy();
  SDDRange.Paste();
  SDDRange.put_Text(commentHeadFormat + commentText);
  SDDRange.put_Style(&tmpParagraphStyle_COMMENT);
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
  tmpParagraphRange_FIGURE_TITLE.Copy();
  SDDRange.Paste();
  SDDRange.put_Style(&tmpParagraphStyle_FIGURE_TITLE);
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
  wordSelection.EndKey(COleVariant((short)6),COleVariant(short(0)));  //定位到全文末尾
  compose_common_paragraph(
    tmpParagraphRange_FIGURE_PLACE,
    tmpParagraphStyle_FIGURE_PLACE,
    _T(""));

  TCHAR fullFigureFileName[MAX_PATH * 2];
  _stprintf(fullFigureFileName, _T("%s\\%s"), xmlDirName, figureFileName);
  vRange.pdispVal = (IDispatch *)wordSelection.get_Range();

  DWORD dwAttrib = GetFileAttributes(fullFigureFileName);
  if (INVALID_FILE_ATTRIBUTES != dwAttrib)
  {
    //SDDinLineShape = SDDinLineShapes.AddPicture(fullFigureFileName, &vFalse, &vTrue, &vRange);//图片与文档一起保存
//TBD    SDDinLineShape = SDDinLineShapes.AddPicture(fullFigureFileName, &vTrue, &vFalse, &vRange);//与上面速度相同
    //SDDinLineShape = SDDinLineShapes.AddPicture(fullFigureFileName, &vFalse, &vFalse, &vRange);异常
  }

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
  _tprintf(_T("%s\n"), heading);

  compose_common_paragraph(
    tmpHeadingRange[level],
    tmpHeadingStyle[level],
    heading
    );

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
void compose_hierarchy_figure(CString listItem)
{
  //float lineUnitBefore = SDDParagraphs.get_LineUnitBefore(); //0
  //float lineUnitAfter = SDDParagraphs.get_LineUnitAfter(); //9999999.0
  //float LineSpacing = SDDParagraphs.get_LineSpacing();//9999999.0
  //SDDParagraphs.put_LineSpacing(1.0f);//效果是多倍行距 还是在word中新增样式
  compose_common_paragraph(
    tmpParagraphRange_HIERARCHY_FIGURE,
    tmpParagraphStyle_HIERARCHY_FIGURE,
    listItem);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_item_list(CXMLDOMElement &previousElement)
{
  CXMLDOMElement itemListFatherElement;
  itemListFatherElement.AttachDispatch(previousElement.get_nextSibling());

  CXMLDOMElement listItem;
  listItem.AttachDispatch(itemListFatherElement.get_firstChild());
  int i = 1;
  while (1)
  {
    CString txt;
    txt.Format(_T("%d) %s"), i++, listItem.get_text());
    compose_common_paragraph(
      tmpParagraphRange_LIST_ITEM,
      tmpParagraphStyle_LIST_ITEM,
      txt);

    LPDISPATCH p = listItem.get_nextSibling();
    if (!p) break;
    listItem.AttachDispatch(p);
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
  tmpParagraphRange_TABLE_TITLE.Copy();
  SDDRange.Paste();
  SDDRange.put_Style(&tmpParagraphStyle_TABLE_TITLE);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
static void prepare_requirement_table(int templateTableId)
{
  SDDParagraph.AttachDispatch(SDDParagraphs.get_Last());
  SDDRange = SDDParagraph.get_Range();

  tmpTableRange[templateTableId].Copy();
  SDDRange.Paste();

  long count = SDDTables.get_Count();
  SDDTable.AttachDispatch(SDDTables.Item(count));
  SDDRows.AttachDispatch(SDDTable.get_Rows());
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_variable_table(LPCTSTR reqStatement, CXMLDOMElement &rowElement)
{
  compose_requirement_paragraph(reqStatement);
  compose_table_title();
  prepare_requirement_table(TEMPLATE_TABLE_VARIABLE);

  int rowNumber = 2;
  while (1)
  {
    CXMLDOMAttribute cellBAttr;
    cellBAttr.AttachDispatch(rowElement.getAttributeNode(_T("cell_b")));
    BSTR cellBVal = cellBAttr.get_nodeValue().bstrVal;
    if (cellBVal != NULL)
    {
      CCell c = SDDTable.Cell(rowNumber, 2);
      SDDRange = c.get_Range();
      SDDRange.put_Text(cellBVal);
    }

    LPDISPATCH p = rowElement.get_nextSibling();
    if (!p) break;
    rowElement.AttachDispatch(p);
    rowNumber++;
    //变量需求表不需要增加新的空白行
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_object_requirement_table(
  LPCTSTR typeName,
  LPCTSTR column1Name,
  int rowCount,
  CXMLDOMElement &rowElement)
{
  compose_table_title();

  template_table_id_t tableId = lookup_template_table_id(typeName, rowCount);
  if (tableId == TEMPLATE_TABLE_UNKNOWN)
  {
    tableId = TEMPLATE_TABLE_COMMON_TYPE;
  }
  prepare_requirement_table(tableId);

  int rowNumber = 3;
  while (1)
  {
    if (tableId == TEMPLATE_TABLE_COMMON_TYPE)
    {
      CXMLDOMAttribute cellAAttr;
      cellAAttr.AttachDispatch(rowElement.getAttributeNode(_T("cell_a")));
      BSTR cellAVal = cellAAttr.get_nodeValue().bstrVal;
      if (cellAVal != NULL)
      {
        CCell c = SDDTable.Cell(rowNumber, 1);
        SDDRange = c.get_Range();
        SDDRange.put_Text(cellAVal);
      }
    }

    CXMLDOMAttribute cellBAttr;
    cellBAttr.AttachDispatch(rowElement.getAttributeNode(_T("cell_b")));
    BSTR cellBVal = cellBAttr.get_nodeValue().bstrVal;
    if (cellBVal != NULL)
    {
      CCell c = SDDTable.Cell(rowNumber, 2);
      SDDRange = c.get_Range();
      SDDRange.put_Text(cellBVal);
    }

    CXMLDOMAttribute cellCAttr;
    cellCAttr.AttachDispatch(rowElement.getAttributeNode(_T("cell_c")));
    BSTR cellCVal = cellCAttr.get_nodeValue().bstrVal;
    if (cellCVal != NULL)
    {
      CCell c = SDDTable.Cell(rowNumber, 3);
      SDDRange = c.get_Range();
      SDDRange.put_Text(cellCVal);
    }

    LPDISPATCH p = rowElement.get_nextSibling();
    if (!p) break;
    rowElement.AttachDispatch(p);
    rowNumber++;

    if (tableId == TEMPLATE_TABLE_COMMON_TYPE)
    {
      SDDRows.Add(covOp);//增加新的空白行
    }
  }

  if (tableId == TEMPLATE_TABLE_COMMON_TYPE)
  {
    CString cellText;
    cellText.Format(_T("Type: %s"), typeName);
    CCell c = SDDTable.Cell(1, 1);
    SDDRange = c.get_Range();
    SDDRange.put_Text(cellText);

    c = SDDTable.Cell(2, 1);
    SDDRange = c.get_Range();
    SDDRange.put_Text(column1Name);
  }
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void compose_mask_table(CXMLDOMElement rowElement)
{
  compose_table_title();
  prepare_requirement_table(TEMPLATE_TABLE_MASK);

  int rowNumber = 3;
  while (1)
  {
    CXMLDOMAttribute cellAAttr;
    cellAAttr.AttachDispatch(rowElement.getAttributeNode(_T("cell_a")));
    BSTR cellAVal = cellAAttr.get_nodeValue().bstrVal;
    if (cellAVal != NULL)
    {
      CCell c = SDDTable.Cell(rowNumber, 1);
      SDDRange = c.get_Range();
      SDDRange.put_Text(cellAVal);
    }

    CXMLDOMAttribute cellBAttr;
    cellBAttr.AttachDispatch(rowElement.getAttributeNode(_T("cell_b")));
    BSTR cellBVal = cellBAttr.get_nodeValue().bstrVal;
    if (cellBVal != NULL)
    {
      CCell c = SDDTable.Cell(rowNumber, 2);
      SDDRange = c.get_Range();
      SDDRange.put_Text(cellBVal);
    }

    LPDISPATCH p = rowElement.get_nextSibling();
    if (!p) break;
    rowElement.AttachDispatch(p);
    rowNumber++;

    SDDRows.Add(covOp);
  }
}
