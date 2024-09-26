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
#define LINE_STIPPLE_BASE_SIZE 10
#define LINE_STIPPLE_LENGTH_ATTR_BASE_SIZE 10

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
line_stipple_def_t *line_stipple_table;
int max_line_stipple_index;
static int line_strpple_size;
extern resource_table_t resource_table;

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void parse_line_stipple_table(void)
{
  xmlDocPtr doc;
  xmlNodePtr curNode;
  xmlAttrPtr attrPtr;
  char *szDocName = resource_table.line_stipple_table_file;
  int index;

  line_stipple_table = (line_stipple_def_t *)get_cmem(LINE_STIPPLE_BASE_SIZE * sizeof(line_stipple_def_t));
  line_strpple_size = LINE_STIPPLE_BASE_SIZE;
  max_line_stipple_index = 0;

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

  if (xmlStrcmp(curNode->name, BAD_CAST "lineStippleTable"))
  {
    xmlFreeDoc(doc);
    fatal_error("document of the wrong type, root node != lineStippleTable\n");
  }

  curNode = curNode->xmlChildrenNode;
  while(curNode != NULL)
  {
    if ((!xmlStrcmp(curNode->name, (const xmlChar *)"lineStippleDefinition")))
    {
      xmlNodePtr sonNode = curNode->xmlChildrenNode;
      char stipple_type[ATTR_LENGTH];

      attrPtr = curNode->properties;
      while (attrPtr != NULL)
      {
        if (!xmlStrcmp(attrPtr->name, BAD_CAST "index"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "index");
          index = atoi((const char *)szAttr);
          xmlFree(szAttr);
          if (index > line_strpple_size - 1)
          {
            line_strpple_size += (line_strpple_size >> 1);
            line_stipple_table = (line_stipple_def_t *)reget_mem(line_stipple_table, line_strpple_size * sizeof(line_stipple_def_t));
          }
          line_stipple_table[index].line_stipple_length_attr_count = 0;
          line_stipple_table[index].line_stipple_length_attr_size = 0;
          line_stipple_table[index].line_stipple_length_attr_table = NULL;
          if (max_line_stipple_index < index)
            max_line_stipple_index = index;
        }
        else if (!xmlStrcmp(attrPtr->name, BAD_CAST "type"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "type");
          strcpy(stipple_type, (const char *)szAttr);
          xmlFree(szAttr);
        }
        attrPtr = attrPtr->next;
      }
      strcpy(line_stipple_table[index].stipple_type, stipple_type);

      while (sonNode != NULL)
      {
        if ((!xmlStrcmp(sonNode->name, (const xmlChar *)"length")))
        {
          line_stipple_length_attr_t length_attr;

          attrPtr = sonNode->properties;
          while (attrPtr != NULL)
          {
            if (!xmlStrcmp(attrPtr->name, BAD_CAST "visible"))
            {
              xmlChar* szAttr = xmlGetProp(sonNode, BAD_CAST "visible");
              strcpy(length_attr.visible, (const char *)szAttr);
              xmlFree(szAttr);
            }
            else if (!xmlStrcmp(attrPtr->name, BAD_CAST "value"))
            {
              xmlChar* szAttr = xmlGetProp(sonNode, BAD_CAST "value");
              length_attr.value = atoi((const char *)szAttr);
              xmlFree(szAttr);
            }
            attrPtr = attrPtr->next;
          }

          adjust_mem((void **)&line_stipple_table[index].line_stipple_length_attr_table, 
            &line_stipple_table[index].line_stipple_length_attr_count, 
            &line_stipple_table[index].line_stipple_length_attr_size,
            LINE_STIPPLE_LENGTH_ATTR_BASE_SIZE, sizeof(line_stipple_length_attr_t));
          line_stipple_table[index].line_stipple_length_attr_table[line_stipple_table[index].line_stipple_length_attr_count++] = length_attr;
        }
        sonNode = sonNode->next;
      }
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
void free_line_stipple_table(void)
{
  int i;
  line_stipple_def_t *line_stipple = line_stipple_table;
  for (i = 0; i <= max_line_stipple_index; i++, line_stipple++)
  {
    if (line_stipple->line_stipple_length_attr_table)
      free_mem(line_stipple->line_stipple_length_attr_table);
  }
  free_mem(line_stipple_table);
}
