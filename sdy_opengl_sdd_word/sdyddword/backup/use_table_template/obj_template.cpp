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
typedef struct  
{
  template_table_id_t tableId;
  CString tableType;
} template_table_info_t;

/* --------------------------- 变量定义 -----------------------------*/
static template_table_info_t templateTableInfoList[] =
{
  {TEMPLATE_TABLE_LAYER, _T("layer")},
  {TEMPLATE_TABLE_CONTAINER, _T("container")},
  {TEMPLATE_TABLE_RECTANGLE, _T("rectangle")},
  {TEMPLATE_TABLE_TEXT, _T("text")},
  {TEMPLATE_TABLE_LINE, _T("line")},
  {TEMPLATE_TABLE_CONDCONTAINER, _T("condContainer")},
  {TEMPLATE_TABLE_REFERENCECONTAINER, _T("referenceContainer")},
  {TEMPLATE_TABLE_INTERFACE_PARAMETERS, _T("interface_parameters")},
  {TEMPLATE_TABLE_STENCIL, _T("stencil")},
  {TEMPLATE_TABLE_SHAPE, _T("shape")},
  {TEMPLATE_TABLE_BIFONT, _T("biFont")},
  {TEMPLATE_TABLE_CIRCLE, _T("circle")},
  {TEMPLATE_TABLE_CLIPPLANE, _T("clipPlane")},
  {TEMPLATE_TABLE_CLIPBOX, _T("clipBox")},
  {TEMPLATE_TABLE_TRANSLATIONCONTAINER, _T("translationContainer")},
  {TEMPLATE_TABLE_UNKNOWN, _T("")}
};
static CDocument0 TmpDoc;
static CParagraphs TmpParagraphs;
static CParagraph TmpParagraph;
static CTables0 TmpTables;
static CTable0 TmpTable;
CRange tmpParagraphRange[MAX_TEMPLATE_PARAGRAPH_ID];
CRange TmpHeadingRange[MAX_HEADING_LEVEL+1];
CRange TmpTableRange[MAX_TEMPLATE_TABLE_ID];

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

  open_word_file(TmpDoc, objTemplateFileName, TRUE);
  TmpParagraphs = TmpDoc.get_Paragraphs();

  i = 1;
  for (j = 0; j < MAX_TEMPLATE_PARAGRAPH_ID; j++, i++)
  {
    TmpParagraph.AttachDispatch(TmpParagraphs.Item(i));
    tmpParagraphRange[j] = TmpParagraph.get_Range();
  }

  for (j = 1; j <= MAX_HEADING_LEVEL; j++, i++)
  {
    TmpParagraph.AttachDispatch(TmpParagraphs.Item(i));
    TmpHeadingRange[j] = TmpParagraph.get_Range();
    txt = TmpHeadingRange[j].get_Text();
  }

  TmpTables = TmpDoc.get_Tables();
  for (j = 1; j < MAX_TEMPLATE_TABLE_ID; j++)
  {
    TmpTable.AttachDispatch(TmpTables.Item(j));
    TmpTableRange[j] = TmpTable.get_Range();
  }
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
    TmpHeadingRange[j].ReleaseDispatch();
  }
  for (j = 1; j < MAX_TEMPLATE_TABLE_ID; j++)
  {
    TmpTableRange[j].ReleaseDispatch();
  }
  TmpTable.ReleaseDispatch();
  TmpTables.ReleaseDispatch();
  TmpParagraph.ReleaseDispatch();
  TmpParagraphs.ReleaseDispatch();
  TmpDoc.ReleaseDispatch();
  close_word_file(TmpDoc, false);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
template_table_id_t lookup_template_table_id(CString tableType)
{
  template_table_info_t *p = templateTableInfoList;
  while (p->tableId != TEMPLATE_TABLE_UNKNOWN)
  {
    if (p->tableType == tableType)
    {
      return p->tableId;
    }
    p++;
  }
  dump_log_file(_T("Unknown tableType %s\n"), tableType);
  return TEMPLATE_TABLE_UNKNOWN;
}