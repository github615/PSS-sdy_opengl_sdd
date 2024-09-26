/* --------------------------- 序言 ---------------------------------*/

/**
 **********************************************************************
 *
 * (C) 2016 上海爱韦讯信息技术有限公司。
 *
 * @功能描述
 *
 **********************************************************************
*/
/* --------------------------- 文件包含 -----------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libxml/parser.h"
#include "libxml/tree.h"
#include "sdy_config.h"
#include "sdy_source.h"
#include "scade_display_dir.h"

/* --------------------------- 宏定义 -------------------------------*/
#define LINE_WIDTH_TABLE_BASE_SIZE 10

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
line_width_t *line_width_table;
int max_line_width_index;
static int line_width_size;
extern resource_table_t resource_table;

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void parse_line_width_table(void)
{
  xmlDocPtr doc;
  xmlNodePtr curNode;
  xmlAttrPtr attrPtr;
  char *szDocName = resource_table.line_width_table_file;
  int index = 0;
  line_width_t temp_line_width;

  line_width_table = (line_width_t *)get_cmem(LINE_WIDTH_TABLE_BASE_SIZE * sizeof(line_width_t));
  line_width_size = LINE_WIDTH_TABLE_BASE_SIZE;
  max_line_width_index = 0;

  doc = xmlReadFile(szDocName,"UTF-8",XML_PARSE_RECOVER);
  if (NULL == doc)
  {  
    fatal_error("Can't open %s\n", szDocName);    
  }

  curNode = xmlDocGetRootElement(doc);
  if (NULL == curNode)
  {
    xmlFreeDoc(doc);
    fatal_error("%s is empty document\n", szDocName);
  }

  if (xmlStrcmp(curNode->name, BAD_CAST "lineWidthTable"))
  {
    xmlFreeDoc(doc);
    fatal_error("document of the wrong type, root node != lineWidthTable\n");
  }

  curNode = curNode->xmlChildrenNode;
  while(curNode != NULL)
  {
    if ((!xmlStrcmp(curNode->name, (const xmlChar *)"lineWidthDefinition")))
    {
      attrPtr = curNode->properties;
      while (attrPtr != NULL)
      {
        if (!xmlStrcmp(attrPtr->name, BAD_CAST "index"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "index");
          index = atoi((const char *)szAttr);
          xmlFree(szAttr);
          if (index > line_width_size - 1)
          {
            line_width_size += (line_width_size >> 1);
            line_width_table = (line_width_t *)reget_mem(line_width_table, line_width_size * sizeof(line_width_t));
          }
          if (max_line_width_index < index)
            max_line_width_index = index;
        }
        else if (!xmlStrcmp(attrPtr->name, BAD_CAST "name"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "name");
          strcpy(temp_line_width.name, (const char *)szAttr);
          xmlFree(szAttr);
        }
        else if (!xmlStrcmp(attrPtr->name, BAD_CAST "value"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "value");
          strcpy(temp_line_width.value, (const char *)szAttr);
          xmlFree(szAttr);
        }
        else if (!xmlStrcmp(attrPtr->name, BAD_CAST "colorWidth"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "colorWidth");
          strcpy(temp_line_width.color_width, (const char *)szAttr);
          xmlFree(szAttr);
        }
        else if (!xmlStrcmp(attrPtr->name, BAD_CAST "haloWidth"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "haloWidth");
          strcpy(temp_line_width.halo_width, (const char *)szAttr);
          xmlFree(szAttr);
        }
        else if (!xmlStrcmp(attrPtr->name, BAD_CAST "width"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "width");
          strcpy(temp_line_width.width, (const char *)szAttr);
          xmlFree(szAttr);
        }
        attrPtr = attrPtr->next;
      }
      sprintf(temp_line_width.ensemble, "{%s, %s, %s} [#%d]", 
        temp_line_width.width, temp_line_width.color_width, temp_line_width.halo_width, index);
      line_width_table[index] = temp_line_width;
    }
    curNode = curNode->next;
  }
  xmlFreeDoc(doc);
}
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void free_line_width_table(void)
{
  free_mem(line_width_table);
}