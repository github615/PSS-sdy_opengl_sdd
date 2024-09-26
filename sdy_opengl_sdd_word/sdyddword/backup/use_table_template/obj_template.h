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
#define MAX_HEADING_LEVEL 7

/* --------------------------- 类型定义 -----------------------------*/
typedef enum
{
  TEMPLATE_REQ_STATEMENT,
  TEMPLATE_LIST_ITEM,
  TEMPLATE_FIGURE_PLACE,
  TEMPLATE_TABLE_TITLE,
  TEMPLATE_FIGURE_TITLE,
  MAX_TEMPLATE_PARAGRAPH_ID
} template_paragraph_id_t;

typedef enum
{
  TEMPLATE_TABLE_UNKNOWN,
  TEMPLATE_TABLE_1,
  TEMPLATE_TABLE_UNKNOWN_TYPE,
  TEMPLATE_TABLE_VARIABLE,
  TEMPLATE_TABLE_LAYER,
  TEMPLATE_TABLE_CONTAINER,
  TEMPLATE_TABLE_RECTANGLE,
  TEMPLATE_TABLE_TEXT,
  TEMPLATE_TABLE_LINE,
  TEMPLATE_TABLE_CONDCONTAINER,
  TEMPLATE_TABLE_REFERENCECONTAINER,
  TEMPLATE_TABLE_INTERFACE_PARAMETERS,
  TEMPLATE_TABLE_STENCIL,
  TEMPLATE_TABLE_SHAPE,
  TEMPLATE_TABLE_BIFONT,
  TEMPLATE_TABLE_CIRCLE,
  TEMPLATE_TABLE_CLIPPLANE,
  TEMPLATE_TABLE_CLIPBOX,
  TEMPLATE_TABLE_TRANSLATIONCONTAINER,
  MAX_TEMPLATE_TABLE_ID
} template_table_id_t;

/* --------------------------- 变量声明 -----------------------------*/
extern CRange tmpParagraphRange[MAX_TEMPLATE_PARAGRAPH_ID];
extern CRange TmpHeadingRange[MAX_HEADING_LEVEL+1];
extern CRange TmpTableRange[MAX_TEMPLATE_TABLE_ID];

/* --------------------------- 函数声明 -----------------------------*/
void init_obj_template();
void close_obj_template();
template_table_id_t lookup_template_table_id(CString tableType);
