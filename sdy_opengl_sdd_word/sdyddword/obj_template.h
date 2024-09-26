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
#define MAX_HEADING_LEVEL 7

/* --------------------------- ���Ͷ��� -----------------------------*/
typedef enum
{
  TEMPLATE_REQ_STATEMENT,
  TEMPLATE_COMMENT,
  TEMPLATE_LIST_ITEM,
  TEMPLATE_HIERARCHY_FIGURE,
  TEMPLATE_FIGURE_PLACE,
  TEMPLATE_TABLE_TITLE,
  TEMPLATE_FIGURE_TITLE,
  MAX_TEMPLATE_PARAGRAPH_ID
} template_paragraph_id_t;

typedef enum
{
  TEMPLATE_TABLE_UNKNOWN,
  TEMPLATE_TABLE_COMMON_TYPE,
  TEMPLATE_TABLE_VARIABLE,
  TEMPLATE_TABLE_MASK,

  TEMPLATE_TABLE_LINE,
  TEMPLATE_TABLE_CONTAINER,
  TEMPLATE_TABLE_TEXT,
  TEMPLATE_TABLE_RECTANGLE,
  TEMPLATE_TABLE_REFERENCECONTAINER,
  TEMPLATE_TABLE_BIFONT,
  TEMPLATE_TABLE_CIRCLE,
  TEMPLATE_TABLE_PANELCONTAINER,
  TEMPLATE_TABLE_SHAPE,
  TEMPLATE_TABLE_ROTATIONCONTAINER,
  TEMPLATE_TABLE_TRANSLATIONCONTAINER,

  TEMPLATE_TABLE_real,//above have real template

  TEMPLATE_TABLE_LAYER,
  TEMPLATE_TABLE_CONDCONTAINER,
  TEMPLATE_TABLE_INTERFACE_PARAMETERS,
  TEMPLATE_TABLE_STENCIL,
  TEMPLATE_TABLE_CLIPPLANE,
  TEMPLATE_TABLE_CLIPBOX,
  MAX_TEMPLATE_TABLE_ID
} template_table_id_t;

typedef struct
{
  template_table_id_t tableId;
  CString tableType;
  INT rowCount;
} template_table_info_t;

/* --------------------------- �������� -----------------------------*/
extern CRange tmpParagraphRange[MAX_TEMPLATE_PARAGRAPH_ID];
extern CRange tmpParagraphRange_REQ_STATEMENT;
extern CRange tmpParagraphRange_COMMENT;
extern CRange tmpParagraphRange_LIST_ITEM;
extern CRange tmpParagraphRange_HIERARCHY_FIGURE;
extern CRange tmpParagraphRange_FIGURE_PLACE;
extern CRange tmpParagraphRange_TABLE_TITLE;
extern CRange tmpParagraphRange_FIGURE_TITLE;
extern VARIANT tmpParagraphStyle[MAX_TEMPLATE_PARAGRAPH_ID];
extern VARIANT tmpParagraphStyle_REQ_STATEMENT;
extern VARIANT tmpParagraphStyle_COMMENT;
extern VARIANT tmpParagraphStyle_LIST_ITEM;
extern VARIANT tmpParagraphStyle_HIERARCHY_FIGURE;
extern VARIANT tmpParagraphStyle_FIGURE_PLACE;
extern VARIANT tmpParagraphStyle_TABLE_TITLE;
extern VARIANT tmpParagraphStyle_FIGURE_TITLE;

extern CRange tmpHeadingRange[MAX_HEADING_LEVEL+1];
extern VARIANT tmpHeadingStyle[MAX_HEADING_LEVEL+1];

extern CRange tmpTableRange[MAX_TEMPLATE_TABLE_ID];
//����Ҫ extern VARIANT tmpTableStyle[MAX_TEMPLATE_TABLE_ID];

extern CString reqNumerFormat;
extern CString commentHeadFormat;

/* --------------------------- �������� -----------------------------*/
void init_obj_template();
void close_obj_template();
template_table_id_t lookup_template_table_id(CString tableType, int rowCount);
