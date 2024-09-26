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
static template_table_info_t templateTableInfoList[] =
{
  {TEMPLATE_TABLE_LINE, _T("line"), 12},
  {TEMPLATE_TABLE_CONTAINER, _T("container"), 10},
  {TEMPLATE_TABLE_TEXT, _T("text"), 14},
  {TEMPLATE_TABLE_RECTANGLE, _T("rectangle"), 20},
  {TEMPLATE_TABLE_REFERENCECONTAINER, _T("referenceContainer"), 8},
  {TEMPLATE_TABLE_BIFONT, _T("biFont"), 19},
  {TEMPLATE_TABLE_CIRCLE, _T("circle"), 17},
  {TEMPLATE_TABLE_PANELCONTAINER, _T("panelContainer"), 6},
  {TEMPLATE_TABLE_SHAPE, _T("shape"), 22},
  {TEMPLATE_TABLE_ROTATIONCONTAINER, _T("rotationContainer"), 13},
  {TEMPLATE_TABLE_TRANSLATIONCONTAINER, _T("translationContainer"), 15},

  {TEMPLATE_TABLE_LAYER, _T("layer"), -1},
  {TEMPLATE_TABLE_CONDCONTAINER, _T("condContainer"), -1},
  {TEMPLATE_TABLE_INTERFACE_PARAMETERS, _T("interface_parameters"), -1},
  {TEMPLATE_TABLE_STENCIL, _T("stencil"), -1},
  {TEMPLATE_TABLE_CLIPPLANE, _T("clipPlane"), -1},
  {TEMPLATE_TABLE_CLIPBOX, _T("clipBox"), -1},
  {TEMPLATE_TABLE_UNKNOWN, _T("")}
};
static CDocument0 tmpDoc;
static CParagraphs tmpParagraphs;
static CParagraph tmpParagraph;
static CTables0 tmpTables;
static CTable0 tmpTable;

CRange tmpParagraphRange[MAX_TEMPLATE_PARAGRAPH_ID];
CRange tmpParagraphRange_REQ_STATEMENT;
CRange tmpParagraphRange_COMMENT;
CRange tmpParagraphRange_LIST_ITEM;
CRange tmpParagraphRange_HIERARCHY_FIGURE;
CRange tmpParagraphRange_FIGURE_PLACE;
CRange tmpParagraphRange_TABLE_TITLE;
CRange tmpParagraphRange_FIGURE_TITLE;
VARIANT tmpParagraphStyle[MAX_TEMPLATE_PARAGRAPH_ID];
VARIANT tmpParagraphStyle_REQ_STATEMENT;
VARIANT tmpParagraphStyle_COMMENT;
VARIANT tmpParagraphStyle_LIST_ITEM;
VARIANT tmpParagraphStyle_HIERARCHY_FIGURE;
VARIANT tmpParagraphStyle_FIGURE_PLACE;
VARIANT tmpParagraphStyle_TABLE_TITLE;
VARIANT tmpParagraphStyle_FIGURE_TITLE;

CRange tmpHeadingRange[MAX_HEADING_LEVEL+1];
VARIANT tmpHeadingStyle[MAX_HEADING_LEVEL+1];

CRange tmpTableRange[MAX_TEMPLATE_TABLE_ID];
//不需要 VARIANT tmpTableStyle[MAX_TEMPLATE_TABLE_ID];

