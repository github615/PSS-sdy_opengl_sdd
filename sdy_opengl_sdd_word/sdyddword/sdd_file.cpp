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
#include "stdafx.h"
#include "COMUTIL.H"

/* --------------------------- �궨�� -------------------------------*/
/*
����	ֵ	˵��
pbLineSpacing1pt5	1	�������о�����Ϊ 1.5 ���оࡣ
pbLineSpacingDouble	2	�������о�����Ϊ 2 ���оࡣ
pbLineSpacingExactly	4	�������о�����Ϊ��ȫ����LineSpacing����ֵ����ʹ�ڶ�����ʹ�ýϴ����塣
pbLineSpacingMixed	-9999999	�о�ֵ���̶��Ķ���ķ���ֵ��
pbLineSpacingMultiple	5	����ָ��һ��LineSpacing����ֵ�е�������
pbLineSpacingSingle	0	�������о�����Ϊ�����оࡣ
*/
#define wdPageBreak 7
#define TABLE_SHAPE_DEFAULT_VALUE_COUNT 20

/* --------------------------- ���Ͷ��� -----------------------------*/

/* --------------------------- �������� -----------------------------*/
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

/* --------------------------- �������� -----------------------------*/
/**
 **********************************************************************
 *
 * @��������
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
 * @��������
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
 * @��������
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
  SDDRange.put_Style(&tmpStyle);//����
}
/**
 **********************************************************************
 *
 * @��������
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
 * @��������
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
 * @��������
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
 * @��������
 *
 **********************************************************************
*/
void compose_figure_with_number(LPCTSTR figureFileName)
{
  wordSelection.EndKey(COleVariant((short)6),COleVariant(short(0)));  //��λ��ȫ��ĩβ
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
    //SDDinLineShape = SDDinLineShapes.AddPicture(fullFigureFileName, &vFalse, &vTrue, &vRange);//ͼƬ���ĵ�һ�𱣴�
//TBD    SDDinLineShape = SDDinLineShapes.AddPicture(fullFigureFileName, &vTrue, &vFalse, &vRange);//�������ٶ���ͬ
    //SDDinLineShape = SDDinLineShapes.AddPicture(fullFigureFileName, &vFalse, &vFalse, &vRange);�쳣
  }

  compose_figure_number();
}
/**
 **********************************************************************
 *
 * @��������
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
 * @��������
 *
 **********************************************************************
*/
void compose_hierarchy_figure(CString listItem)
{
  //float lineUnitBefore = SDDParagraphs.get_LineUnitBefore(); //0
  //float lineUnitAfter = SDDParagraphs.get_LineUnitAfter(); //9999999.0
  //float LineSpacing = SDDParagraphs.get_LineSpacing();//9999999.0
  //SDDParagraphs.put_LineSpacing(1.0f);//Ч���Ƕ౶�о� ������word��������ʽ
  compose_common_paragraph(
    tmpParagraphRange_HIERARCHY_FIGURE,
    tmpParagraphStyle_HIERARCHY_FIGURE,
    listItem);
}
/**
 **********************************************************************
 *
 * @��������
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
 * @��������
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
 * @��������
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
 * @��������
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
    //�����������Ҫ�����µĿհ���
  }
}
/**
 **********************************************************************
 *
 * @��������
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
      SDDRows.Add(covOp);//�����µĿհ���
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
 * @��������
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
