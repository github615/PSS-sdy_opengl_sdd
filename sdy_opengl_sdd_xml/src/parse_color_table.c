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
#include "scade_display_dir.h"
#include "sdy_config.h"
#include "sdy_source.h"

/* --------------------------- 宏定义 -------------------------------*/

/* --------------------------- 类型定义 -----------------------------*/

/* --------------------------- 变量定义 -----------------------------*/
color_def_t *color_table;
int max_color_index;
extern resource_table_t resource_table;

/* --------------------------- 函数定义 -----------------------------*/
/**
 **********************************************************************
 *
 * @功能描述
 *
 **********************************************************************
*/
void parse_color_table(void)
{
  xmlDocPtr doc;
  xmlNodePtr curNode;
  xmlAttrPtr attrPtr;
  char *szDocName = resource_table.color_table_file;
  int color_table_size;
  int index;

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

  if (xmlStrcmp(curNode->name, BAD_CAST "colorTable"))
  {
    xmlFreeDoc(doc);
    fatal_error("document of the wrong type, root node != colorTable\n");
  }
  attrPtr = curNode->properties;
  while (attrPtr != NULL)
  {
    if (!xmlStrcmp(attrPtr->name, BAD_CAST "size"))
    {
      xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "size");
      color_table_size = atoi((const char *)szAttr);
      xmlFree(szAttr);
      break;
    }
    attrPtr = attrPtr->next;
  }
  color_table = (color_def_t *)get_cmem((color_table_size + 1) * sizeof(color_def_t));

  curNode = curNode->xmlChildrenNode;
  while(curNode != NULL)
  {
    char color_name[ATTR_LENGTH];
    char color_rgb[ATTR_LENGTH];
    if ((!xmlStrcmp(curNode->name, (const xmlChar *)"colorDefinition")))
    {
      attrPtr = curNode->properties;
      while (attrPtr != NULL)
      {
        if (!xmlStrcmp(attrPtr->name, BAD_CAST "index"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "index");
          index = atoi((const char *)szAttr);
          xmlFree(szAttr);
          if (max_color_index < index)
            max_color_index = index;
        }
        else if (!xmlStrcmp(attrPtr->name, BAD_CAST "name"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "name");
          strcpy(color_name, (const char *)szAttr);
          xmlFree(szAttr);
        }
        else if (!xmlStrcmp(attrPtr->name, BAD_CAST "rgb"))
        {
          xmlChar* szAttr = xmlGetProp(curNode, BAD_CAST "rgb");
          strcpy(color_rgb, (const char *)szAttr);
          xmlFree(szAttr);
        }
        attrPtr = attrPtr->next;
      }
      strcpy(color_table[index].name, color_name);
      strcpy(color_table[index].rgb, color_rgb);
      sprintf(color_table[index].ensemble, "%s [#%d]", color_name, index);
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
void free_color_table(void)
{
  free_mem(color_table);
}