CString reqNumerFormat;
CString commentHeadFormat;

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void init_obj_template()
{
  long i, j;
  CString txt;

  open_word_file(tmpDoc, objTemplateFileName, TRUE);
  tmpParagraphs = tmpDoc.get_Paragraphs();

  i = 1;
  for (j = 0; j < MAX_TEMPLATE_PARAGRAPH_ID; j++, i++)
  {
    tmpParagraph.AttachDispatch(tmpParagraphs.Item(i));
    tmpParagraphRange[j] = tmpParagraph.get_Range();
    tmpParagraphStyle[j] = tmpParagraphRange[j].get_Style(); 
  }
  tmpParagraphRange_REQ_STATEMENT = tmpParagraphRange[TEMPLATE_REQ_STATEMENT];
  tmpParagraphRange_COMMENT = tmpParagraphRange[TEMPLATE_COMMENT];
  tmpParagraphRange_LIST_ITEM = tmpParagraphRange[TEMPLATE_LIST_ITEM];
  tmpParagraphRange_HIERARCHY_FIGURE = tmpParagraphRange[TEMPLATE_HIERARCHY_FIGURE];
  tmpParagraphRange_FIGURE_PLACE = tmpParagraphRange[TEMPLATE_FIGURE_PLACE];
  tmpParagraphRange_TABLE_TITLE = tmpParagraphRange[TEMPLATE_TABLE_TITLE];
  tmpParagraphRange_FIGURE_TITLE = tmpParagraphRange[TEMPLATE_FIGURE_TITLE];

  tmpParagraphStyle_REQ_STATEMENT = tmpParagraphStyle[TEMPLATE_REQ_STATEMENT];
  tmpParagraphStyle_COMMENT = tmpParagraphStyle[TEMPLATE_COMMENT];
  tmpParagraphStyle_LIST_ITEM = tmpParagraphStyle[TEMPLATE_LIST_ITEM];
  tmpParagraphStyle_HIERARCHY_FIGURE = tmpParagraphStyle[TEMPLATE_HIERARCHY_FIGURE];
  tmpParagraphStyle_FIGURE_PLACE = tmpParagraphStyle[TEMPLATE_FIGURE_PLACE];
  tmpParagraphStyle_TABLE_TITLE = tmpParagraphStyle[TEMPLATE_TABLE_TITLE];
  tmpParagraphStyle_FIGURE_TITLE = tmpParagraphStyle[TEMPLATE_FIGURE_TITLE];

  for (j = 1; j <= MAX_HEADING_LEVEL; j++, i++)
  {
    tmpParagraph.AttachDispatch(tmpParagraphs.Item(i));
    tmpHeadingRange[j] = tmpParagraph.get_Range();
    tmpHeadingStyle[j] = tmpHeadingRange[j].get_Style();
    txt = tmpHeadingRange[j].get_Text();
  }

  tmpTables = tmpDoc.get_Tables();
  for (j = 1; j < TEMPLATE_TABLE_real; j++)
  {
    tmpTable.AttachDispatch(tmpTables.Item(j));
    tmpTableRange[j] = tmpTable.get_Range();
    //不需要 tmpTableStyle[j] = tmpTableRange[j].get_Style();
  }

  CRange tmpRange = tmpParagraphRange[TEMPLATE_REQ_STATEMENT];
  CString temp = tmpRange.get_Text();
  int p1 = temp.Find(_T('['));
  int p2 = temp.Find(_T(']'));
  reqNumerFormat = temp.Mid(p1, p2 - p1 + 1);

  tmpRange = tmpParagraphRange[TEMPLATE_COMMENT];
  temp = tmpRange.get_Text();
  p1 = temp.Find(_T('['));
  p2 = temp.Find(_T(']'));
  commentHeadFormat = temp.Mid(p1, p2 - p1 + 1);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void close_obj_template()
{
  int j;
  for (j = 0; j < MAX_TEMPLATE_PARAGRAPH_ID; j++)
  {
    tmpParagraphRange[j].ReleaseDispatch();
  }
  for (j = 1; j <= MAX_HEADING_LEVEL; j++)
  {
    tmpHeadingRange[j].ReleaseDispatch();
  }
  for (j = 1; j < TEMPLATE_TABLE_real; j++)
  {
    tmpTableRange[j].ReleaseDispatch();
  }
  tmpTable.ReleaseDispatch();
  tmpTables.ReleaseDispatch();
  tmpParagraph.ReleaseDispatch();
  tmpParagraphs.ReleaseDispatch();
  tmpDoc.ReleaseDispatch();
  close_word_file(tmpDoc, false);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
template_table_id_t lookup_template_table_id(CString tableType, int rowCount)
{
  template_table_info_t *p = templateTableInfoList;
  while (p->tableId != TEMPLATE_TABLE_UNKNOWN)
  {
    if (p->tableType == tableType && p->rowCount == rowCount)
    {
      return p->tableId;
    }
    p++;
  }
  dump_log_file(_T("Unknown tableType %s %d\n"), tableType, rowCount);
  return TEMPLATE_TABLE_UNKNOWN;
}